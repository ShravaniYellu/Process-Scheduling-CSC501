/* math.c - provides mathematical functions */
#include<stdio.h>
#include<math.h>

#define RAND_MAX 077777

/*pow calculates x power y, using x as base and y as exponent*/
double pow (double x, double y){
	if( y < 0){
		return pow((1/x),-y);
	}

	double ans = 1;
	int i;
	for(i = 0; i < y; i++){
		ans = ans*x;	
	}
	return ans;
}

/*log is calculated using taylor series by n = 20 (number of terms in taylor series)*/
double log (double x){
	double ans = 0;
	int i;
	for(i=1; i<=20; i++){
		ans = ans + (pow(-1,i-1)*pow(x-1,i))/i;

	}
	return ans;
}

/*expdev generates random numbers which follow the exponential distribution*/
double expdev(double lambda){
    double dummy;
    do{
        dummy = (double) rand() / RAND_MAX;
    }while (dummy == 0.0);
    return -log(dummy) / lambda;
}

