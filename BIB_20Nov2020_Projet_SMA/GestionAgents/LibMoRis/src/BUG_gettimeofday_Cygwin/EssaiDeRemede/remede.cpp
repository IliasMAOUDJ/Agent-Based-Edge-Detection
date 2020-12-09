             ////////////////////////////////////////////////
             // Mesure du temps sans utiliser gettimeofday //
             ////////////////////////////////////////////////

#include <iostream>

///////////////////////////////////////////////////////////////////////
////////// Recuperation de la frequence du processseur ////////////////
///////////////////////////////////////////////////////////////////////
// cat /proc/cpuinfo | grep cpu MHz
//http://haypo.developpez.com/article/frequence_cpu/#frequence_windows

#include <string>
#include <sstream>
#include <fstream>

using namespace std;

static double cpuFrequency = -1.0;

#define NOMFICH_CPUINFO "/proc/cpuinfo"

// Lit la fr�quence du processeur
// Renvoie 0 en cas d'�chec
static int LitFrequenceCpu (double* frequence)
{
 string prefixe_cpu_mhz = "cpu MHz";
 char ligne[300+1];
 int ok=0;

 ifstream cpuinfo(NOMFICH_CPUINFO);

 if (cpuinfo.fail()) {
                       cerr << "Erreur ouverture "<< NOMFICH_CPUINFO << endl;
                       return 0;
 }

 while (!cpuinfo.eof())
 {
  cpuinfo.getline(ligne,300);

  if (!prefixe_cpu_mhz.compare(0,prefixe_cpu_mhz.length(),
                               ligne,
                               0,prefixe_cpu_mhz.length()))
  {
   // Oui, alors lit la frequence

   string theLigne = ligne;
   size_t pos      = theLigne.rfind(':') + 2;     if (!pos) break;
   string f        = theLigne.substr(pos) + "e6";
   istringstream stringToDouble(f);

   stringToDouble >> *frequence;

   ok = 1;
   break;
  }
 }
 cpuinfo.close();

 return ok;
}

double getCpuFrequency(void)
{
 if (cpuFrequency == -1)
 {
  if (!LitFrequenceCpu(&cpuFrequency))
  {
   cerr << "Can't read CPU Frequency !" << endl;
   cerr << "Stop..."                    << endl; 
   exit(1);
  }
 }

 return cpuFrequency;
}

// Affichage d'une frequence en utilisant le suffixe adapte (GHz, MHz, KHz, Hz)
void printFrequency (double cpuFrequency)
{
  if (1e9<cpuFrequency)
    printf ("%.1f GHz\n", cpuFrequency/1e9);
  else if (1e6<cpuFrequency)
    printf ("%.1f MHz\n", cpuFrequency/1e6);
  else if (1e3<cpuFrequency)
    printf ("%.1f KHz\n", cpuFrequency/1e3);
  else
    printf ("%.1f Hz\n", cpuFrequency);
}

///////////////////////////////////////////////////////////////////////
/////////// Recuperation du nombre de ticks d'horloge  ////////////////
///////////////////////////////////////////////////////////////////////

// which gives you a number of CPU clock ticks (on current platforms,
// typically derived from the buss clock and nominal CPU clock speed ratio).
// http://aips2.nrao.edu/code/casa/implement/OS/PrecTimer.h
double rdtsc(void)  // Pour obtenir le nombre de tic d'horloge
{
   unsigned long long a;
#if 0                                                     // Ca ???
   asm volatile("rdtsc":"=A" (a));       
#else                                                     // Ou bien ca ???
  __asm__ volatile  (".byte 0x0f, 0x31" : "=A"(a));
#endif
   return (double)a;
}

double timeSeconds(double cpuFrequency)                  // En secondes
{
 return rdtsc()/cpuFrequency;
}


///////////////////////////////////////////////////////////////////////
//////////////////////// Un programme de test /////////////////////////
///////////////////////////////////////////////////////////////////////

#include "MAS.h"

int main(void)
{
 Scheduler s;

 double f = getCpuFrequency();

 cout << "frequence : " << f << endl;
 printFrequency(f);

//////////////
 cout << endl;
 cout << "Test en utilisant une nouvelle version de timeSeconds" << endl;
 cout << endl;

 double d1,d2,d3;

 d1 = timeSeconds(f);

cout << "On fait quelque chose" << endl;

 d2 = timeSeconds(f);

 d3 = d2-d1;

 printf("%f %f %f\n",d1,d2,d3);

//////////////
 cout << endl;
 cout << "Test en utilisant directement rdtsc" << endl;
 cout << endl;

 double duree;
 double l1, l2; 

 l1 = rdtsc();

cout << "On fait quelque chose" << endl;

 l2 = rdtsc();

 duree = (l2 - l1) / f;

 cout << "l2-l1: " << l2 - l1 << " " << "duree (l2-l1)/cpuF: " << duree << endl;

 printf("duree : %f\n",duree);

 return 0;
}
