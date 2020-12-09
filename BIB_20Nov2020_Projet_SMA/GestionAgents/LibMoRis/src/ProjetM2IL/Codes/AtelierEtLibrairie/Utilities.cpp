#include "Utilities.h"

#include <math.h>
#include <sys/time.h>
#include <stdlib.h>

// Converting
void cartesianToPolar(double x, double y, double * distanceOut, double * angleOut){
	*distanceOut=sqrt(x*x+y*y);
	*angleOut=(*distanceOut ? atan2(y,x) : 0.0);
}
void polarToCartesian(double distance, double angle, double * xOut, double * yOut){
	*xOut=distance*cos(angle);
	*yOut=distance*sin(angle);
}
double cartToDist(double x,double y){
	return(sqrt(x*x+y*y));
}
double cartToAngle(double x,double y){
	return(x||y ? atan2(y,x) : 0.0);
}
double abs(double a){
   if (a>=0) return a;
   else
		return -a;
}

// Randomizing
int integerRand(int n){
	return(rand()%n);
}
double realRand(void){
	return(rand()/(double)RAND_MAX);
}

// Timing
long getTime(){
	struct timeval tv;
	gettimeofday(&tv,(struct timezone *)0);
	return(tv.tv_sec*1000+tv.tv_usec/1000);
}
