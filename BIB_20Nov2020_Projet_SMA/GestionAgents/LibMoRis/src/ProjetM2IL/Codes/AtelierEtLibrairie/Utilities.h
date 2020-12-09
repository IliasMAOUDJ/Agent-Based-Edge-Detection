#ifndef _Utilities_H
#define _Utilities_H

// States
#define INIT        0
#define CHOICE      1
#define CONFIRM     2
#define TRANSIT     3
#define READY       4
#define BUSY        5
#define END         6
#define EXIT        7

// Timeouts
#define YESTIMEOUT       2000
#define CONFIRMTIMEOUT   2000
#define ACKTIMEOUT       2000
#define READYTIMEOUT     2000
#define BUSYTIMEOUT      2000
#define WORKINGTIME      500

// Converting
void cartesianToPolar(double, double, double *, double *);
void polarToCartesian(double, double, double *, double *);
double cartToDist(double, double);
double cartToAngle(double, double);
double abs(double);

// Randomizing
int integerRand(int);
double realRand();

// Timing
long getTime();

#endif
