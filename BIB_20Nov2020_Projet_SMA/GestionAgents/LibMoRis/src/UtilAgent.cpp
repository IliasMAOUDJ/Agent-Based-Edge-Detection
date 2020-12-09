///////////////////////////// Gestion du temps //////////////////////////
#include <sys/time.h>

#include <unistd.h>    

//
// Une definition de macro pour definir __Cygwin__ ...
// mais __GYGWIN___ existe deja...
// http://www.vtk.org/Wiki/CMake_Platform_Dependent_Issues 
//
//                                             # Histoire d'avoir __Cygwin__
//                                               # sous cygwin... A cause d'un
// OS=__$(shell basename `uname -o 2>/dev/null`)__ # bug de gettimeofday
//                                             # Voir le fichier UtilAgent.cpp
// UtilAgent.o : $(INC)/UtilMAS.h UtilAgent.cpp
// 	g++ -D$(OS) $(CCFLAGS) -c UtilAgent.cpp -I$(INC)
//


double getTimeMicroSeconds(void)                     // En microsecondes
{
#ifdef __CYGWIN__
usleep(1);                // Histoire de limiter le bug de gettimeofday?
#endif

struct timeval tv;
gettimeofday(&tv,(struct timezone *)0);
return(tv.tv_sec*1000000.0+tv.tv_usec);
}

double getTimeMilliSeconds(void)                     // En millisecondes
{
#ifdef __CYGWIN__
usleep(1);                // Histoire de limiter le bug de gettimeofday?
#endif

struct timeval tv;
gettimeofday(&tv,(struct timezone *)0);
return(tv.tv_sec*1000.0+tv.tv_usec/1000.0);
}

double getTimeSeconds(void)                          // En secondes
{
#ifdef __CYGWIN__
usleep(1);                // Histoire de limiter le bug de gettimeofday?
#endif

struct timeval tv;
gettimeofday(&tv,(struct timezone *)0);
return(tv.tv_sec+tv.tv_usec/1000000.0);
}

///////////////////////////// Generation aleatoire //////////////////////////

#include <time.h>   // Pour time, time_t, ...
#include <stdlib.h> // Pour rand, srand, ...

void initRandom(void)              // Initialisation generateur
{
 static bool pasEncore=true;       // Fait une et une seule fois... 
 if (pasEncore)
 {
  srand(time((time_t *)0));        // Graine (generation de nombres aleatoires)
  pasEncore=false;
 }
}

//--
size_t randomMinMax(size_t min, size_t max)      // Generation dans [min,max]
{
 double interval = 0.0;

 if (max<min) { size_t temp=min; min=max; max=temp; }

 interval = (double)(max-min+1);

 return (min+(size_t)(interval*rand()/(RAND_MAX)));      // [min,max]
}

//--
double random01(void)                            // Generation dans [0.0,1.0]
{
 return ((double)rand()/RAND_MAX);                       // [0.0,1.0]
}
