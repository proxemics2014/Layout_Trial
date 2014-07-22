#include <iostream>
#include <cstdio>
#include <fstream>
/*
  #define MIN_PERFORMANCE (0.1)
  #define MAX_PERFORMANCE (0.9)
  #define MIN_DISTANCE (0.25)
  #define MAX_DISTANCE (4.75)
*/
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
#endif  /* CALC_PERFORMANCE_PEAK_H */

///////////////---------ROSS MEAD's MAIL------------//////////////////
#ifndef CALC_PERFORMANCE_UNIFORM_H_
#define CALC_PERFORMANCE_UNIFORM_H_

// uniform distribution
double calc_performance_uniform(double distance)
{
  cout<<"Uniform function!";	
  return MAX_PERFORMANCE;
}

#endif /* CALC_PERFORMANCE_UNIFORM_H */

#ifndef CALC_PERFORMANCE_FAR_H
#define CALC_PERFORMANCE_FAR_H
// high performance at far distances
// (peak = 0.75 between MIN_/MAX_DISTANCE)
double calc_performance_far(double distance)
{
  cout<<"Far function!";
  return calc_performance_peak(distance, 0.75);
} 
#endif /* CALC_PERFORMANCE_FAR_H */


#ifndef CALC_PERFORMANCE_NEAR_H
#define CALC_PERFORMANCE_NEAR_H
// high performance at near distances
// (peak = 0.25 between MIN_/MAX_DISTANCE)
double calc_performance_near(double distance)
{
  return calc_performance_peak(distance, 0.25);
}
#endif /*CALC_PERFORMANCE_NEAR_H*/


#ifndef CALC_PERFORMANCE_MID_H
#define CALC_PERFORMANCE_MID_H
// high performance at midpoint distances
// (peak = 0.5 between MIN_/MAX_DISTANCE)
double calc_performance_mid(double distance)
{
  cout<<"Mid function!";
  return calc_performance_peak(distance, 0.5);
}
#endif /* CALC_PERFORMANCE_MID_H */


////---------------/////////////
extern float compare=1.0;

#ifndef FUNCTIONCALL_H_
#define FUNCTIONCALL_H_

// typedef double (*fptr)();
double functioncall(int random, double distance)
{
  int uniform=0,farperf=0,midperf=0,nearperf=0;
  ofstream out;
  out.open("/home/robotlab/catkin_ws/src/Layout_Trial/src/list.txt");
  if(out.is_open())
  {
  if(random==0)
    {
      compare = 0.9;
      cout<<"Uniform function!";
      uniform++;
      return calc_performance_uniform(distance);
    }
  if(random==1)
    {
      compare = 0.75;
      farperf++;
      cout<<"Far function!";
      return calc_performance_far(distance);
    }
  if(random==2)
    {
      compare = 0.5;
      midperf++;
      cout<<"Mid function!";
      return calc_performance_mid(distance);
    }
  if(random==3)
    {
      compare = 0.25;
      nearperf++;
      cout<<"Near function!";
      return calc_performance_near(distance);
    }
  out << uniform << farperf << midperf << nearperf;
  out.close();
  }
}
#endif

#ifndef PEAK_VALUE_H_
#define PEAK_VALUE_H_
double peak_value(int c)
{
  double max=0.0,temp=0.0;
  for(double i=0.25;i<5;i+=0.05)
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
