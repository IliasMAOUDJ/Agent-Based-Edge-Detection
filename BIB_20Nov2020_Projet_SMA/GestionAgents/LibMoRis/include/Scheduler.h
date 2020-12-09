#ifndef _SCHEDULER_H_
#define _SCHEDULER_H_

#define SMAdeSMA   // Pour autoriser plusieurs ordonnanceurs...

#include <iostream>

#include <string>
#include <vector>
#include <set>

#include "MAS.h"
#include "Agent.h"
#include "NameService.h"

using namespace std;

class Scheduler
{
  friend void   getAllAgents(string aClass, vector<Agent*>& anAgentVector);
  friend Agent* getAgent(string aName);
  friend bool   exist(const Agent* anAgent);

  friend ostream& operator<<(ostream& os, const Scheduler& aScheduler);

 public:

   // Allocateurs/Desallocateurs

                     Scheduler(void);
                     Scheduler(const Scheduler& aScheduler); // Progr. avertis!
                     Scheduler& operator=(const Scheduler& aScheduler); // Idem
            virtual ~Scheduler(void);

                void cycle(void);

                void setRandomMode(bool randomMode);
                bool getRandomMode(void) const;

                void setRealTimeMode(bool realTimeMode);
                bool getRealTimeMode(void) const;

#ifdef SMAdeSMA
 public:
#else
 private:
#endif

  //-- Gestion de plusieurs ordonnanceurs...
  //-- Pour programmeurs avertis ... pour faire des SMA de SMA !
  //
  //-- Constructeur par recopie et affectation
  // Implemente !    Scheduler(const Scheduler& aScheduler);
  // Implemente !    Scheduler& operator=(const Scheduler& aScheduler);
  //
  //-- Changement et memorisation de l'ordonnanceur courant........
   static void       setCurrentSched(Scheduler& aScheduler);
   static void       setCurrentSched(Scheduler* aSchedulerPtr);
   static Scheduler* getCurrentSched(void);
  //

 private:                 

  friend class Agent;

  static Scheduler* _theSched; // The current Scheduler

//--
  void          decNextInstanceNumber(string baseClass);
  unsigned long getNextInstanceNumber(string agentsClass);
  void          addNewInstance(Agent *anAgent,string agentsClass);
  void          recordInstanceName(Agent *anAgent);
  void          removeInstance(Agent *anAgent,vector<string>& agentsClasses);

//--
  static Agent* getAgent(string aName);
  static bool   exist(const Agent* anAgent);
  static void   getAllAgents(string aClass, vector<Agent*>& anAgentVector);

 public: // Pour eviter un "friend class Agent2D;"
  static const  set<Agent*>& getAllAgents(string aClass);
             // ---  set... pas vector!

 private:

   // Attributs prives

  bool _randomMode;

  bool _realTimeMode;  // Et les deux methodes associees :

                              // Agents pas encore dans allAgents. 
  set<Agent*> _justAdded;     // A la fin du cycle en cours, ils
                              // seront mis dans allAgents et enregistres
                              // dans le NameService

  set<Agent*>    _allAgents;  // Agents propre a l'ordonnanceur

  void justAdded_To_allAgents(void); // Pour veritablement activer les agents

  vector<Agent*> _currentCycle; // Vecteur courant d'activation

  // Methodes privees
                                              // Genere _currentCycle a partir
  vector<Agent*>& _computeCurrentCycle(void); // de _allAgents.
                                              // Soit sequentiel, soit random

  // Attributs prives statiques

  static NameService _ns;
  static vector<Scheduler*> _allSchedulers;

  // Methodes privees "classiques"

  void _destroy(void);
  void _copy(const Scheduler& aScheduler);
};

#endif // _SCHEDULER_H_
