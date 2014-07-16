#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>


//geometry_msgs::Twist user_drive_fb(int,geometry_msgs::Twist);
#ifndef USER_DRIVE_FB_H_
#define USER_DRIVE_FB_H_

geometry_msgs::Twist user_drive_fb(float x, geometry_msgs::Twist v) // user_drive function 
{
    
    ros::Rate r(10);	
    v.linear.x = x;
    
    r.sleep();
//    ros::Publisher vel_pub_;
   // ROS_INFO("In drive function vel : %f",v.linear.x);	
  //  vel_pub_ = nh_.advertise<geometry_msgs::Twist>("RosAria/cmd_vel", 1);
  //  vel_pub_.publish(v);
 return v;
}
#endif


#ifndef USER_DRIVE_LR_H_
#define USER_DRIVE_LR_H_

geometry_msgs::Twist user_drive_lr(float x, geometry_msgs::Twist v) // user_drive function 
{

       ros::Rate r(10);
	v.angular.z= x;
	return v;
}
#endif
