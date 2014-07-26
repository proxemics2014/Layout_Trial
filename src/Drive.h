#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/LaserScan.h>
#include "laserscan.h"
#include <fstream>
#include <iostream>
#include <time.h>
using namespace std;


#ifndef USER_DRIVE_F_H_
#define USER_DRIVE_F_H_

geometry_msgs::Twist drive_f(float x, geometry_msgs::Twist v, float distance_value) // user_drive function 
{
  v.linear.x = x;
  return v;
}
#endif

#ifndef USER_DRIVE_FB_H_
#define USER_DRIVE_FB_H_

geometry_msgs::Twist drive_fb(float x, geometry_msgs::Twist v, float distance_value) // user_drive function 
{
  //ros::Duration(0.5).sleep();
  v.linear.x = x;
  return v;
}
#endif

#ifndef USER_DRIVE_B_H_
#define USER_DRIVE_B_H_


geometry_msgs::Twist drive_b(float x, geometry_msgs::Twist v, float distance_value) // user_drive function 
{
  //ros::Duration(0.5).sleep();
  v.linear.x = x;
  return v;
}
#endif



#ifndef USER_DRIVE_LR_H_
#define USER_DRIVE_LR_H_

geometry_msgs::Twist drive_lr(float x, geometry_msgs::Twist v,float distance_value) // user_drive function 
{
  //ros::Duration(0.5).sleep();
  v.angular.z = x;
  return v;
}
#endif

#ifndef RANDOM_NUM_H_
#define RANDOM_NUM_H_

float* random_num()
{
  srand (time(NULL));
  float Seq[] = {0.25, 0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25,4.75};  
  float *ptr = new float[4];
  for(int i=0;i<4;i++)
    {
      ptr[i] = Seq[rand()%10];
    }
  return ptr;
}

#endif

// Create a random number generator function which takes in an integer variable as an input and returns rand() % i !! if i is the argument
#ifndef RANDOM_PROB_H_
#define	RANDOM_PROB_H_ 
double random_prob()
{
  double u;
  srand (signed(time(NULL)));
  u=(double)rand()/(RAND_MAX);
  return u;
}
#endif

#ifndef RANDOM_NUMBER_GEN_H_
#define RANDOM_NUMBER_GEN_H_
int rand_num_gen(int n)
{
  srand (time(NULL));
  int r;
  r = rand() % n;
  return r;
}
#endif

#ifndef RETURN_DISTANCE_H_
#define RETURN_DISTANCE_H_
float return_distance(sensor_msgs::LaserScan laser)
{
  return laser.ranges[0];
}
#endif

#ifndef DRIVE_FUNCTION_H_
#define DRIVE_FUNCTION_H_
geometry_msgs::Twist driver(float linearError)
{
      geometry_msgs::Twist velo;
      float kp_linear_ = 0.75;
      geometry_msgs::Twist vel;
      velo.linear.x = kp_linear_ * linearError;
      //vel.angular.z = kp_angular_ * angularError;
      float vel_linear_threshold_ = 0.025;
     // float vel_angular_threshold_ = 0.025;
      if (abs(velo.linear.x) < vel_linear_threshold_) velo.linear.x = 0.0;
      //if (abs(vel.angular.z) < vel_angular_threshold_) vel.angular.z = 0.0;     
    return velo;
    }
#endif /*Drive_Function_H_ */
