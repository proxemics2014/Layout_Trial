#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include "Drive.h"
#include <sensor_msgs/LaserScan.h>
#include <iostream>
#include "Performance.h"
#include "chooseFunction.h"
#include <algorithm>

#ifndef TELEOPTURTLE_H_
#define TELEOPTURTLE_H_
float  middle_user = 0.0,middleN_user=0.0;
float near_user = 0.0, nearN_user = 0.0;
float far_user = 0.0, farN_user =0.0;
float home=0.25; // drive to home position
float user=3.0; // drive up to user
float pos=0.0;
int Q=-1,chosenfunction;
int ste=0,p=0,counting=0;
//float REQ_POS[] = {middle,middle,far,near,far,near,far,0,far,0,far,0,far,0,far,far,far,far,far};
//float REQ_POS[] = {home,middle_user,user,middleN_user,home,far_user,user,farN_user,home,near_user,user,nearN_user,home,0,home,0,home,0,home,0,home,0,home,0,home,home,home};	
float AUTO_POS[] = {home,user,home,user,home,user,home,0,home,0,home,0,home,0,home,0,home,0,home,home};
float shuffled_pos[] = {0,0,0,0,0,0};
//float autonomous = {0,home,0,home,0,home,0,home,0,home,home,home,home,home};  *Make this a pointer array? Because size is not known!  *
float user_inputs[] = {middle_user,middleN_user,nearN_user,near_user,far_user,farN_user}; // make driving to home autonomous? 
double perf_dist,randomized,p_v=0.0;
class TeleopTurtle
{
 public:
  TeleopTurtle()
    {
      linear_ = 1;
      angular_= 2;
    }
  float dist;
  int flag;
  geometry_msgs::Twist vel;
  void init_func();
  void return_array(double);
  void buttonPress(float);
  void increment();
 private:
  void lasercall(const sensor_msgs::LaserScan::ConstPtr& laser);
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
  void joyCall(const sensor_msgs::Joy::ConstPtr& wii);
  ros::NodeHandle nh_;
  int linear_, angular_;
  ros::Publisher vel_pub_;
  ros::Subscriber joy_sub_;
  ros::Subscriber wii_sub_;
  ros::Subscriber laser_sub_;
};
void TeleopTurtle::return_array (double pv)
{
  // REQ_POS[] = {middle,middle,far,near,far,near,far,0,far,0,far,0,far,0,far,far,far,far,far};
  //float* ptrm;
  float Seq[] = {user*0.05, user*.15, user*0.25, user*0.35, user*0.45, user*0.55, user*0.65, user*0.75, user*0.85,user*0.95};  
  float *ptr = new float[4];
  srand (time(NULL));
  for(int i=0;i<4;i++)
    {
      ptr[i] = Seq[rand()%10];
    }
  //  ptrm = random_num();
  for(int i=0; i<4;i++)
    { 
      shuffled_pos[i] = *(ptr+i);
    }
  shuffled_pos[4]=pv;
}
//TeleopTurtle::TeleopTurtle():
void TeleopTurtle::init_func()
{
  nh_.param("axis_linear", linear_, linear_);
  vel_pub_ = nh_.advertise<geometry_msgs::Twist>("RosAria/cmd_vel", 1);
  //laser_sub_ = nh_.subscribe<sensor_msgs::LaserScan>("/scan", 50, &TeleopTurtle::lasercall, this);
  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 10, &TeleopTurtle::joyCallback, this);
  wii_sub_ = nh_.subscribe<sensor_msgs::Joy>("/wiimote/nunchuk", 10, &TeleopTurtle::joyCall, this); 
  ROS_INFO("Value of i is: %d",Q);
  chosenfunction = chooseFunction();   // this function reads DATA from the file and correspodingly returns the CHOSEN FUNCTION!
  p_v=float(peak_value(chosenfunction));
  return_array(p_v); 
  cout<<"\n\nPeak value of this function is: "<<p_v;
  //srand (time(NULL));
  ROS_INFO("Value of Chosen Function: %d",chosenfunction);
}
//////////////-------------JOY CALL--------------///////////////////////
void TeleopTurtle::joyCall(const sensor_msgs::Joy::ConstPtr& wii)
{
  if(flag==0)
    {
      ROS_INFO("Entering Wiimote function call");
      vel = drive_fb(-0.5*wii->axes[linear_], vel); // vel belongs to geometry_msgs::Twist  CHANGED TO SUIT THE USER!!
      vel_pub_.publish(vel);
      ROS_INFO("Wiimote control with VALUE: %f and FLAG: %d",vel.linear.x,flag);
      if(wii->buttons[0] == 1)   // escape sequence for user by pressing the A button
	{
	  //flag = 1;
	  //joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 10, &TeleopTurtle::joyCallback, this); // coming out of function
	  flag=3;
	  laser_sub_=nh_.subscribe<sensor_msgs::LaserScan>("/scan", 10, &TeleopTurtle::lasercall,this);	
	}	
    }
  else
    joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 10, &TeleopTurtle::joyCallback, this); // coming out of function
}
////////////----------LASER CALL------------////////////////////////////////
void TeleopTurtle::lasercall(const sensor_msgs::LaserScan::ConstPtr& laser)
{	
  //ROS_INFO("INSIDE THE LASERSCAN FUNCTION and value of flag is: %d", flag);
  if(flag==1)
    {
      joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 10, &TeleopTurtle::joyCallback, this);
    }
  if(flag==2)
    {  
      
      dist = laser->ranges[0];
      if (((AUTO_POS[Q]) != dist))
	{ 
	  ROS_INFO("The distance is: %f",dist);
	  if(dist==0)
	    {
	      ROS_INFO("The robot is going backward!");
	      vel=drive_fb(-0.2,vel); 
	    }
	  else if(((dist-0.05)<=(AUTO_POS[Q])) && (AUTO_POS[Q]<=(dist+0.05)))
	    {
	      file_handling(dist);
	      ROS_INFO("Stopping Robot");
	      flag =1;
	      vel = drive_fb(0.0,vel);
	      vel_pub_.publish(vel);    
	      system("mplayer /home/robotlab/catkin_ws/src/Layout_Trial/src/WINNER.mp3");
	      cout<<"\n\n\n\n--------------------Value of shuffled array: ---------------------";
	      for(int i=0,j=7;i<=5;i++,j+=2)
	     	{
		  AUTO_POS[j]=shuffled_pos[i];
		  cout<<shuffled_pos[i]<< " ";
	     	}
	      joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 10, &TeleopTurtle::joyCallback, this);
	    }
	  else if((AUTO_POS[Q])>(dist+0.05) && (dist !=0))
	    { 
	      ROS_INFO("The robot is going forward!");
	      ROS_INFO("Going to: %f",(AUTO_POS[Q]));
	      ROS_INFO("THE VALUE OF Q is: %d", Q);
	      ROS_INFO("THE VALUE OF Q is: %d", Q);
	      ROS_INFO(" ");
	      vel= drive_fb(0.4, vel);
	    }
	  else
	    {
	      ROS_INFO("The robot is going backward!");
	      ROS_INFO("Going to: %f",(AUTO_POS[Q]));
	      ROS_INFO("THE VALUE OF Q is: %d", Q);
	      ROS_INFO("THE VALUE OF Q is: %d", Q);
	      ROS_INFO(" ");
	      vel=drive_fb(-0.4,vel);
	    }
	}
      vel_pub_.publish(vel);
    }    
  if ((flag >= 4) && (flag <8))
    {
      dist = laser->ranges[0];
      buttonPress(dist);
      flag =1;
      joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 10, &TeleopTurtle::joyCallback, this);
    }
  if(flag==3)
    {
      flag=1;
      dist = laser->ranges[0];
      user_inputs[p++]= dist;
      shuffled_pos[5] = (user_inputs[0]+user_inputs[1])/2;
      cout<<"Writing distance to file!!! ";
      file_handling(dist);
      //joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 10, &TeleopTurtle::joyCallback, this);
    }
  else
    {
      joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 10, &TeleopTurtle::joyCallback, this);
    } 
}

///////////////---------JOY CALL BACK-------------///////////////////
void TeleopTurtle::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  //if((joy->buttons[11]==0)&&(joy->buttons[2]==1) && (joy->buttons[1]==1))
  if(joy->buttons[3]==1 && joy->buttons[0]==1 && joy->buttons[11]==0)
    {
      if(counting==0)
	{
	  random_shuffle(&shuffled_pos[0],&shuffled_pos[5]);
	  cout<<"\n\n\n\n--------------------Value of shuffled array: ---------------------";
	  for(int i=0,j=7;i<=5;i++,j+=2)
	    {
	      AUTO_POS[j]=shuffled_pos[i];
	      cout<<shuffled_pos[i]<< " ";
	    }
	  counting=1;
	}
      
      ros::Rate(10);	 
      ros::Duration(2.0).sleep();
      if(Q <18)
	{
	  //increment(); // increments Q
	  Q+=1;
	  flag = 2;
	  ROS_INFO("WILL NOW CALL LASERSCAN!!");
	  ROS_INFO("ITERATION is: %d", Q);
	  cout<<"\n\n\n\n--------------------Value of shuffled array: ---------------------";
	  for(int i=0,j=7;i<=5;i++,j+=2)
	    {
	      cout<<shuffled_pos[i]<< " ";
	    }
	  //ROS_INFO("THE VALUE OF JOY BUTTONS 2 is: %d", joy->buttons[2]);
	  laser_sub_ = nh_.subscribe<sensor_msgs::LaserScan>("/scan", 10, &TeleopTurtle::lasercall, this);  
	}
    } 	
  if((joy->buttons[10] == 1))
    {
      flag = 0;
      ROS_INFO("Switching to Wiimote Controller");
      wii_sub_ = nh_.subscribe<sensor_msgs::Joy>("/wiimote/nunchuk", 10, &TeleopTurtle::joyCall, this);
    }
  if(joy->buttons[12]==1) 
    {
      ROS_INFO("Pressed triangle");
      ros::Duration(1.0).sleep();
      flag=4;
      laser_sub_= nh_.subscribe<sensor_msgs::LaserScan>("/scan",10,&TeleopTurtle::lasercall, this);
    }
  if (joy->buttons[13]==1) 
    {
      ROS_INFO("Pressed circle");	
      ros::Duration(1.0).sleep();	
      flag = 5;
      laser_sub_= nh_.subscribe<sensor_msgs::LaserScan>("/scan",10,&TeleopTurtle::lasercall, this);
    }
  if (joy->buttons[14]==1) 
    {
      ROS_INFO("Pressed cross");    	
      ros::Duration(1.0).sleep();	
      flag = 6;
      laser_sub_= nh_.subscribe<sensor_msgs::LaserScan>("/scan",10,&TeleopTurtle::lasercall, this);
    }
  if(joy->buttons[15]==1)
    {
      ROS_INFO("Pressed square");
      ros::Duration(1.0).sleep();
      flag =7;
      laser_sub_= nh_.subscribe<sensor_msgs::LaserScan>("/scan",10,&TeleopTurtle::lasercall, this);
    }
  if(joy->buttons[11]==1)
    {
      flag=1;
      ROS_INFO("Working with PS3 Controller");
      
      if (joy->buttons[4] == 1)
	{
	  vel= drive_fb(0.4*joy->buttons[4], vel);
	  ROS_INFO("You are going UP with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
	}
      if (joy->buttons[5] == 1)  
	{
	  vel = drive_lr(-0.5*joy->buttons[5],vel);
	  ROS_INFO("You are going RIGHT with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
	}
      if (joy->buttons[6] == 1)
	{
	  vel= drive_fb(-0.5*joy->buttons[6],vel);
	  ROS_INFO("You are going DOWN with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
	}
      if (joy->buttons[7] == 1)
	{
	  vel = drive_lr(0.4*joy->buttons[7],vel);
	  ROS_INFO("You are going LEFT with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
	}
      if(joy->buttons[8] == 1 && joy->buttons[9] == 1)
	{
	  flag=1;
	  vel = drive_fb(0*joy->buttons[8],vel);
	  vel.angular.z = 0*joy->buttons[8];
	  ROS_INFO("You have stopped with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
	} 
      vel_pub_.publish(vel);
    } 
  ROS_INFO("Linear_Vel = %f, Angular_Vel = 0", vel.linear.x);
  ROS_INFO(" ");
  //   ROS_INFO("THE VALUE OF JOY BUTTONS 2 is: %d", joy->buttons[2]);
}  
void TeleopTurtle::buttonPress(float u)
{
  perf_dist = functioncall(chosenfunction,double(u));
  randomized = random_prob();
  ROS_INFO("Value of Performance with respect to distance: %f and value of compare: %f",perf_dist,randomized);
  ros::Duration(2.0).sleep();
  if(perf_dist>randomized)
    {
      ROS_INFO("I am going to give you the wrong answerr!");	  	
      ROS_INFO(" ");
      if(flag==4)
	{
	  ROS_INFO("Not looking at TRIANGLE!");
	  ROS_INFO(" ");
	  system("mplayer /home/robotlab/catkin_ws/src/Layout_Trial/src/BUZZER.mp3");
	}
      if(flag==5)
	{
	  ROS_INFO("Not looking at Circle!");
	  ROS_INFO(" ");
	  system("mplayer /home/robotlab/catkin_ws/src/Layout_Trial/src/BUZZER.mp3");
	}
      if(flag==6)
	{
	  ROS_INFO("Not looking at Cross!");
	  ROS_INFO(" ");
	  system("mplayer /home/robotlab/catkin_ws/src/Layout_Trial/src/BUZZER.mp3");
	}
      if(flag==7)
	{
	  ROS_INFO("Not looking at SQUARE!");
	  ROS_INFO(" ");
	  system("mplayer /home/robotlab/catkin_ws/src/Layout_Trial/src/BUZZER.mp3");
	}
      
      //ros::Duration(2.0).sleep();
      flag = 1;
      //joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 100, &TeleopTurtle::joyCallback, this);
    }
  else
    {
      ROS_INFO("I WILL LOOK AT THE CORRECT PLACE!");	  	
      ROS_INFO(" ");
      //ros::Duration(2.0).sleep();
      if(flag==4)
	{
	  ROS_INFO("Looking at TRIANGLE!");
	  ROS_INFO(" ");
	  system("mplayer /home/robotlab/catkin_ws/src/Layout_Trial/src/WINNER.mp3");
	}
      if(flag==5)
	{
	  ROS_INFO("Looking at Circle!");
	  ROS_INFO(" ");
	  system("mplayer /home/robotlab/catkin_ws/src/Layout_Trial/src/WINNER.mp3");
	}
      if(flag==6)
	{
	  ROS_INFO("Looking at Cross!");
	  ROS_INFO(" ");
	  system("mplayer /home/robotlab/catkin_ws/src/Layout_Trial/src/WINNER.mp3");
	}
      if(flag==7)
	{
	  ROS_INFO("Looking at SQUARE!");
	  ROS_INFO(" ");
	  system("mplayer /home/robotlab/catkin_ws/src/Layout_Trial/src/WINNER.mp3");
	}
      flag = 1;	      	
      //joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 100, &TeleopTurtle::joyCallback, this);
    } 
}
void TeleopTurtle::increment()
{	
  ste+=1;
  if (ste==2)
    {
      Q=Q+1;
      ste=0;
    }
}
#endif
