#include <iostream>
#include <math.h>

float MIN_PERFORMANCE = 0.1;
float MAX_PERFORMANCE = 0.9;
float MIN_DISTANCE = 0.25;
float MAX_DISTANCE = 4.75;


#ifndef CALC_PERFORMANCE_UNIFORM_H_
#define CALC_PERFORMANCE_UNIFORM_H_
float calc_performance_uniform(float distance)
{
  return MAX_PERFORMANCE;
}
#endif /* CALC_PERFORMANCE_H_  */


#ifndef CALC_PERFORMANCE_NEAR_H_
#define CALC_PERFORMANCE_NEAR_H_
float calc_performance_near(float distance, float mean, float variance)
{
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
float calc_performance_far(float distance, float mean, float variance)
{
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

#endif /* CALC_PERFORMANCE_MID_H_  */
