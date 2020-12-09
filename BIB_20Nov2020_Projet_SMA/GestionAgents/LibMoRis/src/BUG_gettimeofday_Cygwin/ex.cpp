#include <iostream>

#include <sys/time.h>  // Pour gettimeofday, struct timeval, ...

#include "MAS.h"

using namespace std;


double timeSeconds(void)                          // En secondes
{
struct timeval tv;
gettimeofday(&tv,(struct timezone *)0);
cout << "getTimeSeconds tv_sec: " << tv.tv_sec
     <<              " tv_usec: " << tv.tv_usec << endl;
return(tv.tv_sec+tv.tv_usec/1000000.0);
}


int main(void)
{
 Scheduler s;      // Le bug n'apparait que si un Scheduler est instancie

 double d1,d2,d3;

 d1 = timeSeconds();

cout << "On fait quelque chose" << endl;

 d2 = timeSeconds();

 d3 = d2-d1;

 printf("%f %f %f\n",d1,d2,d3);

 return 0;
}
