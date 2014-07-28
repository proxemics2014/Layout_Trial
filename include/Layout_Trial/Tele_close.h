#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>
#include <sensor_msgs/Joy.h>
#include <geometry_msgs/Twist.h>
#include <Drive.h>
#include <iostream>
#include <chooseFunction.h>
#include <algorithm>
#include <Performance.h>
#include <tf/transform_broadcaster.h>
#include <Broadcastpioneer.h>
#include <string>

#ifndef Tele_current_H_
#define Tele_current_H_
float  middle_user = 0.0,middleN_user=0.0;
float near_user = 0.0, nearN_user = 0.0;
float far_user = 0.0, farN_user =0.0;
float offset = 0.45;
int arraySize;
float home=0.5; // drive to home position
float user=5.0; // drive up to user
float user_home = 4.75; // this is the 0.25 position
float pos=0.0;
int Q=-1,chosenfunction,stop=0;
int ste=0,p=0,counting=0;
float AUTO_POS[] = {home,user_home,home,user_home,home,user_home,home,0,home,0,home,0,home,0,home,0,home,0,home,/*WAIT FOR PARTICIPANT TO MOVE ROBOT*/home,user_home,home,home,home,home};
float shuffled_pos[] = {0,0,0,0,0,0};
float user_inputs[] = {middle_user,middleN_user,nearN_user,near_user,far_user,farN_user}; // make driving to home autonomous? 
double perf_dist,randomized,p_v=0.0;

class Tele_current
{
 public:
  Tele_current()
    {
      linear_ = 1;
      angular_= 2;
      dist=0.0;
    }
  float dist;
  int flag;
  geometry_msgs::Twist vel;
  ros::NodeHandle nh_;
  void init_func();
  void return_array(double);
  void buttonPress();
  void increment(); 
 private:
  void lasercall(const sensor_msgs::LaserScan::ConstPtr& laser);
  void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
  void joyCall(const sensor_msgs::Joy::ConstPtr& wii);
  TransformBroadcasterExample broadcaster;
  tf::TransformBroadcaster br;
  int linear_, angular_;
  ros::Publisher vel_pub_;
  ros::Subscriber joy_sub_;
  ros::Subscriber wii_sub_;
  ros::Subscriber laser_sub_;
};
void Tele_current::return_array (double pv)
{
  float Seq[] = {(user*0.14)-offset, (user*0.24)-offset, (user*0.34)-offset, (user*0.44)-offset, (user*0.54)-offset, (user*0.64)-offset, (user*0.74)-offset, (user*0.84)-offset};
  float *ptr = new float[4];
  
  ROS_INFO("Positions: ");
  srand (time(NULL));
  for(int i=0,j=2,k=0;i<4,j<=8,k<=6;i++,j+=2,k+=2)
    {
      ptr[i]=Seq[rand()%(j-k)+k];
    }
  //cout<<"Random positions: ";
  for(int i=0; i<4;i++)
    { 
      shuffled_pos[i] = *(ptr+i);
    }
  shuffled_pos[4]=pv;
  delete ptr;
}
//Tele_current::Tele_current():
void Tele_current::init_func()
{
  nh_.param("axis_linear", linear_, linear_);
  vel_pub_ = nh_.advertise<geometry_msgs::Twist>("RosAria/cmd_vel", 1);
  //laser_sub_ = nh_.subscribe<sensor_msgs::LaserScan>("/scan", 50, &Tele_current::lasercall, this)
  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 1, &Tele_current::joyCallback, this);
  wii_sub_ = nh_.subscribe<sensor_msgs::Joy>("/wiimote/joy", 1, &Tele_current::joyCall, this); 
  ROS_INFO("Value of iteration is: %d",Q);
  chosenfunction = chooseFunction();   // this function reads DATA from the file and correspodingly returns the CHOSEN FUNCTION!
  p_v=float(peak_value(chosenfunction));
  return_array(p_v); 
  cout<<"\n\nPeak value of this function is: "<<p_v;
  ROS_INFO("Value of Chosen Function: %d",chosenfunction);
}
//////////////-------------JOY CALL--------------///////////////////////
void Tele_current::joyCall(const sensor_msgs::Joy::ConstPtr& wii_call)
{
    if(flag==0)
    { 
    laser_sub_=nh_.subscribe<sensor_msgs::LaserScan>("/scan", 1, &Tele_current::lasercall,this); 
    ROS_INFO("Entering Wiimote function call");
    if(wii_call->buttons[8] == 1)
    {
      vel = driver(-wii_call->buttons[8],0.0);
    }
    else if(wii_call->buttons[9] == 1)
    {
      vel = driver(wii_call->buttons[9],0.0);
    }
    else
    {
      vel = driver(0.0,0.0); 
    }
    vel_pub_.publish(vel);
    ROS_INFO("Wiimote control with VEL: %f and distance: %f",vel.linear.x,dist);
    }
    if(wii_call->buttons[10] == 1)   // escape sequence for user by pressing the A button
	    {
       system("mplayer /home/robotlab/catkin_ws/src/Layout_Trial/src/Stop.mp3");  // stopping sound   
       //laser_sub_=nh_.subscribe<sensor_msgs::LaserScan>("/scan", 10, &Tele_current::lasercall,this);
       user_inputs[p++]= dist;
       shuffled_pos[5] = (user_inputs[0]+user_inputs[1])/2;
       if(shuffled_pos[5]==0) { shuffled_pos[5]= -0.25; }
       flag=1;
	    }
  else
    joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 1, &Tele_current::joyCallback, this); // coming out of function
}
////////////----------LASER CALL------------////////////////////////////////
void Tele_current::lasercall(const sensor_msgs::LaserScan::ConstPtr& laser)
{
 arraySize = laser->ranges.size(); 
 dist = laser->ranges[256];
 tf::Transform transform(tf::Quaternion(0,0,0), tf::Vector3((5.0-dist),0,0)); // replace 2 with dist 
 br.sendTransform(tf::StampedTransform (transform, ros::Time::now(),"/pioneer","/world"));
/////////////NEW////////////	
  if(flag==0)
  {
    if(dist>(4.75) || dist<(0.5))
    {
      vel=driver(0.0,0.0);
      flag=1;
      vel_pub_.publish(vel);
    }
  }
 //////////////// 
   if(flag==2 && Q<=21)
    {  
      if(Q==-1) Q=0;
      if (((AUTO_POS[Q]) != dist))
     { 
     ROS_INFO("I Need to GO TO: %f", AUTO_POS[Q]);
     ROS_INFO("The distance is: %f",dist);
     if(dist==0)
      {
        ROS_INFO("The robot is going backward!");
        vel=driver(-0.2,0.0); 
      }
     else if(((dist-0.05)<=(AUTO_POS[Q])) && (AUTO_POS[Q]<=(dist+0.05)))
      {
        file_handling(dist,chosenfunction);
        ROS_INFO("Stopping Robot!");
        perf_dist = functioncall(chosenfunction,double(dist));
        flag =1;
        vel = driver(0.0,0.0);
        vel_pub_.publish(vel);    
        system("mplayer /home/robotlab/catkin_ws/src/Layout_Trial/src/Stop.mp3");  // stopping sound
        ROS_INFO("--------------------Value of shuffled array: ---------------------");
        for(int i=0,j=7;i<=5;i++,j+=2)
        {
        AUTO_POS[j]=shuffled_pos[i];
        ROS_INFO("%f",shuffled_pos[i]);
        }
        joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 1, &Tele_current::joyCallback, this);
      }
     else if((AUTO_POS[Q])>(dist+0.05) && (dist !=0))
      { 
        ROS_INFO("The robot is going forward!");
        ROS_INFO("Going to: %f",(AUTO_POS[Q]));
        ROS_INFO("THE VALUE OF Q is: %d", Q);
        vel= driver(AUTO_POS[Q]-dist,0.0);
      }
    else if(AUTO_POS[Q]<= (dist+0.05))
      {
        ROS_INFO("The robot is going backward!");
        ROS_INFO("Going to: %f",(AUTO_POS[Q]));
        ROS_INFO("THE VALUE OF Q is: %d", Q);
        vel=driver(AUTO_POS[Q]-dist,0.0);
      }
  }
      vel_pub_.publish(vel);
    }    
    if ((flag >= 4) && (flag <8))
    {
      dist = laser->ranges[0];
      buttonPress();
      flag =1;
      joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 1, &Tele_current::joyCallback, this);
    } 
    if(flag==10)
    {
      if(dist <=(2.8)) //user_home-offset
      {
       vel= driver(0.35,0.0); 
      }
      else if(dist>2.8) //user_home-offset
      {
       vel=driver(0.0,0.0);
      }
      vel_pub_.publish(vel);
      }
      else if(flag ==11)
      {
       if(dist >(0.5))
       {
       vel= driver(-0.35,0.0);
       }
       else
        {
        vel=driver(0.0,0.0);
       }
      vel_pub_.publish(vel);
      }
  else 
    {
      joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("/joy", 1, &Tele_current::joyCallback, this);
    } 
}

///////////////---------JOY CALL BACK-------------///////////////////
void Tele_current::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  laser_sub_ = nh_.subscribe<sensor_msgs::LaserScan>("/scan", 1, &Tele_current::lasercall, this);  
  if(joy->buttons[3]==1)
    {
     flag=2;
     if(Q <20)
	   {
	    increment(); // increments Q
	    //Q+=1;
	    ROS_INFO("WILL NOW CALL LASERSCAN!");
	    ROS_INFO("ITERATION is: %d", Q);
	    laser_sub_ = nh_.subscribe<sensor_msgs::LaserScan>("/scan", 1, &Tele_current::lasercall, this);  
	   }
    } 	
  if((joy->buttons[11] == 1))
    {
      flag = 0;
      ROS_INFO("Switching to Wiimote Controller");
      wii_sub_ = nh_.subscribe<sensor_msgs::Joy>("/wiimote/joy", 1, &Tele_current::joyCall, this);
    } 
  if(joy->buttons[9]==1 && joy->buttons[12]==1) 
    {
      ROS_INFO("Pressed triangle");
      ros::Duration(1.0).sleep();
      flag=4;
      laser_sub_= nh_.subscribe<sensor_msgs::LaserScan>("/scan",1,&Tele_current::lasercall, this);
    }
  if (joy->buttons[9]==1 && joy->buttons[13]==1) 
    {
      ROS_INFO("Pressed circle");	
      ros::Duration(1.0).sleep();	
      flag = 5;
      laser_sub_= nh_.subscribe<sensor_msgs::LaserScan>("/scan",1,&Tele_current::lasercall, this);
    }
  if (joy->buttons[9]==1 && joy->buttons[14]==1) 
    {
      ROS_INFO("Pressed cross");    	
      ros::Duration(1.0).sleep();	
      flag = 6;
      laser_sub_= nh_.subscribe<sensor_msgs::LaserScan>("/scan",1,&Tele_current::lasercall, this);
    }
  if(joy->buttons[9]==1 && joy->buttons[15]==1)
    {
      ROS_INFO("Pressed square");
      ros::Duration(1.0).sleep();
      flag =7;
      laser_sub_= nh_.subscribe<sensor_msgs::LaserScan>("/scan",1,&Tele_current::lasercall, this);
    }
     if(joy->buttons[4]==1 && joy->buttons[10]==1) 
     {
       flag=10;
       laser_sub_= nh_.subscribe<sensor_msgs::LaserScan>("/scan",1,&Tele_current::lasercall, this); 
      }
     if(joy->buttons[6]==1 && joy->buttons[10]==1)
     {
       flag=11;
       laser_sub_= nh_.subscribe<sensor_msgs::LaserScan>("/scan",1,&Tele_current::lasercall, this);
     }
     if ((joy->axes[1] !=0.05) && joy->buttons[10]==1)
	     {
         laser_sub_= nh_.subscribe<sensor_msgs::LaserScan>("/scan",1,&Tele_current::lasercall, this);
         vel= drive_f(0.35*joy->axes[1],vel);
	       ROS_INFO("You are moving with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
         stop=1;
         vel_pub_.publish(vel);
       }
      if (joy->axes[2] != 0.05 && joy->buttons[10]==1)
	     {
         laser_sub_= nh_.subscribe<sensor_msgs::LaserScan>("/scan",1,&Tele_current::lasercall, this);
         vel = drive_lr(0.35*joy->axes[2],vel);
	       ROS_INFO("You are turning with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
         stop=1;
         vel_pub_.publish(vel);
	     }
      if(joy->axes[10]==0 && stop==1 )
      {
         flag=1;
         stop=0;      
         vel.linear.x=0;
         vel.angular.z=0;
         ROS_INFO("You have stopped with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
         vel_pub_.publish(vel);        
      }
    if((joy->buttons[8] == 1) && (joy->buttons[9] == 1)) // master kill swithc brings robot to complete halt
	     {  
         flag=1;      
	       vel=driver(0.0,0.0);
         ROS_INFO("You have stopped with Linear: %f, Angular: %f", vel.linear.x, vel.angular.z);
         vel_pub_.publish(vel);
        } 
}  
void Tele_current::buttonPress()
{
//  perf_dist = functioncall(chosenfunction,double(u));
  randomized = random_prob();
  string tf_str[4]= {"/box_triangle","/box_circle","/box_cross","/box_square"};
  ROS_INFO("Value of Performance with respect to distance: %f and value of compare: %f",randomized,perf_dist);
  if(perf_dist<randomized)
    {
      //ROS_INFO("I am going to give you the wrong answerr!");	  	
      //ROS_INFO(" ");
      srand (time(NULL));
      if(flag==4)
	   {
      broadcaster.selectPrimaryTarget(tf_str[rand()%3 + 1]);
      broadcaster.selectFallbackTarget("/person_face");
     //   lookAtTarget(tf_str[rand()%3 + 1]);
	    ROS_INFO("Not looking at Triangle!");
	    ROS_INFO(" ");
	    }
      if(flag==5)
	   {
      broadcaster.selectPrimaryTarget(tf_str[rand()%2 + 2]);
      broadcaster.selectFallbackTarget("/person_face");
     // lookAtTarget(tf_str[rand()%2 + 2]);
	    ROS_INFO("Not looking at Circle!");
	    ROS_INFO(" ");
	   }
      if(flag==6)
	   {
      broadcaster.selectPrimaryTarget(tf_str[rand()%2]);
      broadcaster.selectFallbackTarget("/person_face");
     // lookAtTarget(tf_str[rand()%2]);
	    ROS_INFO("Not looking at Cross!");
	    ROS_INFO(" ");
	   }
      if(flag==7)
	   {
      broadcaster.selectPrimaryTarget(tf_str[rand()%3]);
      broadcaster.selectFallbackTarget("/person_face");
	   // lookAtTarget(tf_str[rand()%3]); 
      ROS_INFO("Not looking at Square!");
	    ROS_INFO(" ");
	   }
      system("mplayer /home/robotlab/catkin_ws/src/Layout_Trial/src/BUZZER.mp3");
      flag = 1;
    }
  else
    {
      //ROS_INFO("I WILL LOOK AT THE CORRECT PLACE!");	  	
      //ROS_INFO(" ");
      //ros::Duration(2.0).sleep();
      if(flag==4)
	     {
        broadcaster.selectPrimaryTarget(tf_str[0]);
        broadcaster.selectFallbackTarget("/person_face");
//         lookAtTarget("/box_triangle");
	       ROS_INFO("Looking at Triangle!");
	       ROS_INFO(" ");
	     }
      if(flag==5)
	     {
        broadcaster.selectPrimaryTarget(tf_str[1]);
        broadcaster.selectFallbackTarget("/person_face");
        // lookAtTarget("/box_circle");
	       ROS_INFO("Looking at Circle!");
	       ROS_INFO(" ");
	     }
      if(flag==6)
	     {
        broadcaster.selectPrimaryTarget(tf_str[2]);
        broadcaster.selectFallbackTarget("/person_face");
//         lookAtTarget("/box_cross");
	       ROS_INFO("Looking at Cross!");
	       ROS_INFO(" ");
	     }
      if(flag==7)
	     {
        broadcaster.selectPrimaryTarget(tf_str[3]);
        broadcaster.selectFallbackTarget("/person_face");
//         lookAtTarget("/box_square");
	       ROS_INFO("Looking at Square!");
	       ROS_INFO(" ");
	     }
      system("mplayer /home/robotlab/catkin_ws/src/Layout_Trial/src/WINNER.mp3");
      flag = 1;	      	
    } 
}
void Tele_current::increment()
{	
  ste+=1;
  if (ste==2)
    {
      Q=Q+1;
      ste=0;
    }
}
#endif
