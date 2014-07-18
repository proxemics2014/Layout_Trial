	#include <ros/ros.h>
	#include <sensor_msgs/Joy.h>
	#include <geometry_msgs/Twist.h>
	#include "Drive.h"
	#include <sensor_msgs/LaserScan.h>
	#include <iostream>
    #include "Performance.h"
	
	#ifndef TELEOPTURTLE_H_
	#define TELEOPTURTLE_H_
	float near = 5.0;
	float far = 0.25;
	float middle = 2.38;
	int Q=-1,chosenfunction;
	float REQ_POS[] = {middle,middle,far,near,far,near,far,0,far,0,far,0,far,0,far,far,far,far,far};
	double w;
	class TeleopTurtle
	{
	 public:
	  TeleopTurtle()
	  {
	  linear_ = 1;
	  angular_= 2;
	  }
	  float dist;
	  geometry_msgs::Twist vel;
	  void init_func();
	  void return_array();
	 private:
	  void lasercall(const sensor_msgs::LaserScan::ConstPtr& laser);
	  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
	  void joyCall(const sensor_msgs::Joy::ConstPtr& wii);
	  ros::NodeHandle nh_;
	  int flag;
	  int linear_, angular_;
	  ros::Publisher vel_pub_;
	  ros::Subscriber joy_sub_;
	  ros::Subscriber wii_sub_;
	  ros::Subscriber laser_sub_;
	};
	
	void TeleopTurtle::return_array ()
	{
	//   REQ_POS[] = {middle,middle,far,near,far,near,far,0,far,0,far,0,far,0,far,far,far,far,far};
	  //float* ptrm;
	   float Seq[] = {0.25, 0.75, 1.25, 1.75, 2.25, 2.75, 3.25, 3.75, 4.25,4.75};  
	   float *ptr = new float[4];
	   for(int i=0;i<4;i++)
	   {
	    ptr[i] = Seq[rand()%10];
	   }
	//  ptrm = random_num();
	   for(int i=0,j=7; i<4;i++,j+=2)
	    { 
	      REQ_POS[j] = *(ptr+i);
	    }
	 }
	
	//TeleopTurtle::TeleopTurtle():
	
	
	void TeleopTurtle::init_func()
	{
	  nh_.param("axis_linear", linear_, linear_);
	  vel_pub_ = nh_.advertise<geometry_msgs::Twist>("RosAria/cmd_vel", 1);
	  //laser_sub_ = nh_.subscribe<sensor_msgs::LaserScan>("/scan", 50, &TeleopTurtle::lasercall, this);
	  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 100, &TeleopTurtle::joyCallback, this);
	  wii_sub_ = nh_.subscribe<sensor_msgs::Joy>("/wiimote/nunchuk", 10, &TeleopTurtle::joyCall, this); 
	  ROS_INFO("Value of i is: %d",Q);
	  return_array(); 
	  chosenfunction = 2;
	}
	
//////////////-------------JOY CALL--------------///////////////////////
	void TeleopTurtle::joyCall(const sensor_msgs::Joy::ConstPtr& wii)
	{
	    if(flag==0)
	    {
	        ROS_INFO("Entering Wiimote function call");
	        vel = user_drive_fb(0.27*wii->axes[linear_], vel); // vel belongs to geometry_msgs::Twist 
	        vel_pub_.publish(vel);
	        ROS_INFO("Wiimote control with VALUE: %f and FLAG: %d",vel.linear.x,flag);
	    		if(wii->buttons[0] == 1)   // escape sequence for user by pressing the A button
	         	{
	            flag = 1;
	            joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 100, &TeleopTurtle::joyCallback, this); // coming out of function
	            }	
	    }
	    else
	       joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 100, &TeleopTurtle::joyCallback, this); // coming out of function
	}
	
////////////----------LASER CALL------------////////////////////////////////
	void TeleopTurtle::lasercall(const sensor_msgs::LaserScan::ConstPtr& laser)
	{
	  ROS_INFO("INSIDE THE LASERSCAN FUNCTION and value of flag is: %d", flag);
	  if(flag==1)
      {
		joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 100, &TeleopTurtle::joyCallback, this);
	  }
	  if(flag==2)
	  {
	   	dist = laser->ranges[0];
	   	file_handling(dist);
	    if (((REQ_POS[Q]) != dist))
	    { 
	       ROS_INFO("The distance is: %f",dist);
	       if(dist==0)
	       {
	        ROS_INFO("The robot is going backward!");
	        vel=user_drive_fb(-0.2,vel); 
	       }
	       else if(((dist-0.05)<=(REQ_POS[Q])) && (REQ_POS[Q]<=(dist+0.05)))
	       {
	        ROS_INFO("Stopping Robot");
	        flag =1;
	        vel = user_drive_fb(0.0,vel);
	        vel_pub_.publish(vel);    
		   }
		   else if((REQ_POS[Q])>(dist+0.05) && (dist !=0))
	       { 
	        ROS_INFO("The robot is going forward!");
	        ROS_INFO("Going to: %f",(REQ_POS[Q]));
	        ROS_INFO("THE VALUE OF I is: %d", Q);
	        ROS_INFO(" ");
	        vel= user_drive_fb(0.2, vel);
	        }
	        else
	        {
	            ROS_INFO("The robot is going backward!");
	            ROS_INFO("Going to: %f",(REQ_POS[Q]));
	            ROS_INFO("THE VALUE OF I is: %d", Q);
	            ROS_INFO(" ");
	            vel=user_drive_fb(-0.2,vel);
	        }
	    }
	    vel_pub_.publish(vel);
	  }    
	  if (flag ==3)
	  {
		 dist = laser->ranges[0];
	  	 w = functioncall(chosenfunction,double(dist));
	     ROS_INFO("Value of W: %f and value of compare: %f",w,compare);
	     ros::Duration(2.0).sleep();
	     if(w>compare)
	      {
	    	  ROS_INFO("I am going to give you the wrong answerr!");
	    	  //ros::Duration(2.0).sleep();
	    	  flag = 1;
	          //joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 100, &TeleopTurtle::joyCallback, this);
	      }
	     else
	      {
		 	  ROS_INFO("I WILL LOOK AT THE CORRECT PLACE!");
			  //ros::Duration(2.0).sleep();
			  flag = 1;	      	
	          //joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 100, &TeleopTurtle::joyCallback, this);
	      }
	//     ros::Duration(2.0).sleep();
	//     flag ==1;
	  }
      else
	  {
		joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 100, &TeleopTurtle::joyCallback, this);
	  } 
    }

///////////////---------JOY CALL BACK-------------///////////////////
	void TeleopTurtle::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
	{
      if(((joy->buttons[11]==0)&&(joy->buttons[3]==1)) && (joy->buttons[0]==1))
	   {
	     ros::Duration(1.0).sleep();
	     if (((Q==7) || (Q==9)) || ((Q==11) || (Q==13)))
	     {
			flag = 3;
			ROS_INFO("Switching to Interaction Performance");
			ros::Duration(1.0).sleep();
			laser_sub_= nh_.subscribe<sensor_msgs::LaserScan>("/scan",10,&TeleopTurtle::lasercall, this);
		}
	   /*  w = functioncall(chosenfunction,double(dist));
	     ROS_INFO("Value of W: %f and value of compare: %f",w,compare);
	     if(w>compare)
	      {
	    	  ROS_INFO("I am going to give you the wrong answerr!");
	          joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 100, &TeleopTurtle::joyCallback, this);
	      }
	     else
	      {
		 	  ROS_INFO("I WILL LOOK AT THE CORRECT PLACE!");
	          joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 100, &TeleopTurtle::joyCallback, this);
	      }
	     ros::Duration(2.0).sleep(); */
	   }
	
	  if((joy->buttons[11]==0)&&(joy->buttons[2]==1) && (joy->buttons[1]==1))
	   { 
	     ros::Duration(1.0).sleep();
		 ros::spinOnce();			 
		 Q++;
	     if(Q<18)
	       {
	        flag = 2;
	        ROS_INFO("WILL NOW CALL LASERSCAN");
	        laser_sub_ = nh_.subscribe<sensor_msgs::LaserScan>("/scan", 10, &TeleopTurtle::lasercall, this);  
	       }
	      else
	      {
	        ROS_INFO("I shouldn't be here!");
		  }
	   } 
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
		      vel= user_drive_fb(0.2*joy->buttons[4], vel);
	  		  ROS_INFO("You are going UP with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
	        }
	      if (joy->buttons[5] == 1)
	        {
	          vel = user_drive_lr(-0.4*joy->buttons[5],vel);
	          ROS_INFO("You are going RIGHT with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
	        }
	      if (joy->buttons[6] == 1)
	        {
	          vel= user_drive_fb(-0.2*joy->buttons[6],vel);
	          ROS_INFO("You are going DOWN with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
	        }
	      if (joy->buttons[7] == 1)
	        {
	          vel = user_drive_lr(0.4*joy->buttons[7],vel);
	          ROS_INFO("You are going LEFT with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
	        }
	      if(joy->buttons[8] == 1 && joy->buttons[9] == 1)
	        {
	          flag=1;
	          vel = user_drive_fb(0*joy->buttons[8],vel);
	          vel.angular.z = 0*joy->buttons[8];
	          ROS_INFO("You have stopped with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
	  		} 
	      vel_pub_.publish(vel);
	    } 
	    ROS_INFO("Linear_Vel = %f, Angular_Vel = 0", vel.linear.x);
	    ROS_INFO(" ");
	}  
#endif
	
