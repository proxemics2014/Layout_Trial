	#include <ros/ros.h>
	#include <sensor_msgs/Joy.h>
	#include <geometry_msgs/Twist.h>
	#include "laserscan.h"
	#include <fstream>
	#include <iostream>
	#include <time.h>
	using namespace std;

	
	#ifndef USER_DRIVE_FB_H_
	#define USER_DRIVE_FB_H_
	
	geometry_msgs::Twist drive_fb(float x, geometry_msgs::Twist v) // user_drive function 
	{
	    
//	    ros::Duration(0.5).sleep();	
	    v.linear.x = x;
	    
//	    r.sleep();
	//    ros::Publisher vel_pub_;
	   // ROS_INFO("In drive function vel : %f",v.linear.x);	
	  //  vel_pub_ = nh_.advertise<geometry_msgs::Twist>("RosAria/cmd_vel", 1);
	  //  vel_pub_.publish(v);
	    return v;
	}
	#endif
	
	
	#ifndef USER_DRIVE_LR_H_
	#define USER_DRIVE_LR_H_
	
	geometry_msgs::Twist drive_lr(float x, geometry_msgs::Twist v) // user_drive function 
	{
	
	    ros::Rate r(10);
		v.angular.z= x;
		return v;
	}
	#endif
	/*
	#ifndef AUTO_DRIVE_H_
	#define AUTO_DRIVE_H_
	
	void auto_drive(float REQ_POS)
	{
	  LaserScan l;	
	  while (REQ_POS != l.dist)
	    {
	      if(REQ_POS>l.dist)
	        { 
	          vel.linear.x=0.2;
	        }
	      else if(REQ_POS<l.dist)
	      {
	        vel.linear.x =-0.2;
	      }
	      else
	        break;
	    }
	  }
	#endif
	*/
	#ifndef FILE_HANDLING_H_
	#define FILE_HANDLING_H_
	
	void file_handling (float distance_written)
	{
	  ofstream in;
	  in.open("/home/robotlab/File Read/write.csv", ios::ate | ios::app);
	  if (in.is_open()) 
	  {
	    in << distance_written<<" ";
	    in.close();
	  } else {
	    cout << "\nUnable to open file!";
	  }
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
	
	/*
	#ifndef RETURN_ARRAY_H_
	#define RETURN_ARRAY_H_
	float* return_array ()
	{
	  int sizeof_array = 18;
	  float far = 5.0;
	  float near = 0.25;
	  float middle = 2.38;
	  float REQ_POS_Arr[] = {middle,middle,far,near,far,near,far,0,far,0,far,0,far,0,far,far,far,far,far};
	  float* ptrm;
	  ptrm = random_num();
	  for(int i=0,j=7; i<4;i++,j+=2)
	    { 
	      REQ_POS_Arr[j] = *(ptrm+i);
	    }
	    float *ptrF = new float [18];
	    ptrF = REQ_POS_Arr;
	    cout<<"Printing Array REQ_POS in DRIVE.h : ";
	    for(int j=0;j<18;j++)
	     {
	    cout<<*(ptrF+j)<<" ";
	     } 
	     cout<<endl;
	    return ptrF;
	 }
	#endif
	*/
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

  