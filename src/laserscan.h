#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
// #include <geometry_msgs/Twist.h>

#ifndef LASERSCAN_H_
#define LASERSCAN_H_

class LaserScan
{
 public:
  LaserScan();
  // geometry_msgs::Twist vel;
  float dist;
  void lasercall(const sensor_msgs::LaserScan::ConstPtr& laser);
  
  ros::NodeHandle nh_laser_;
  
  float min_angle_, max_angle_;
  ros::Subscriber laser_sub_;
};

LaserScan::LaserScan()
{
  laser_sub_ = nh_laser_.subscribe<sensor_msgs::LaserScan>("/scan", 50, &LaserScan::lasercall, this);
}

void LaserScan::lasercall(const sensor_msgs::LaserScan::ConstPtr& laser)
{
  ROS_INFO("INSIDE THE LASERSCAN FUNCTION");
  
  //laser->angle_min = min_angle_;
  //laser->angle_max = max_angle_;
  
  dist = laser->ranges[0];
  ros::Rate r(1.0);
  
  ROS_INFO("The min angle is: %f and the max angle is: %f", laser->angle_min, laser->angle_max);
  ROS_INFO("The distance value is: %f", dist);
  
}

#endif /* laserscan */
