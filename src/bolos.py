#!/usr/bin/env python
import rospy
import wiringpi
from geometry_msgs.msg import Twist, Vector3
from nav_msgs.msg import Odometry

wiringpi.wiringPiSetup() 
#pose.pose.position.x
#pose.pose.orientation.x

def posicionar():

    velocity_publisher = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
    odometry_subscriber = rospy.Subscriber('/micro/odom', Odometry, queue_size=10)
    odom_msgs = Odometry()
    vel_msg = Twist()
    
    #Receiveing the user's input
    print("Vamos a posicionarnos")

    while odom_msgs.pose.pose.position.x <= 0.6:
	    vel_msg.linear.x = 20.0
	    vel_msg.linear.y = 0
	    vel_msg.linear.z = 0
	    vel_msg.angular.x = 0
	    vel_msg.angular.y = 0
	    vel_msg.angular.z = 0
	    velocity_publisher.publish(vel_msg)

    while odom_msgs.pose.pose.orientation.z <= 1.581: #1.581 pi/2, 0.785 pi/4
	    vel_msg.linear.x = 0
	    vel_msg.linear.y = 0
	    vel_msg.linear.z = 0
	    vel_msg.angular.x = 0
	    vel_msg.angular.y = 0
	    vel_msg.angular.z = 0.4
	    velocity_publisher.publish(vel_msg)

     vel_msg.angular.z = 0
     velocity_publisher.publish(vel_msg)



  

def apuntar():

    velocity_publisher = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
    odometry_subscriber = rospy.Subscriber('/micro/odom', Odometry, queue_size=10)
    ultrasonic_subscriber = rospy.Subscriber('/micro/us', Vector3, queue_size=10)
    us_msgs = Vector3()
    odom_msgs = Odometry()
    vel_msg = Twist()

    apuntado = us_msgs.y
    cercano = inf
    anterior = apuntado
    giro = 1

    while 1 
        if giro = 1:
	    apuntado = us_msgs.y #Quizás hay que poner de nuevo que se subscriba pero no sé muy bien cómo irá
		if abs(apuntado-anterior) >= 0.05:
		    if anterior > apuntado:
		        anterior = apuntado
			    if abs(apuntado-cercano) > 0.02:
			        if apuntado < cercano:
			            cercano = apuntado
			        else:
			            giro = -1
			    else:
			        vel_msg.linear.x = 0
	        	        vel_msg.linear.y = 0
	        	        vel_msg.linear.z = 0
	       		        vel_msg.angular.x = 0
	        	        vel_msg.angular.y = 0
	        	        vel_msg.angular.z = 0
	        	        velocity_publisher.publish(vel_msg)
			        #Acercarse y lanzar bola
			        rospy.loginfo("Objetivo apuntado")
			        return 0
		    else:
			anterior = apuntado
		        giro = -1 
        else:
	    apuntado = us_msgs.y #Quizás hay que poner de nuevo que se subscriba pero no sé muy bien cómo irá
		if abs(apuntado-anterior) >= 0.05:
		    if anterior > apuntado:
		        anterior = apuntado
			    if abs(apuntado-cercano) > 0.02:
			        if apuntado < cercano:
			            cercano = apuntado
			        else:
			            giro = 1
			    else:
			        vel_msg.linear.x = 0
	        	        vel_msg.linear.y = 0
	        	        vel_msg.linear.z = 0
	       		        vel_msg.angular.x = 0
	        	        vel_msg.angular.y = 0
	        	        vel_msg.angular.z = 0
	        	        velocity_publisher.publish(vel_msg)
			        #Acercarse y lanzar bola
			        rospy.loginfo("Objetivo apuntado")
			        return 0
		    else:
			anterior = apuntado
		        giro = 1
			
	vel_msg.linear.x = 0
	vel_msg.linear.y = 0
	vel_msg.linear.z = 0
	vel_msg.angular.x = 0
	vel_msg.angular.y = 0
	vel_msg.angular.z = giro*0.4
	velocity_publisher.publish(vel_msg)


def acercar()
    limite_i = wiringpi.digitalRead(0)      # Read pin 0
    limite_d = wiringpi.digitalRead(3)      # Read pin 3
    
    while limite_i and limite_d:	    # Mientras los dos lean blanco, el robot avanzará
	vel_msg.linear.x = 20.0
	vel_msg.linear.y = 0
	vel_msg.linear.z = 0
	vel_msg.angular.x = 0
	vel_msg.angular.y = 0
	vel_msg.angular.z = 0
	velocity_publisher.publish(vel_msg)
	limite_i = wiringpi.digitalRead(0)      # Read pin 0
	limite_d = wiringpi.digitalRead(3)      # Read pin 3

    vel_msg.linear.x = -20.0		    # Retrocederá durante medio segundo
    vel_msg.linear.y = 0
    vel_msg.linear.z = 0
    vel_msg.angular.x = 0
    vel_msg.angular.y = 0
    vel_msg.angular.z = 0
    velocity_publisher.publish(vel_msg)
    
    delay(500)

    vel_msg.linear.x = 0
    vel_msg.linear.y = 0
    vel_msg.linear.z = 0
    vel_msg.angular.x = 0
    vel_msg.angular.y = 0
    vel_msg.angular.z = 0
    velocity_publisher.publish(vel_msg)	    # Y se parará

if __name__ == '__main__':
    try:
	wiringpi.pinMode(0, 0)       # Set pin 0 to 0 ( INPUT ) Delantero izquierda
	wiringpi.pinMode(3, 0)       # Set pin 3 to 0 ( INPUT ) Delantero derecha

        # Starts a new node
        rospy.init_node('bolos_nodo')
        #Testing our function
        posicionar()
	apuntar()
	acercar()
    except rospy.ROSInterruptException: pass
