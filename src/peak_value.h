#include "Performance.h"
#include <iostream>
   #ifndef PEAK_VALUE_H_
   #define PEAK_VALUE_H_
    double peak_value(int c)
     {
     	double max=0.0,temp=0.0;
     	for(double i=0.25;i<5;i+=0.05)
     	{
     		if(c==0)
     		{
     			temp= calc_performance_uniform[i];
     			if(max<temp)
     			{
     				max=temp;
     			}
     		}
     		if(c==1)
     		{
     			temp= calc_performance_far[i];
     			if(max<temp)
     			{
     				max=temp;
     			}
     		}
     		if(c==2)
     		{
     			temp= calc_performance_mid[i];
     			if(max<temp)
     			{
     				max=temp;
     			}
     		}
     		if(c==3)
     		{
     			temp= calc_performance_near[i];
     			if(max<temp)
     			{
     				max=temp;
     			}
     		}
     	}
     	return max; 	
     }
     #endif