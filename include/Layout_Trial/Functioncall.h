#include <iostream>
#include "Performance.h"

//extern float compare=1.0;

#ifndef FUNCTIONCALL_H_
#define FUNCTIONCALL_H_

//typedef double (*fptr());
int uniform=0,farperf=0,midperf=0,nearperf=0;
double (*functioncall()) (int random, double distance)
{
  if(random==0)
    {
      compare = 0.9;
      uniform++;
      return calc_performance_uniform(distance);
    }
  if(random==1)
    {
      compare = 0.75;
      farperf++;
      return calc_performance_far(distance);
    }
  if(random==2)
    {
      compare = 0.5;
      midperf++;
      return calc_performance_mid(distance);
    }
  if(random==3)
    {
      compare = 0.25;
      nearperf++;
      return calc_performance_near(distance);
    }
}
#endif
