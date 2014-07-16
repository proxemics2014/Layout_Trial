#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include "Drive.h"
#include "DriveLR.h"

// int flag= 1 ; // global variable
#ifndef TELEOPTURTLE_H_
#define TELEOPTURTLE_H_

class TeleopTurtle
{
public:
  TeleopTurtle()
{
  linear_ = 1;
  angular_= 2;
  flag = 1;
}

  geometry_msgs::Twist vel;
  void init_func();
private:
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
  void joyCall(const sensor_msgs::Joy::ConstPtr& wii);

  ros::NodeHandle nh_;
  int flag;
  int linear_, angular_;
  double l_scale_, a_scale_;
  ros::Publisher vel_pub_;
  ros::Subscriber joy_sub_;
  ros::Subscriber wii_sub_;
  
};

//TeleopTurtle::TeleopTurtle():



void TeleopTurtle::init_func()
{
  nh_.param("axis_linear", linear_, linear_);
  nh_.param("scale_linear", l_scale_, l_scale_);
  vel_pub_ = nh_.advertise<geometry_msgs::Twist>("RosAria/cmd_vel", 1);
  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 100, &TeleopTurtle::joyCallback, this);
  wii_sub_ = nh_.subscribe<sensor_msgs::Joy>("/wiimote/nunchuk", 10, &TeleopTurtle::joyCall, this);

}

void TeleopTurtle::joyCall(const sensor_msgs::Joy::ConstPtr& wii)
{
    if(flag==0)
    {
        ROS_INFO("Entering Wiimote function call");
	vel = user_drive_fb(0.27*wii->axes[linear_], vel); // vel belongs to geometry_msgs::Twist 
	vel_pub_.publish(vel);
	ROS_INFO("Wiimote control with VALUE: %f and FLAG: %d",vel.linear.x,flag);
///----------------------------------------------------------///////////////

     if(wii->buttons[0] == 1)   // escape sequence for user by pressing the A button
        {
            flag = 1;
            joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 100, &TeleopTurtle::joyCallback, this); // coming out of function
        }
     }
     else
        joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 100, &TeleopTurtle::joyCallback, this); // coming out of function
}

void TeleopTurtle::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
 float temp =0 ;
  if((joy->buttons[10] == 1))
  {
      flag = 0;
      ROS_INFO("Switching to Wiimote Controller");
      wii_sub_ = nh_.subscribe<sensor_msgs::Joy>("/wiimote/nunchuk", 10, &TeleopTurtle::joyCall, this);

  }
  if(joy->buttons[11]==1)
    {
      flag=1;
      ROS_INFO("Working with PS3 Controller");
      if (joy->buttons[4] == 1)
       {
		temp = joy->buttons[4];
		vel= user_drive_fb(0.2*temp, vel);
		ROS_INFO("You are going UP with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
          	temp = 0;
        }

///----------------------------------------------------------/////
      if (joy->buttons[5] == 1)
      {
	  temp = joy->buttons[5];
          vel = user_drive_lr(-0.4*joy->buttons[5],vel);
          ROS_INFO("You are going RIGHT with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
          temp = 0;
      }

      if (joy->buttons[6] == 1)
      {
	  temp = joy->buttons[6];
          vel= user_drive_fb(-0.2*temp,vel);
          ROS_INFO("You are going DOWN with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
          
temp = 0;
      }
      if (joy->buttons[7] == 1)
      {
	  temp = joy->buttons[7];
          vel = user_drive_lr(0.4*temp,vel);
          ROS_INFO("You are going LEFT with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
 	   temp = 0;     
      }
////-----------------------------------------------------/////////
   
     if(joy->buttons[8] == 1 && joy->buttons[9] == 1)
        {
          flag = 1;
          vel = user_drive_fb(0*joy->buttons[8],vel);
          vel.angular.z = 0*joy->buttons[8];
          ROS_INFO("You have stopped with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
	  
       } 
   vel_pub_.publish(vel);
}
// ROS_INFO("Stopped with : %d , %d", joy->buttons[4],flag);
 ROS_INFO("Linear_Vel = %f, Angular_Vel = 0", vel.linear.x);
 ROS_INFO(" ");
}  

#endif
