#include <iostream>
#include <cstdio>
#include <fstream>
#include <math.h>

float MIN_PERFORMANCE = 0.1;
float MAX_PERFORMANCE = 0.9;
float MIN_DISTANCE = 0.25;
float MAX_DISTANCE = 4.75;

float ReturnD[3] = {0.0,0.0,0.0};
/*
  #define MIN_PERFORMANCE (0.1)
  #define MAX_PERFORMANCE (0.9)
  #define MIN_DISTANCE (0.25)
  #define MAX_DISTANCE (4.75)

double MIN_PERFORMANCE = 0.1;
double MAX_PERFORMANCE = 0.9;
double MIN_DISTANCE = 0.25;
double MAX_DISTANCE = 4.75;
using namespace std;




#ifndef CALC_PERFORMANCE_PEAK_H
#define CALC_PERFORMANCE_PEAK_H
// high performance at specified peak [0:1]
// between MIN_/MAX_Distance
// * NOTE: performance functions for far, near, and
//         midpoint all call this function!
double calc_performance_peak(double distance, double peak)
{
  if (distance < MIN_DISTANCE) distance = MIN_DISTANCE;
  if (distance > MAX_DISTANCE) distance = MAX_DISTANCE;
  double param = (distance - MIN_DISTANCE) / (MAX_DISTANCE - MIN_DISTANCE);
  double performance = 2.0 * (0.5 - fabs(param - peak));
  return MIN_PERFORMANCE + (MAX_PERFORMANCE - MIN_PERFORMANCE) * performance ;  // scale between MIN_/MAX_PERFORMANCE
}
#endif  /* CALC_PERFORMANCE_PEAK_H 

///////////////---------ROSS MEAD's MAIL------------//////////////////
#ifndef CALC_PERFORMANCE_UNIFORM_H_
#define CALC_PERFORMANCE_UNIFORM_H_

// uniform distribution
double calc_performance_uniform(double distance)
{	
  return MAX_PERFORMANCE;
}

#endif /* CALC_PERFORMANCE_UNIFORM_H 

#ifndef CALC_PERFORMANCE_FAR_H
#define CALC_PERFORMANCE_FAR_H
// high performance at far distances
// (peak = 0.75 between MIN_/MAX_DISTANCE)
double calc_performance_far(double distance)
{
  return calc_performance_peak(distance, 0.75);
} 
#endif /* CALC_PERFORMANCE_FAR_H 


#ifndef CALC_PERFORMANCE_NEAR_H
#define CALC_PERFORMANCE_NEAR_H
// high performance at near distances
// (peak = 0.25 between MIN_/MAX_DISTANCE)
double calc_performance_near(double distance)
{
  return calc_performance_peak(distance, 0.25);
}
#endif /*CALC_PERFORMANCE_NEAR_H


#ifndef CALC_PERFORMANCE_MID_H
#define CALC_PERFORMANCE_MID_H
// high performance at midpoint distances
// (peak = 0.5 between MIN_/MAX_DISTANCE)
double calc_performance_mid(double distance)
{
  return calc_performance_peak(distance, 0.5);
}
#endif /* CALC_PERFORMANCE_MID_H */


////---------------/////////////
//extern float compare=1.0;

#ifndef FUNCTIONCALL_H_
#define FUNCTIONCALL_H_

// typedef double (*fptr)();
double functioncall(int random, double distance)
{
  if(random==0)
    {
      return calc_performance_uniform(distance);
    }
  if(random==1)
    {
      return calc_performance_far(distance);
    }
  if(random==2)
    {
      return calc_performance_mid(distance);
    }
  if(random==3)
    {
      return calc_performance_near(distance);
    }
  
}
#endif

#ifndef PEAK_VALUE_H_
#define PEAK_VALUE_H_
double peak_value(int c)
{
  float max=0.0,temp=0.0;
  for(float i=0.25;i<=4.75;i+=0.05)
  {
    if(c==0)
	  {
	  temp= calc_performance_uniform(i);
	  if(max<temp)
	    {
	      max=temp;
	    }
	  }
    if(c==1)
	  {
	  temp= calc_performance_far(i);
	  if(max<temp)
	    {
	      max=temp;
	    }
	  }
    if(c==2)
	  {
	  temp= calc_performance_mid(i);
	  if(max<temp)
	    {
	      max=temp;
	    }
	  }
    if(c==3)
	  {
	  temp= calc_performance_near(i);
	  if(max<temp)
	    {
	      max=temp;
	    }
	  }
  }
  return max; 	
}
#endif


#ifndef FILE_HANDLING_H_
#define FILE_HANDLING_H_

void file_handling (float distance_written, int chosenOne)
{
  ofstream in;
  double performance_written;
  in.open("/home/robotlab/File Read/write.csv", ios::ate | ios::app);
  if(chosenOne==0) 
    {
  performance_written = calc_performance_uniform(double(distance_written));
    }
  if(chosenOne==1)
    {
  performance_written = calc_performance_far(double(distance_written));
    }
  if(chosenOne==2)
    {
  performance_written = calc_performance_mid(double(distance_written));
    }
  if(chosenOne==3)
    {
  performance_written = calc_performance_near(double(distance_written));
    }
  
  if (in.is_open()) 
    {
      in << distance_written<<","<<performance_written<< " " ;
      in.close();
    } 
  else 
    {
    cout << "\nUnable to open file!";
  }
}      
#endif

//// Gaussian Trial ///////////////////


#ifndef CALC_PERFORMANCE_UNIFORM_H_
#define CALC_PERFORMANCE_UNIFORM_H_
float calc_performance_uniform(float distance)
{
  return MAX_PERFORMANCE;
}
#endif /* CALC_PERFORMANCE_H_  */


#ifndef CALC_PERFORMANCE_NEAR_H_
#define CALC_PERFORMANCE_NEAR_H_
float calc_performance_near(float distance)
{
  float mean = (ReturnD[0]+ReturnD[2])/2;
  float variance = 0.3;
  float pi = 3.141593;
  float e = float(exp);
  float power_num = pow((dist-mean),2);
  float power_den = (-1)*(2*pow(variance,2));
  float nearGauss = (1/(variance*sqrt(2*pi)))*pow(e,(power_num/power_den));
  return nearGauss;
}
#endif /* CALC_PERFORMANCE_NEAR_H_ */


#ifndef CALC_PERFORMANCE_FAR_H_
#define CALC_PERFORMANCE_FAR_H_
float calc_performance_far(float distance)
{
  float mean = (ReturnD[0]+ReturnD[1])/2;
  float variance = 0.3;
  float pi = 3.141593;
  float e = float(exp);
  float power_num = pow((dist-mean),2);
  float power_den = (-1)*(2*pow(variance,2));
  float farGauss = (1/(variance*sqrt(2*pi)))*pow(e,(power_num/power_den));
  return farGauss;
}
#endif /* CALC_PERFORMANCE_FAR_H_  */


#ifndef CALC_PERFORMANCE_MID_H_
#define CALC_PERFORMANCE_MID_H_
float calc_performance_mid(float distance)
{
 return (MAX_PERFORMANCE+MIN_PERFORMANCE)/2;
}
#endif /* CALC_PERFORMANCE_MID_H_  */


#ifndef USING_DATA_H_
#define USING_DATA_H_

float usingdata(float D[])
{
  int k=0;
  int sizeD = sizeof(D)/sizeof(double);
  for(int i=0,j=1;j<sizeD;i+=2,j+=2)
    ReturnD[k++] = (D[i]+D[j])/2;
}
#endif