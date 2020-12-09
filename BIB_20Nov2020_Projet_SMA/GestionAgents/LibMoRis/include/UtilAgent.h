#ifndef _UTILAGENT_H
#define _UTILAGENT_H

///////////////////////////// Gestion du temps ///////////////////////////////

extern double getTimeMicroSeconds(void);               // En microsecondes
extern double getTimeMilliSeconds(void);               // En millisecondes
extern double getTimeSeconds(void);                    // En secondes

///////////////////////////// Generation aleatoire //////////////////////////

extern void   initRandom(void);                    // Initialisation generateur
extern size_t randomMinMax(size_t min, size_t max);// Generation dans [min,max]
extern double random01(void);                      // Generation dans [0.0,1.0]

#endif // _UTILAGENT_H
