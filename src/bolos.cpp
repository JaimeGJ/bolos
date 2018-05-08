#include "ros/ros.h"
#include "std_msgs/Byte.h"
#include "stdlib.h"
#include <time.h>
#include <math.h>
#include "wiringPi.h"
#include <stdio.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Vector3.h>
#include <nav_msgs/Odometry.h>

#define control_enable 27    //36 fisico

    nav_msgs::Odometry odom_msgs;
    geometry_msgs::Vector3 us_msgs;
    geometry_msgs::Twist vel_msg;
    
void callback(const nav_msgs::Odometry::ConstPtr& msgs){
		odom_msgs.pose.pose.position.x=msgs->pose.pose.position.x;
		odom_msgs.pose.pose.orientation.z=msgs->pose.pose.orientation.z;
		 
}

void us_callback(const geometry_msgs::Vector3::ConstPtr& msgs){
		us_msgs.y=msgs->y;
		us_msgs.x= msgs->x;
		}


void reset(){
	//AÑADIR AL PPIO DEL CÓDIGO:   #define control_enable 27   
	pinMode(control_enable, OUTPUT);
	digitalWrite(control_enable, LOW);
	delay(100);
	digitalWrite(control_enable, HIGH);
}


int main (int argc, char  **argv)
{

	wiringPiSetup();
	pinMode(0, INPUT);      // Set pin 0 to INPUT  Delantero izquierda
	pinMode(3, INPUT);      // Set pin 3 to INPUT  Delantero derecha
	
	int limite_i = digitalRead(0);     // Read pin 0
    int limite_d = digitalRead(3);      // Read pin 3
    
    //Starts a new node
    ros::init(argc, argv,"bolos_nodo");
    ROS_INFO("El nodo bolos_nodo ha sido lanzado");
    ros::NodeHandle n;
    ros::Subscriber sub_odom = n.subscribe("/micro/odom",10,callback);
    ros::Subscriber sub_us = n.subscribe("/micro/us",10,us_callback);
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);
    
    reset();
	
	/* estados:
	1 = avanzar hasta 0.6 metros
	2 = girar
	3 = tomar medidas
	4 = apuntar
	5 = acercarse
	6 = retroceder
	7 = disparar
	*/
	
	int estado = 1;
	int giro = 1;
	float vel_lineal = 0.0;
	float vel_angular = 0.0;
	ros::Rate loop_rate(50);
	
	int apuntado = us_msgs.y;
    int cercano;
    float medidas[21] = {};
    float contador = 0;
	while (ros::ok())
    {
		switch(estado){
			case 1:  // Avanzar hasta 0.6 metros
				ROS_INFO("Vamos a avanzar");
				if (odom_msgs.pose.pose.position.x < 0.6 ||  us_msgs.x < 0.5)){
					vel_lineal = 0.1;
					printf("posicion x = %.3f  \n",msgs->pose.pose.position.x);
				}
				
				else{
					vel_lineal = 0.0;
					estado = 2;
				}
			case 2:		// Girar 90 grados
				ROS_INFO("Vamos a girar");
				if (odom_msgs.pose.pose.orientation.z < 1.1344){ //1.581 pi/2, 0.785 pi/4, 1.1344  65º
					//vel_lineal = 0.1;
					vel_angular = 0.2;
				}
				else{
					//vel_lineal = 0.0;
					vel_angular = 0.0;
					estado = 3;
				}
			case 3:		// Tomar medidas
				ROS_INFO("Vamos a tomar medidas");
				apuntado = us_msgs.y;	//2.5º =0.04363
				if (contador <= 21){
					if (abs(odom_msgs.pose.pose.orientation.z-1.344-(contador*0.04363))<= 0.02){ //1.581 pi/2, 0.785 pi/4, 1.1344  65º, 2.0173 115º
						medidas[contador] = {apuntado};
						contador++;
						ROS_INFO("Medida [%f] : [%f] ", contador, apuntado);
					}
					else{
						giro = 1;
						vel_angular = 0.1;					
					}
				}
				else{
					vel_angular = 0.0;
					cercano = min(medidas);
					estado = 4;
				}
			case 4:		//Apuntar
				ROS_INFO("Vamos a apuntar");
				apuntado = us_msgs.y;
				if (abs(apuntado - cercano)>= 0.02){
					giro = -1;
					vel_angular = 0.1;
				}
				else{
					ROS_INFO("Apuntado");
					vel_angular = 0.0;
					estado = 5
				}
			case 5:		// Acercarse hasta la línea negra
				ROS_INFO("Vamos a acercarnos");
				limite_i = digitalRead(0);      // Read pin 0
				limite_d = digitalRead(3);     // Read pin 3
				if (limite_i && limite_d){
					vel_lineal = 0.1;
				}
				else {
					vel_lineal = 0.0;
					estado = 6
				}
			case 6:		// Retroceder
				ROS_INFO("Tomando distancia de seguridad de 2.5 cm");
				vel_lineal = -0.05;
				vel_msg.linear.x = vel_lineal;
				vel_msg.linear.y = 0;
				vel_msg.linear.z = 0;
				vel_msg.angular.x = 0;
				vel_msg.angular.y = 0;
				vel_msg.angular.z = 0.0;
				pub.publish(vel_msg);
				delay(500);
				vel_lineal = 0.0;
				estado = 7;
			case 7:
				vel_lineal = 0.0;
				vel_angular = 0.0;
				//Soltar barra
			}
		
		vel_msg.linear.x = vel_lineal;
		vel_msg.linear.y = 0;
		vel_msg.linear.z = 0;
		vel_msg.angular.x = 0;
		vel_msg.angular.y = 0;
		vel_msg.angular.z = giro*vel_angular;
		pub.publish(vel_msg);
		ros::spinOnce();

        loop_rate.sleep();
	} 
	
}
