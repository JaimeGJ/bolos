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

    nav_msgs::Odometry odom_msgs;
    geometry_msgs::Vector3 us_msgs;
    
void callback(const nav_msgs::Odometry::ConstPtr& msgs){
		odom_msgs.pose.pose.position.x=msgs->pose.pose.position.x;
		odom_msgs.pose.pose.orientation.z=msgs->pose.pose.orientation.z;
}

void us_callback(const geometry_msgs::Vector3::ConstPtr& msgs){
		us_msgs.y=msgs->y;
		}

void posicionar(){


    geometry_msgs::Twist vel_msg;

    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("/micro/odom",10,callback);
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    
    //Receiveing the user's input
    ROS_INFO("Vamos a posicionarnos");

    while (odom_msgs.pose.pose.position.x <= 0.6){
	    vel_msg.linear.x = 20.0;
	    vel_msg.linear.y = 0;
	    vel_msg.linear.z = 0;
	    vel_msg.angular.x = 0;
	    vel_msg.angular.y = 0;
	    vel_msg.angular.z = 0;
	    pub.publish(vel_msg);
	    ros::Subscriber sub = n.subscribe("/micro/odom",10,callback);
	}
    while (odom_msgs.pose.pose.orientation.z <= 1.581){ //1.581 pi/2, 0.785 pi/4
	    vel_msg.linear.x = 0;
	    vel_msg.linear.y = 0;
	    vel_msg.linear.z = 0;
	    vel_msg.angular.x = 0;
	    vel_msg.angular.y = 0;
	    vel_msg.angular.z = 0.4;
	    pub.publish(vel_msg);
	    ros::Subscriber sub = n.subscribe("/micro/odom",10,callback);
	}
     vel_msg.angular.z = 0;
     pub.publish(vel_msg);

}

  

void apuntar(){

    nav_msgs::Odometry odom_msgs;
    geometry_msgs::Twist vel_msg;

    ros::NodeHandle n;
    ros::Subscriber sub_odom = n.subscribe("/micro/odom",10,callback);
    ros::Subscriber sub_us = n.subscribe("/micro/us",10,us_callback);
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);



    int apuntado = us_msgs.y;
    int cercano = 30000;
    int anterior = apuntado;
    int giro = 1;

    while (1){
		ros::Subscriber sub = n.subscribe("/micro/us",10,us_callback);
        if (giro == 1){
	    apuntado=us_msgs.y;
		if (abs(apuntado-anterior) >= 0.05){
		    if (anterior > apuntado){
		        anterior = apuntado;
			    if (abs(apuntado-cercano) >= 0.02){
			        if (apuntado < cercano){
			            cercano = apuntado;
					}
			        else{
			            giro = -1;
					}
				}
			    else{
			        vel_msg.linear.x = 0;
	        	    vel_msg.linear.y = 0;
	        	    vel_msg.linear.z = 0;
	       		    vel_msg.angular.x = 0;
	        	    vel_msg.angular.y = 0;
	        	    vel_msg.angular.z = 0;
	        	    pub.publish(vel_msg);
			        ROS_INFO("Objetivo apuntado");
			        break;
				}
			}
		    else{
			anterior = apuntado;
		        giro = -1;
			}
		}
	    }
        else{
	    apuntado=us_msgs.y;
		if (abs(apuntado-anterior) >= 0.05){
		    if (anterior > apuntado){
		        anterior = apuntado;
			    if (abs(apuntado-cercano) >= 0.02){
			        if (apuntado < cercano){
			            cercano = apuntado;
					}
			        else{
			            giro = 1;
					}
				}
			    else{
			        vel_msg.linear.x = 0;
	        	    vel_msg.linear.y = 0;
	        	    vel_msg.linear.z = 0;
	       		    vel_msg.angular.x = 0;
	        	    vel_msg.angular.y = 0;
	        	    vel_msg.angular.z = 0;
	        	    pub.publish(vel_msg);
			        ROS_INFO("Objetivo apuntado");
			        break;
				}
			}
		    else{
			anterior = apuntado;
		        giro = 1;
			}
		}
	}	
	vel_msg.linear.x = 0;
	vel_msg.linear.y = 0;
	vel_msg.linear.z = 0;
	vel_msg.angular.x = 0;
	vel_msg.angular.y = 0;
	vel_msg.angular.z = giro*0.4;
	pub.publish(vel_msg);
	}
}


void acercar(){
	
	ros::NodeHandle n;
    ros::Publisher pub = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

    geometry_msgs::Twist vel_msg;
    
    //Receiveing the user's input
    ROS_INFO("Vamos a acercarnos");
	
	
    int limite_i = digitalRead(0);     // Read pin 0
    int limite_d = digitalRead(3);      // Read pin 3
    
    while (limite_i && limite_d){	    // Mientras los dos lean blanco, el robot avanzará
	vel_msg.linear.x = 20.0;
	vel_msg.linear.y = 0;
	vel_msg.linear.z = 0;
	vel_msg.angular.x = 0;
	vel_msg.angular.y = 0;
	vel_msg.angular.z = 0;
	pub.publish(vel_msg);
	limite_i = digitalRead(0);      // Read pin 0
	limite_d = digitalRead(3);     // Read pin 3
	}
	
    vel_msg.linear.x = -20.0;		    // Retrocederá durante medio segundo
    vel_msg.linear.y = 0;
    vel_msg.linear.z = 0;
    vel_msg.angular.x = 0;
    vel_msg.angular.y = 0;
    vel_msg.angular.z = 0;
    pub.publish(vel_msg);
    
    delay(500);

    vel_msg.linear.x = 0;
    vel_msg.linear.y = 0;
    vel_msg.linear.z = 0;
    vel_msg.angular.x = 0;
    vel_msg.angular.y = 0;
    vel_msg.angular.z = 0;
    pub.publish(vel_msg);	    // Y se parará
}


int main (int argc, char  **argv)
{

	wiringPiSetup();
	pinMode(0, INPUT);      // Set pin 0 to INPUT  Delantero izquierda
	pinMode(3, INPUT);      // Set pin 3 to INPUT  Delantero derecha

    //Starts a new node
    ros::init(argc, argv,"bolos_nodo");
    ROS_INFO("El nodo bolos_nodo ha sido lanzado");
    posicionar();
    acercar();
	apuntar();

}
