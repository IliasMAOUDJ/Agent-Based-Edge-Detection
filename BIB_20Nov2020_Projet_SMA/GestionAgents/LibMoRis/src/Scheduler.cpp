#include <stdlib.h>
#include "Scheduler.h"

Scheduler*  Scheduler::_theSched=NULL;

NameService Scheduler::_ns;

vector<Scheduler*> Scheduler::_allSchedulers;

//--
void getAllAgents(string aClass, vector<Agent*>& anAgentVector)
{
 Scheduler::getAllAgents(aClass,anAgentVector);
}

Agent* getAgent(string aName)
{
 return Scheduler::getAgent(aName);
}

bool   exist(const Agent* anAgent)
{
 return Scheduler::exist(anAgent);
}
//--


//--
Scheduler::Scheduler(void)
{
 if(_theSched==NULL) { setCurrentSched(this); // _theSched=this;
                       initRandom();
 }
 else 
 {
#ifndef SMAdeSMA
   cerr << ">-----------------------------------------------------" << endl;
   cerr << "Warning: Scheduler::Scheduler(void),\n"
        << "there is already 1 Scheduler which is the current one!" << endl;
   cerr << "<-----------------------------------------------------" << endl;
#endif
 }

 _allSchedulers.push_back(this);

 setRandomMode(false);
 setRealTimeMode(false);
}

//--
Scheduler::Scheduler(const Scheduler& aScheduler)
{
 (void)aScheduler; // Pour eviter un warning
#ifdef SMAdeSMA
 _copy(aScheduler);
#else
 cerr << ">--------------------------------------------------------" << endl;
 cerr << "Error: Scheduler::Scheduler(const Scheduler& aScheduler),\n"
      << "is not implemented!"                                       << endl;
 cerr << "Stop..."                                                   << endl;
 cerr << "<--------------------------------------------------------" << endl;
 exit(1);
#endif
}

//--
Scheduler& Scheduler::operator=(const Scheduler& aScheduler)
{
 (void)aScheduler; // Pour eviter un warning
#ifdef SMAdeSMA
 if (this != &aScheduler)
 {
  _destroy();
  _copy(aScheduler);
 }
#else
 cerr << ">----------------------------------------------------------" << endl;
 cerr << "Error: Scheduler& Scheduler::"
      <<                   "operator=(const Scheduler& aScheduler),\n"
      << "is not implemented!"                                         << endl;
 cerr << "Stop..."                                                     << endl;
 cerr << "<----------------------------------------------------------" << endl;
 exit(1);
#endif
 return *this;
}

//--
Scheduler::~Scheduler(void)
{
 _destroy();
}

//--
Scheduler* Scheduler::getCurrentSched(void)
{
 return _theSched;
}

//--
void Scheduler::setCurrentSched(Scheduler& aScheduler)
{
 setCurrentSched(&aScheduler);
}

//--
void Scheduler::setCurrentSched(Scheduler* aSchedulerPtr)
{
 _theSched=aSchedulerPtr;
}

void Scheduler::justAdded_To_allAgents(void)
{
 // Un agent en cours d'ajout doit etre place dans _allAgents et
 // on doit enregistrer son nom (dans le NameService)
 // ============================================================

 set<Agent*>::iterator iter;

 for(iter = _justAdded.begin(); iter != _justAdded.end(); iter++)
 {
  // Ajout dans _allAgents

   Agent* anAgent = *iter;

  _allAgents.insert(anAgent);

  // On peut savoir si sur dans un constructeur on a fait newAgent
  // il faut comparer getClass() avec getClassName()

   if (anAgent->getClass() != anAgent->getClassName())
   {
    static set<string> dejaVu;

    set<string>::iterator itDejaVu = dejaVu.find(anAgent->getClassName());

    if (itDejaVu==dejaVu.end()) {
       dejaVu.insert(anAgent->getClassName());
       cerr << ">----------------------------------------------------" << endl;
       cerr << "Warning: there is no newAgent() call in class "
            <<  anAgent->getClassName() << " constructor !"            << endl;
       cerr << "<----------------------------------------------------" << endl;
    }
   }

   recordInstanceName(anAgent); // Enregistrement du nom 
 }

 _justAdded.clear();
}

//--
void Scheduler::cycle(void)
{

 justAdded_To_allAgents();

 vector<Agent*>& currentCycle = _computeCurrentCycle();

 // Activation ... en sequence ou random !
 // ======================================

 for(size_t i = currentCycle.size() ; i > 0; i--)
 {
  Agent *ag = currentCycle[i-1];

  currentCycle[i-1] = NULL; 

  if (ag && !ag->_isSuspended) { 
            ag->_posInCurrentCycle = -1;

            double dt          = 0;
            double currentTime = 0;

            if (_realTimeMode)
            {
             currentTime = getTimeSeconds();
             if (ag->_lastActivation != 0)
             {
              dt = currentTime - ag->_lastActivation;
             }
            }

            ag->_lastActivation = currentTime;

            if (ag->_liveMethod) (ag->*(ag->_liveMethod))(dt);
            else ag->live(dt);
  }
 }
}

//--
void Scheduler::setRandomMode(bool randomMode)
{
 _randomMode = randomMode;
}

//--
bool Scheduler::getRandomMode(void) const
{
 return _randomMode;
}

//--
void Scheduler::setRealTimeMode(bool realTimeMode)
{
 _realTimeMode = realTimeMode;
}

//--
bool Scheduler::getRealTimeMode(void) const
{
 return _realTimeMode;
}

//--
ostream& operator<<(ostream& os, const Scheduler& aScheduler)
{
 os << "** Deb *** operator<< Scheduler **********" << endl;

 os << "nbScheds=" << Scheduler::_allSchedulers.size() << endl;
 
 os << Scheduler::_ns << endl;

 os << "Liste des agents en cours d'ajout...." << endl;
 set<Agent*>::iterator iter;

 for(iter  = aScheduler._justAdded.begin();
     iter != aScheduler._justAdded.end();
     iter++)
 {
  // Ajout dans _allAgents
  Agent* anAgent = *iter;
  os << anAgent->getName() << endl;
 }
 os << "Il y en a " << aScheduler._justAdded.size() << endl;
 os << "Fin Liste des agents en cours d'ajout...." << endl;

 os << "Liste des agents en cours...." << endl;

 // set<Agent*>::iterator iter;

 for(iter  = aScheduler._allAgents.begin();
     iter != aScheduler._allAgents.end();
     iter++)
 {
  os << (*iter)->getName() << endl;
 }

 os << "Fin Liste des agents en cours...." << endl;

 os << "** Fin *** operator<< Scheduler **********" << endl;

 return os;
}

//--
void Scheduler::decNextInstanceNumber(string baseClass) 
{                           // A faire avant getNextInstanceNumber !!!
 return _ns.decNextInstanceNumber(baseClass);
}

//--
unsigned long Scheduler::getNextInstanceNumber(string agentsClass) 
{                           // A faire apres decNextInstanceNumber !!!
 return _ns.getNextInstanceNumber(agentsClass);
}

//--
void Scheduler::addNewInstance(Agent *anAgent,string agentsClass)
{
 if (anAgent==NULL)
 {
  cerr << ">---------------------------------------------------------" << endl;
  cerr << "Error: void Scheduler::"
                "addNewInstance(Agent *anAgent,string agentsClass),\n"
       << "anAgent parameter is NULL"                                  << endl;
  cerr << "Stop..."                                                    << endl;
  cerr << "<---------------------------------------------------------" << endl;
  exit(1);
 }

 // Initialisation des donnees (liees a l'ordonnanceur)
 // Donnees TRES TRES PRIVEES de L'Agent
 // ===================================================

  anAgent->_lastActivation    =  0;   // Agent jamais encore active

                                      // Au depart l'agent n'est
  anAgent->_posInCurrentCycle = -1;   // pas en cours de cycle !

  anAgent->_theAgentsSched    = this; // Normalement, c'est deja le cas

 // Enregistrement dans le NameService
 // ==================================

 _ns.addNewInstance(anAgent,agentsClass);

 // Enregistrement dans _justAdded
 // ==============================

 _justAdded.insert(anAgent);
}

void Scheduler::recordInstanceName(Agent *anAgent)
{
 if (anAgent==NULL)
 {
  cerr << ">--------------------------------------------------------"  << endl;
  cerr << "Error: void Scheduler::recordInstanceName(Agent *anAgent),\n"
       << "anAgent parameter is NULL"                                  << endl;
  cerr << "Stop..."                                                    << endl;
  cerr << "<--------------------------------------------------------"  << endl;
  exit(1);
 }

 _ns.recordInstanceName(anAgent);
}

//--
void Scheduler::removeInstance(Agent *anAgent,vector<string>& agentsClasses)
{
 if (anAgent==NULL)
 {
  cerr << ">---------------------------------------------------------" << endl;
  cerr << "Warning: void Scheduler::"
       << "removeInstance(Agent *anAgent,vector<string>& agentsClasses),\n"
       << "anAgent parameter is NULL !"                                << endl;
  cerr << "<---------------------------------------------------------" << endl;
  return;
 }

 // On enleve l'Agent des agents en cours. En gros _allAgents.erase(anAgent); 
 // =========================================================================

 set<Agent*>::iterator it = _allAgents.find(anAgent);
 if (it!=_allAgents.end()) { _allAgents.erase(it); // Agent trouve
 }                                                 // ============
 else { // Agent non trouve, on enleve l'Agent de _justAdded
        // =================================================

        it = _justAdded.find(anAgent);
        if (it!=_justAdded.end()) { _justAdded.erase(it); // Agent trouve
        }                                                 // ============
 }

 // Et on enleve l'Agent des agents en cours de cycle
 // =================================================

 if (anAgent->_posInCurrentCycle != -1)
      _currentCycle[anAgent->_posInCurrentCycle] = NULL;

 /* Version sans _posInCurrentCycle :
    ================================

   for(size_t i = _currentCycle.size() ; i > 0; i--)
   {
    if (_currentCycle[i-1]==anAgent) { _currentCycle[i-1]=NULL; break; }
   }

 */

 // Et on le supprime au niveau du NameService
 // ==========================================

 _ns.removeInstance(anAgent,agentsClasses);
}

//--
void Scheduler::getAllAgents(string aClass, vector<Agent*>& anAgentVector)
{
 _ns.getAllAgents(aClass,anAgentVector);
}

const set<Agent*>& Scheduler::getAllAgents(string aClass) // set... pas vector!
{
 return _ns.getAllAgents(aClass);
}

//--
Agent* Scheduler::getAgent(string aName)
{
 Agent* theAgent = _ns.getAgent(aName);

 if (theAgent != NULL) return theAgent;


 // Le nom de l'Agent n'a pas ete trouve 
 // => l'Agent n'est pas encore enregistre
 // => On le cherche dans les Agents en cours d'ajout

 vector<Scheduler*>::iterator iterSched;
 for(iterSched  = _allSchedulers.begin();
     iterSched != _allSchedulers.end();
     iterSched++)
 {
  set<Agent*>::iterator iterSet;

  for(iterSet  = (*iterSched)->_justAdded.begin();
      iterSet != (*iterSched)->_justAdded.end();
      iterSet++)
  {
   Agent* anAgent = *iterSet;
   if (anAgent->getName() == aName) return anAgent;
  }
 }

 return NULL;
}

//--
bool   Scheduler::exist(const Agent* anAgent)
{
 if (anAgent==NULL) return false;

 Agent* theAgent=(Agent*)anAgent;

 vector<Scheduler*>::iterator iterSched;
 for(iterSched  = _allSchedulers.begin();
     iterSched != _allSchedulers.end();
     iterSched++)
 {

  // 1) On cherche si l'agent est en cours d'execution dans _allSchedulers[i]

  if ((*iterSched)->_allAgents.find(theAgent)!= (*iterSched)->_allAgents.end())
  {
   return true; // Trouve !
  }

  // 2) Puis, on cherche si l'agent est en cours d'ajout dans _allSchedulers[i]

  if ((*iterSched)->_justAdded.find(theAgent)!= (*iterSched)->_justAdded.end())
  {
   return true; // Trouve !
  }

 }

 return false;
}

//--
vector<Agent*>& Scheduler::_computeCurrentCycle(void)
{
   size_t newSize = _allAgents.size();
   size_t oldSize = _currentCycle.size();

  if (newSize!=oldSize) _currentCycle.assign(newSize,NULL);

  set<Agent*>::reverse_iterator iter;

  if (_randomMode)
  {
    for(iter = _allAgents.rbegin(); iter != _allAgents.rend(); iter++)
    {
      size_t pos = randomMinMax(0,newSize-1);

      // recherche de la prochaine place libre a partir de pos

      size_t i = pos;
      while (_currentCycle[i]!=NULL) { i++; i = i % newSize; }

     (*iter)->_posInCurrentCycle = i;
     _currentCycle[i] = (*iter);
    }
  }
  else
  {
    size_t i=0;
    for(iter = _allAgents.rbegin(); iter != _allAgents.rend(); iter++)
    {
     (*iter)->_posInCurrentCycle = i;
     _currentCycle[i++] = (*iter);
    }
  }

 return _currentCycle;
}

//--
#define DESTRUCTEUR_SCHEDULER_DELETE_AGENT 1
#define DESTRUCTEUR_SCHEDULER_AFFINFO      0
void Scheduler::_destroy(void)
{
#if DESTRUCTEUR_SCHEDULER_DELETE_AGENT

  /////////////////////////////////////////////////////////////////////
  // Ne fonctionne que si tous les agents sont alloues dynamiquement //
  /////////////////////////////////////////////////////////////////////
 
#if DESTRUCTEUR_SCHEDULER_AFFINFO == 1
 cout << "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
 cout << "Destruction d'un Scheduler et" << endl;
 cout << "des agents associes : " << endl;
#endif

 set<Agent*>::iterator iter;

//--
 set<Agent*> copieJustAdded = _justAdded;

 for(iter = copieJustAdded.begin(); iter != copieJustAdded.end(); iter++)
 {
#if DESTRUCTEUR_SCHEDULER_AFFINFO == 1
  cout << "  delete " << (*iter)->getName() << endl;
#endif
  delete (*iter);
 }

  copieJustAdded.clear();
 _justAdded.clear();

//--
 set<Agent*> copieAllAgents = _allAgents;

 for(iter = copieAllAgents.begin(); iter != copieAllAgents.end(); iter++)
 {
#if DESTRUCTEUR_SCHEDULER_AFFINFO == 1
  cout << "  delete " << (*iter)->getName() << endl;
#endif
  delete (*iter);
 }

  copieAllAgents.clear();
 _allAgents.clear();

#if DESTRUCTEUR_SCHEDULER_AFFINFO == 1
 cout << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>" << endl;
#endif
#endif

 // On enleve l'ordonnanceur du vecteur des ordonnanceurs
 // =====================================================

 vector<Scheduler*>::iterator iterSched =  _allSchedulers.begin();

 while (iterSched != _allSchedulers.end() && (*iterSched!=this)) iterSched++;
 if (iterSched!=_allSchedulers.end()) _allSchedulers.erase(iterSched);

 // Au cas ou, si on est en train de detruire le Scheduler courant
 // ==============================================================

 if(getCurrentSched()==this) setCurrentSched(NULL);
}

//--
void Scheduler::_copy(const Scheduler& aScheduler)
{
 setRandomMode(aScheduler.getRandomMode());
 setRealTimeMode(aScheduler.getRealTimeMode());

 Scheduler *oldSched=getCurrentSched();  //-- sauvegarde ancien

 setCurrentSched(this);
 
 set<Agent*>::reverse_iterator iter;

 // Copie des agents se trouvant dans _allAgents
 for(iter=aScheduler._allAgents.rbegin();
     iter!=aScheduler._allAgents.rend();
     iter++)
 {
  (*iter)->virtualCopy();
 }

 // Copie des agents se trouvant dans _justAdded
 for(iter=aScheduler._justAdded.rbegin();
     iter!=aScheduler._justAdded.rend();
     iter++)
 {
  (*iter)->virtualCopy();
 }

 setCurrentSched(oldSched);             //-- restauration ancien
}
