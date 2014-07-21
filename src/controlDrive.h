#include <iostream>
#include <stdlib.h>
#include <array>
#include <math.h>

#ifndef CONTROLDRIVE_H_
#define CONTROLDRIVE_H_

void controlDrive()
{
  ////////// You will have to call the laserScan topic and make "this" = "laser" //////////
  int arraySize = sizeof(laser->ranges)/sizeof(laser->ranges[0]);
  float pi = 3.1415;
  int sideLimits = 3;
  float angleArray[arraySize];
  float yDistance[arraySize];
  float leftSideDistance = 0;
  float rightSideDistance = 0;
  int midPoint = arraySize/2;
  int arrayPosition = 0;
  float linearError = REQ_POS - laser-ranges[midPoint];

  float kp_linear_ = 0.6;
  float kp_angular_ = 0.2;

  float incrementAngle = laser->angle_increment;
  float minAngle = laser->angle_min;
  float maxAngle = laser->angle_max;
  
  for(arrayPosition = 1; arrayPosition < arraysize ; arrayPosition++)
    {
      angleArray[arrayPosition] = (minAngle + (arrayPosition*incrementAngle))*(pi/180);
      yDistance[arrarPosition] = cos(angleArray[arrayPosition]);
      if(yDistance[arrayPosition] >= sideLimits)
	{
	  yDistance[arrayPosition] = sidelimits;
	}
      while(arrayPosition <= midPoint)
	{
	  if(angleArray[arrayPosition] < angleArray[arrayPosition - 1])
	    rigthSideDistance = angleArray[arrayPosition];
	}
      while(arrayPosition > midPoint)
	{
	  if(angleArray[arrayPosition - 1] > angleArray[arrayPosition])
	    leftSideDistance = angleArray[arrayPosition - 1];
	}
    }
  
  float angularError = leftSideDistance - rightSideDistance;

  if(linearError > 0)
    {
      vel.linear.x = kp_linear_*linearError;
      if(angularError > 0)
	{
	  vel.angular.z = kp_angular_*angularError;
	}
      else if(angularError < 0)
	{
	  vel.angular.z = kp_angular_*angularError;
	}
    }
  else if(linearError < 0)
    {
      vel.linear.x = kp_linear_*linearError;
      if(angularError > 0)
	{
	  vel.angular.z = kp_angular_*angularError;
	}
      else if(angularError < 0)
	{
	  vel.angular.z = kp_angular_*angularError;
	}
    }
  else
    {
      vel.linear.x = 0;
      vel.angular.z = 0;
    }
}

#endif /* controlDrive.h */
