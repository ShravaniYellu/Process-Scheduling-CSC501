/* math.h - header file for defining mathematical functions */
#ifndef _MATH_H_
#define _MATH_H_

/*log is calculated using taylor series by n = 20 (number of terms in taylor series)*/
double log(double x);

/*pow calculates x power y, using x as base and y as exponent*/
double pow(double x, double y);

/*expdev generates random numbers which follow the exponential distribution*/
double expdev(double lambda);

#endif
