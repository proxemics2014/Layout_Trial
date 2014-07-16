#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include <iostream>

geometry_msgs::Twist user_drive_fb(int x, geometry_msgs::Twist v) // user_drive function 
{
	ROS_INFO("I am here! " );
	v.linear.x = x;
	return v;
}

