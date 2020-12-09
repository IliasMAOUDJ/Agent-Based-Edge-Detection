#include <stdlib.h>
#include "NameService.h"

bool NameService::_alreadyOneNameService=false;

//--
NameService::NameService(void)
{
  if (_alreadyOneNameService)
  {
   cerr << ">------------------------------------------------" << endl;
   cerr << "Error: NameService::NameService(void),\n"
        << "there is already 1 instance of NameService class!" << endl;
   cerr << "Stop..."                                           << endl;
   cerr << "<------------------------------------------------" << endl;
   exit(1);
  }

  _alreadyOneNameService=true;
}

//--
NameService::NameService(const NameService& aNS)
{
  if (_alreadyOneNameService)
  {
   cerr << ">--------------------------------------------------------" << endl;
   cerr << "Warning: NameService::NameService(const NameService& aNS)),\n"
        << "there is already 1 instance of NameService class!"         << endl;
   cerr << "<--------------------------------------------------------" << endl;
  }
  else { // Normalement, on arrive jamais ici
         _alreadyOneNameService=true;
  }

 _copy(aNS);
}

//--
NameService& NameService::operator=(const NameService& aNS)
{
 if (this != &aNS)
 {
  _destroy();
  _copy(aNS);
 }

 return *this;
}

//--
NameService::~NameService(void)
{
 _destroy();
}

//--
ostream& operator<<(ostream& os, const NameService& aNS)
{
 os << "*************************************************************" << endl;
 os << "** Deb *** operator<< NameService **********" << endl;

 os << "1) Liste des classes et nextInstanceNumber associes(_numberMap)"<<endl;
 map<string, unsigned long>::const_iterator iterNP;
 for(iterNP = aNS._numberMap.begin(); iterNP != aNS._numberMap.end(); iterNP++)
 {
  os << "********* Classe " <<
         (*iterNP).first << " ("<< (*iterNP).second <<  ") " << endl;
 }

 os << "2) Liste des classes et instances associes(_instanceMap)"<<endl;
 map<string, set<Agent*> >::const_iterator iterIM;
 for(iterIM  = aNS._instancesMap.begin();
     iterIM != aNS._instancesMap.end();
     iterIM++)
 {
  os << "********* Classe " << (*iterIM).first << endl;
  set<Agent*>::const_iterator iterSet;
  for(iterSet  = (*iterIM).second.begin();
      iterSet != (*iterIM).second.end();
      iterSet++)
  {
    os << (*iterSet)->getName() << endl;
  }
 }

 os << "3) Liste des noms et des agents associes (_namesMap)" << endl;

 map<string, map<string, Agent*> >::const_iterator iterNM;

 for(iterNM  = aNS._namesMap.begin();
     iterNM != aNS._namesMap.end();
     iterNM++)
 {
  os << "Classe : " << (*iterNM).first << endl;
  map<string, Agent*>::const_iterator innerIterNM;
  for(innerIterNM  = (*iterNM).second.begin();
      innerIterNM != (*iterNM).second.end();
      innerIterNM++)
  {
   os << (*innerIterNM).first <<" "<< (*innerIterNM).second->getName() << endl;
  }
 }

 os << "** Fin *** operator<< NameService **********" << endl;
 os << "*************************************************************" << endl;

 return os;
}

//--
void NameService::decNextInstanceNumber(string baseClass)
{
  map<string, unsigned long>::iterator iter;

// On s'occupe de dimunuer de 1 la valeur pour la classe de base baseClass.
// C'est necessaire !
// En effet si A herite d'Agent. Quand on veut obtenir A.1, on a deja Agent.1
// Alors que l'on ne cree pas d'Agent...
// ==========================================================================
 if (baseClass != "") { iter = _numberMap.find(baseClass);
                        if (iter!=_numberMap.end()) --(*iter).second;
 }
}

//--
unsigned long NameService::getNextInstanceNumber(string agentsClass)
{
  map<string, unsigned long>::iterator iter;

// On recupere le numero de la prochaine instance
// =================================================

  unsigned long nextInstanceNumber = 1;

  iter=_numberMap.find(agentsClass);                                 // Classe
  if (iter!=_numberMap.end()) nextInstanceNumber = ++(*iter).second; // trouvee
  else _numberMap[agentsClass] = nextInstanceNumber;

 return nextInstanceNumber;
}

//--
unsigned long NameService::getNextInstanceNumber(string baseClass,
                                                 string agentsClass)
{
 decNextInstanceNumber(baseClass);

 return getNextInstanceNumber(agentsClass);
}

//--
void NameService::addNewInstance(Agent *anAgent,string agentsClass)
{
 if (anAgent==NULL)
 {
  cerr << ">---------------------------------------------------------" << endl;
  cerr << "Warning: void NameService::"
       <<       "addNewInstance(Agent *anAgent,string agentsClass),\n"
       << "anAgent parameter is NULL !"                                << endl;
  cerr << "<---------------------------------------------------------" << endl;
  return;
 }

 map<string, set<Agent*> >::iterator iter;
 iter=_instancesMap.find(agentsClass);
 if (iter!=_instancesMap.end()) { // Classe trouvee
                                  (*iter).second.insert(anAgent);
 }
 else
 { set<Agent*> newSet;
   newSet.insert(anAgent);
  _instancesMap[agentsClass]=newSet;
 }
}

//--
void NameService::recordInstanceName(Agent *anAgent)
{
 if (anAgent==NULL)
 {
  cerr << ">---------------------------------------------------------" << endl;
  cerr << "Warning: void NameService::"
       <<                      "recordInstanceName(Agent *anAgent),\n"
       << "anAgent parameter is NULL !"                                << endl;
  cerr << "<---------------------------------------------------------" << endl;
  return;
 }

 string className = anAgent->getClass();
 string      name = anAgent->getName();

 map<string, map<string, Agent*> >::iterator iterNM;

 iterNM = _namesMap.find(className);
 if (iterNM!=_namesMap.end()) { // Classe trouvee
                                (*iterNM).second[name]=anAgent;
 }
 else {
        map<string, Agent*> associationMap;
        associationMap[name] = anAgent;
       _namesMap[className]  = associationMap;
 }
}

//--
void NameService::removeInstance(Agent *anAgent, vector<string>& agentsClasses)
{
 if (anAgent==NULL)
 {
  cerr << ">---------------------------------------------------------" << endl;
  cerr << "Warning: void NameService::"
       << "removeInstance(Agent *anAgent, vector<string>& agentsClasses),\n"
       << "anAgent parameter is NULL !"                                << endl;
  cerr << "<---------------------------------------------------------" << endl;
  return;
 }

 // Il faut enlever l'Agent de _instancesMap
 // ========================================

 map<string, set<Agent*> >::iterator iterIM;

 vector<string>::const_iterator iter;
 for(iter = agentsClasses.begin(); iter != agentsClasses.end(); iter++)
 {                                           // Pour toutes les classes de
                                             // l'agent, on le supprime !
  iterIM = _instancesMap.find(*iter);
  if (iterIM!=_instancesMap.end())  { // Classe trouvee

     (*iterIM).second.erase(anAgent);
  }
//else {
//  cerr << "NameService::removeInstance Cas 1 normalement impossible!" << endl;
//}
 }

 // Et de _namesMap
 // ===============

  string className = anAgent->getClass();
  string      name = anAgent->getName();

  map<string, map<string, Agent*> >::iterator iterNM;

  iterNM = _namesMap.find(className);
  if (iterNM!=_namesMap.end()) { // Classe trouvee
                                (*iterNM).second.erase(name);
  }
//else {
//  cerr << "NameService::removeInstance Cas 2 normalement impossible!" << endl;
//}
}

//--
void NameService::getAllAgents(string aClass,
                               vector<Agent*>& anAgentVector) const
{
 if (aClass =="") getAllAgents("Agent",anAgentVector);

 anAgentVector.clear(); // S'il y avait des choses dans le vecteur

 map<string, set<Agent*> >::const_iterator iterIM;

 iterIM = _instancesMap.find(aClass);
 if (iterIM!=_instancesMap.end())              // Classe trouvee
 { 
  set<Agent*>::const_iterator iterSet;
  for(iterSet  = (*iterIM).second.begin();
      iterSet != (*iterIM).second.end();
      iterSet++)
  {
   anAgentVector.push_back(*iterSet);
  }
 }
//else cerr << "WARNING, getAllAgents: " << aClass << " is unknown" << endl;
}

//--
const set<Agent*>& NameService::getAllAgents(string aClass) const
{
 static set<Agent*> vide;
 if (aClass =="") return getAllAgents("Agent");

 map<string, set<Agent*> >::const_iterator iterIM;
 iterIM = _instancesMap.find(aClass);
 if (iterIM!=_instancesMap.end())              // Classe trouvee
 {
  return (*iterIM).second;
 }
 else return vide;
}

//--
Agent* NameService::getAgent(string aName) const
{
 Agent* anAgent = NULL;

 string className = "faire une recherche du . le plus a droite";

 int posPoint = aName.rfind('.');

 if (posPoint == -1) return NULL;

 className = aName.substr(0,posPoint);

 map<string, map<string, Agent*> >::const_iterator iterNM;

 iterNM = _namesMap.find(className);

 if (iterNM!=_namesMap.end()) { // Classe trouvee

   map<string, Agent*>::const_iterator innerIterNM;
   innerIterNM = (*iterNM).second.find(aName);
   if (innerIterNM!=(*iterNM).second.end()) anAgent = (*innerIterNM).second;
 }

 return anAgent;
}

//--
void NameService::_copy(const NameService& aNS)
{
 // Copie de _numberMap
 // ===================

 _numberMap = aNS._numberMap;

 // Copie de _instancesMap
 // =====================

#if 1

 _instancesMap = aNS._instancesMap;

#else
 map<string, set<Agent*> >::const_iterator iterIM;

 for(iterIM  = aNS._instancesMap.begin();
     iterIM != aNS._instancesMap.end();
     iterIM++)
 {
  //os << "Classe : " << (*iterIM).first << endl;
#if 0
  set<Agent*>::const_iterator innerIterIM;
  for(innerIterIM  = (*iterIM).second.begin();
      innerIterIM != (*iterIM).second.end();
      innerIterIM++)
  {
   string className = (*iterIM).first;
   Agent* anAgent   = (*innerIterIM);
   addNewInstance(anAgent,className);  // Tres simple !
  }
#else
  string className = (*iterIM).first;
  set<Agent*> aNewSet;
 _instancesMap[className] = aNewSet;

  map<string, set<Agent*> >::iterator iterNewSet;
  iterNewSet = _instancesMap.find(className);

  set<Agent*>::const_iterator innerIterIM;
  for(innerIterIM  = (*iterIM).second.begin();
      innerIterIM != (*iterIM).second.end();
      innerIterIM++)
  {
   Agent* anAgent   = (*innerIterIM);
   
   (*iterNewSet).second.insert(anAgent);
  }
#endif
 }
#endif

 // Copie de _namesMap
 // ==================

#if 1

 _namesMap = aNS._namesMap;

#else
 map<string, map<string, Agent*> >::const_iterator iterNM;

 for(iterNM = aNS._namesMap.begin(); iterNM != aNS._namesMap.end(); iterNM++)
 {
  //os << "Classe : " << (*iterNM).first << endl;
#if 0
  map<string, Agent*>::const_iterator innerIterNM;
  for(innerIterNM  = (*iterNM).second.begin();
      innerIterNM != (*iterNM).second.end();
      innerIterNM++)
  {
   Agent*   anAgent = (*innerIterNM).second;
   recordInstanceName(anAgent);     // Tres simple !
  }
#else
  string className = (*iterNM).first;
  map<string, Agent*> aNewMap;
 _namesMap[className] = aNewMap;


  map<string, map<string, Agent*> >::iterator iterNewMap;
  iterNewMap = _namesMap.find(className);


  map<string, Agent*>::const_iterator innerIterNM;
  for(innerIterNM  = (*iterNM).second.begin();
      innerIterNM != (*iterNM).second.end();
      innerIterNM++)
  {
   string agentsName = (*innerIterNM).first;
   Agent*    anAgent = (*innerIterNM).second;

   (*iterNewMap).second[agentsName] = anAgent;
  }
#endif
 }
#endif
}

//--
void NameService::_destroy(void)
{
// Destruction de _numberMap
// =========================

 _numberMap.clear();

// Destruction de _instancesMap
// ===========================

 map<string, set<Agent*> >::iterator iterIM;

 for(iterIM = _instancesMap.begin(); iterIM != _instancesMap.end(); iterIM++)
 {
  (*iterIM).second.clear();       // Destruction de chaque set interne
 }

 _instancesMap.clear();           // Et on detruit _instancesMap

// Destruction de _namesMap
// =======================

 map<string, map<string, Agent*> >::iterator iterNM;

 for(iterNM = _namesMap.begin() ;iterNM != _namesMap.end(); iterNM++)
 {
  (*iterNM).second.clear();      // Destruction de chaque map interne
 }

 _namesMap.clear();              // Et on detruit _namesMap
}

//--
