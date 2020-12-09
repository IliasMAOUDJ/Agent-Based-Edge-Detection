#ifndef _NAMESERVICE_H_
#define _NAMESERVICE_H_

#include <iostream>

#include <string>
#include <vector>
#include <set>
#include <map>

class NameService;

#include "Agent.h"

using namespace std;

class NameService
{
 friend ostream& operator<<(ostream& os, const NameService& aNS);

 private:  // Avec un friend class Scheduler !

   friend class Scheduler;

   // Allocateurs/Desallocateurs

           NameService(void);
           NameService(const NameService& aNS);
           NameService& operator=(const NameService& aNS);
  virtual ~NameService(void);

  void          decNextInstanceNumber(string baseClass);
  unsigned long getNextInstanceNumber(string agentsClass);
  unsigned long getNextInstanceNumber(string baseClass, string agentsClass);
  void          addNewInstance(Agent *anAgent,string agentsClass);
  void          recordInstanceName(Agent *anAgent);
  void          removeInstance(Agent *anAgent,vector<string>& agentsClasses);

   // Inspecteurs

  void   getAllAgents(string aClass, vector<Agent*>& anAgentVector) const;
  const  set<Agent*>& getAllAgents(string aClass) const;
  Agent* getAgent(string aName) const;

 private:

   // Attributs et methodes vraiment prives

        map<string, unsigned long> _numberMap;
        //    ^            ^
        // nom de    (numero-1) de
        // classe    la prochaine
        //            instance

        map<string, set<Agent*> > _instancesMap;

        //    ^            ^
        // nom de    ensemble des Agents
        // classe   de la classe et des
        //           classes derivees 

        map<string, map<string, Agent*> > _namesMap;

        //    ^            ^         ^
        // nom de    nom instance  Agent* (info a retrouver)
        // classe

  static bool _alreadyOneNameService;

   // Methodes privees d'allocation/desallocation

  void _copy(const NameService& aNS);
  void _destroy(void);
};

#endif // _NAMESERVICE_H_
