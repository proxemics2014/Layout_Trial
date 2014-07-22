#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>

#ifndef USER_DRIVE_LR_H_
#define USER_DRIVE_LR_H_

geometry_msgs::Twist user_drive_lr(float x, geometry_msgs::Twist v) // user_drive function 
{

       ros::Rate r(10);
	v.angular.z= x;
	return v;
}
#endif
