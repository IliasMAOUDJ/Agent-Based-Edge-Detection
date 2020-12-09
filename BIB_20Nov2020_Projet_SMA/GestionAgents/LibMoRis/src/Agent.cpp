#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Agent.h"

#include <sstream>

//--
Agent::Agent(void)
{
 _theAgentsSched = NULL;
 _name           = "";                       // L'agent n'a pas encore de nom
 _baseClass      = "";                       // ni de classe de base
 _class          = "";                       // ni de classe
 _deadAgent      = false;                    // cf void newAgent(Agent* This)

 _nbMessages     = 0;
 _liveMethod     = NULL;
 _isSuspended    = false;

  newAgent();
}

//--
Agent::Agent(const Agent& anAgent)
{
 _theAgentsSched = NULL;
 _name           = "";                       // L'agent n'a pas encore de nom
 _baseClass      = "";                       // ni de classe de base
 _class          = "";                       // ni de classe
 _deadAgent      = false;                    // cf void newAgent(Agent* This)

 _copy(anAgent);       // Recopie de la boite _mailBox + gestion _sensitivity
                       // + _isSuspended + _liveMethod
  newAgent();
}

//--
Agent& Agent::operator=(const Agent& anAgent)
{
 if (this != &anAgent)
 {
  _destroy();        // Destruction de la boite _mailBox + gestion _sensitivity
  _copy(anAgent);    // Recopie de la boite _mailBox + gestion _sensitivity
                     // + _isSuspended + _liveMethod
 }

 return *this;
}

//--
Agent::~Agent(void)
{
 if (_theAgentsSched) _theAgentsSched->removeInstance(this,_agentsClasses);

 _agentsClasses.clear();

 _destroy();        // Destruction de la boite _mailBox + gestion _sensitivity
}

//--
void Agent::newAgent(void)
{
 if (_deadAgent) return;

 _theAgentsSched = Scheduler::getCurrentSched();
 if(_theAgentsSched==NULL)
 {
  cerr << ">---------------------------------"   << endl;
  cerr << "Error: void Agent::newAgent(void),\n"
       << "there is no Scheduler !!"             << endl;
  cerr << "Stop..."                              << endl;
  cerr << "<---------------------------------"   << endl;
  exit(1);
 }

  _baseClass = _class;

  _class   = getClassName();  // Voir DEFCLASS

  if (_baseClass!=_class)
  {                                   // Histoire d'avoir l'arbre d'heritage,
   _agentsClasses.push_back(_class);  // on enregistre la classe

   _theAgentsSched->decNextInstanceNumber(_baseClass);

    unsigned long instanceNumber =
        _theAgentsSched->getNextInstanceNumber(_class);

    setName(instanceNumber);

   _theAgentsSched->addNewInstance(this,_class);
 }
}

//--
void Agent::newAgent(Agent *This)   // This (pour l'heritage multiple)
{
 if (_deadAgent) return;

 if (This == NULL || this == This) newAgent(); // Cas heritage simple
 else { // if (this != This)   // Cas heritage multiple !

 _theAgentsSched = Scheduler::getCurrentSched();
 if(_theAgentsSched==NULL)
 {
  cerr << ">---------------------------------"   << endl;
  cerr << "Error: void Agent::newAgent(void),\n"
       << "there is no Scheduler !!"             << endl;
  cerr << "Stop..."                              << endl;
  cerr << "<---------------------------------"   << endl;
  exit(1);
 }

 _baseClass = _class;

 _class   = getClassName();  // Voir DEFCLASS

  if (_baseClass!=_class) _theAgentsSched->decNextInstanceNumber(_baseClass);

 _theAgentsSched->removeInstance(this,_agentsClasses);

  vector<string>::iterator iter;
  for(iter = _agentsClasses.begin(); iter != _agentsClasses.end(); iter++)
  {
   _theAgentsSched->addNewInstance(This,*iter);
   This->_agentsClasses.push_back(*iter);
  }

  /////////// Ou (pour conserver l'ordre dans This->_agentsClasses) ////
  // {
  //  This->newAgent();
  //
  //          // Mise a jour de This->_agentsClasses
  //
  //  This->_agentsClasses.pop_back(); // il faudra remettre _class
  //
  //  size_t agentsClassesSize=_agentsClasses.size();
  //  for(size_t i=0;i<agentsClassesSize;i++)
  //  {
  //   This->_agentsClasses.push_back(_agentsClasses[i]);
  //  }
  //
  //  This->_agentsClasses.push_back(_class); // on remet _class
  // }
  /////////////////////////////////////////////////////////////////////

  _name      = ""; // _name = This->_name;  // Mise a jour du nom
  _baseClass = ""; // _baseClass = _baseClass; // On laisse comme c'est
  _class     = ""; // _class   = getClassName();  // Voir DEFCLASS

  _agentsClasses.clear(); // _agentsClasses = This->_agentsClasses;

                     // Indique que l'Agent ("mauvaise" branche de l'heritage
  _deadAgent = true; // multiple) n'est plus a considerer lors d'un nouvel
                     // appel a newAgent
 }
}

//--
void Agent::suspend(void)
{
 _isSuspended = true;
}

//--
void Agent::restart(void)
{
 _isSuspended = false;
}

//--
bool Agent::isSuspended(void) const
{
 return _isSuspended;
}

/////////////////////////////////////////////////////////////////////////
// Pour changer de methode appelee par l'ordonnanceur...
// Pour programmeurs avertis, voir exemple :
// Exemples/ExemplesPourProgrammeursAvertis/Exemple_setLiveMethod
// --
// Dans setLiveMethod, si newLiveMethod est NULL alors c'est la methode
// live qui sera appelee par l'ordonnanceur...
// C'est le comportement par defaut..!
//
liveMethodType Agent::getLiveMethod(void)
{
 return _liveMethod;
}

void Agent::setLiveMethod(liveMethodType newLiveMethod)
{
 _liveMethod = newLiveMethod;
}
//
/////////////////////////////////////////////////////////////////////////

//--
bool operator==(const Agent& anAgent1, const Agent& anAgent2)
{
 return anAgent1.isEqualTo(anAgent2);
}

//--
bool operator!=(const Agent& anAgent1, const Agent& anAgent2)
{
 return !(anAgent1==anAgent2);
}

//--
string Agent::getClass(void) const
{
 return _class;
}

//--
bool Agent::isA(string aClass) const
{
            // Rappel: faire un find si autre chose que vector (set,...)

 vector<string>::const_iterator iter;
 for(iter = _agentsClasses.begin(); iter != _agentsClasses.end(); iter++)
 {
  if (*iter==aClass) return true;
 }

 return false;
}

//--
string Agent::getName(void) const
{
 return _name;
}

//--
void Agent::setName(unsigned long instanceNumber)
{
  ostringstream unsignedlongToString; unsignedlongToString << instanceNumber;
 _name   = getClass()+"."+unsignedlongToString.str();
}

//--
unsigned long Agent::getSuffix(void) const
{
 string theName = getName();
 const char *name   = theName.c_str();
 const char *suffix = strrchr(name,'.');
 if (suffix==NULL) return 0;
 unsigned long instanceNumber=0;
 sscanf(suffix+1,"%lu",&instanceNumber); // +1 pour passer le .
 return instanceNumber;
}

//--
ostream& operator<<(ostream& os,const Agent& anAgent)
{
 anAgent.display(os);
 return os;
}

void Agent::display(ostream& os) const
{
 if (!_deadAgent)
 {
  os << _name << " : " << getClass() << endl;
  os << "Nombre de messages en attente :" << getNbMessages() << endl;

  vector<string>::const_iterator iter;

  if (_agentsClasses.size()!=0)
  {
   os << _name << " est un : ";

   for(iter = _agentsClasses.begin(); iter!= _agentsClasses.end(); iter++)
   {
    if (iter!=_agentsClasses.begin())
    {
     for(size_t n=0; n<_name.length();n++) os << ' ';
     os << "          ";
    }
    os << *iter << endl;
   }
  }
 }
}

//--
bool Agent::isEqualTo(const Agent& anAgent) const
{
 (void)anAgent;  // Pour eviter un warning

#if 1            // A voir ..! , actuellement deux agents sont tjs egaux
 return true;                              // Deux agents sont tjs egaux !!!!!
#else                                      //            OU
 if (_name != anAgent._name) return false; // resultat de _name==anAgent._name
 if (_liveMethod != anAgent._liveMethod) return false;
 return true;
#endif
}

/////////////////////////////////////////////////////////////////////////////
///        Gestion mailBox
/////////////////////////////////////////////////////////////////////////////

//***********************************************
// map<string,size_t> _nbMessagesMap; // Pour chaque classe, le nb de messages
// void   _addMessage(Message* message);    // trois methodes pour connaitre le
// void   _removeMessage(Message* message); // nb de messages d'un type
// size_t _nbMessages(string aClass) const; // particulier presents ds _mailBox

void Agent::_addMessage(Message* message)
{
 if (message==NULL) return;

 vector<string>& v = message->_messagesClasses;

 vector<string>::iterator iterVect;
 for(iterVect = v.begin(); iterVect != v.end(); iterVect++)
 {
  string aClass = *iterVect;

  map<string,size_t>::iterator iter=_nbMessagesMap.find(aClass);
  if (iter!=_nbMessagesMap.end()) ++(*iter).second;  // ++ si deja present
  else _nbMessagesMap[aClass]=1;                     //  1 si 1ere fois
 }
}

void Agent::_removeMessage(Message* message)
{
 if (message==NULL) return;

 vector<string>& v = message->_messagesClasses;

 vector<string>::iterator iterVect;
 for(iterVect = v.begin(); iterVect != v.end(); iterVect++)
 {
  string aClass = *iterVect;

  map<string,size_t>::iterator iter=_nbMessagesMap.find(aClass);
  if (iter!=_nbMessagesMap.end()) --(*iter).second;  // -- si deja present
  else {                                            // si absent : warning!!
         cerr << "Warning Agent::removeMessage" << endl;
         _nbMessagesMap[aClass]=0;
  }
 }
}

size_t Agent::_hmMessages(string aClass) const  // how many messages
{
 map<string,size_t>::const_iterator iter=_nbMessagesMap.find(aClass);
 if (iter!=_nbMessagesMap.end()) return (*iter).second;
 else return 0;
}
//***********************************************

#define USE_nbMessagesMap 1

//--
void Agent::putInMailBox(Message* message)   // Private !... utilise dans la
{                                            // classe amie Message
 if (message==NULL) return;

 bool alreadyInMailBox=false;

 // On regarde les differents cas pour voir si on ne peut pas simplement
 // ajouter a la fin

 // Sans priorite c'est simplement:
 //  _mailBox.push_back(message);
 //  _nbMessages++;
 // #if USE_nbMessagesMap
 //  _addMessage(message);
 // #endif

 if (_mailBox.empty()) { _mailBox.push_back(message); alreadyInMailBox=true; }

 if (!alreadyInMailBox)
 {
  list<Message*>::reverse_iterator it=_mailBox.rbegin();
  if (message->_priority <= (*it)->_priority) { _mailBox.push_back(message);
                                                alreadyInMailBox=true;
  }
 }

 if (!alreadyInMailBox)
 {
  list<Message*>::iterator it;
  for(it=_mailBox.begin();it!=_mailBox.end();it++)
  {
   if (message->_priority > (*it)->_priority) break;
  }
   
  _mailBox.insert(it,message);
 }

 _nbMessages++;

#if USE_nbMessagesMap
 _addMessage(message);
#endif
}

//-----------
size_t Agent::_getNbMessages(void) const  // Classe Message uniquement
{
 if (_mailBox.empty())  return 0;

 return _nbMessages; // _mailBox.size();
}
//--
size_t Agent::getNbMessages(string aClass) const
{
 if (aClass=="Message") return _getNbMessages();

 if (_mailBox.empty())  return 0;

#if USE_nbMessagesMap
//
 return _hmMessages(aClass); // How many message
//
#else
//
 size_t nb=0;

 list<Message*>::const_iterator it;

 for(it=_mailBox.begin();it!=_mailBox.end();it++)
 {
  if ((*it)->isA(aClass)) nb++;
 }

 return nb;
//
#endif
}
//-----------

//-----------
Message* Agent::_getNextMessage(void)     // Classe Message uniquement
{
 Message* newMessage = NULL;

 if (!_mailBox.empty()) {
                         newMessage = _mailBox.front();
                        _mailBox.pop_front();
                        _nbMessages--;
 }

#if USE_nbMessagesMap
 _removeMessage(newMessage);
#endif

 return newMessage;
}
//--
Message* Agent::getNextMessage(string aClass)
{
 if (aClass=="Message") return _getNextMessage();

 Message* newMessage = NULL;

 list<Message*>::iterator it;

 for(it=_mailBox.begin();it!=_mailBox.end();it++)
 {
  if ((*it)->isA(aClass)) { newMessage = *it;
                           _mailBox.erase(it);
                           _nbMessages--;
                            break;
  }
 }

#if USE_nbMessagesMap
 _removeMessage(newMessage);
#endif

 return newMessage;
}
//-----------

//-----------
Message* Agent::_getOneMessage(void)      // Classe Message uniquement
{
 Message* newMessage = NULL;

 size_t nb = _getNbMessages();

 if (nb==0) return newMessage; // NULL

 size_t rang = randomMinMax(0,nb-1);

 list<Message*>::iterator it=_mailBox.begin();
#if 1
  for(size_t i=0;i<rang;i++) it++;
#else
  it = it + rang;
#endif 
  newMessage = *it;
 _mailBox.erase(it);
 _nbMessages--;

#if USE_nbMessagesMap
 _removeMessage(newMessage);
#endif

 return newMessage;
}
//--
   // Retourne un (aleatoirement) des Messages
   // => Besoin de connaitre le nombre de messages d'une classe particuilere
Message* Agent::getOneMessage(string aClass)
{
 if (aClass=="Message") return _getOneMessage();

 Message* newMessage = NULL;

 size_t nb = getNbMessages(aClass);

 if (nb==0) return newMessage; // NULL

 size_t rang = randomMinMax(0,nb-1);

 // Chercher le message du bon type (aClass) se trouvant
 // a la bonne position (rang)

 list<Message*>::iterator it;

 size_t pos=0;

 for(it=_mailBox.begin();it!=_mailBox.end();it++)
 {
  if ((*it)->isA(aClass))
  { 
   if (pos==rang) {
     newMessage = *it;
    _mailBox.erase(it);
    _nbMessages--;
     break;
   }
   else pos++;
  }
 }

#if USE_nbMessagesMap
 _removeMessage(newMessage);
#endif

 return newMessage;
}

//--
//-----------
//-- Et pourquoi pas :
size_t Agent::_getNbMessagesFrom(Agent* src) const // Classe Message uniquement
{
 size_t nb=0;

 list<Message*>::const_iterator it;

 for(it=_mailBox.begin();it!=_mailBox.end();it++)
 {
  if ((*it)->getEmitter()==src) nb++;
 }

 return nb;
}
//--
size_t Agent::getNbMessagesFrom(Agent* src, string aClass) const
{
 if (aClass=="Message") return _getNbMessagesFrom(src);

 size_t nb=0;

 list<Message*>::const_iterator it;

 for(it=_mailBox.begin();it!=_mailBox.end();it++)
 {
  if ((*it)->isA(aClass) && (*it)->getEmitter()==src) nb++;
 }

 return nb;
}
//-----------
//--

//--
//-----------
//-- Et pourquoi pas aussi:
Message* Agent::_getNextMessageFrom(Agent* src) // Classe Message uniquement
{
 Message* newMessage = NULL;

 list<Message*>::iterator it;

 for(it=_mailBox.begin();it!=_mailBox.end();it++)
 {
  if ((*it)->getEmitter()==src) { newMessage = *it;
                                 _mailBox.erase(it);
                                 _nbMessages--;
                                  break;
  }
 }

#if USE_nbMessagesMap
 _removeMessage(newMessage);
#endif

 return newMessage;
}
//--
Message* Agent::getNextMessageFrom(Agent* src, string aClass)
{
 if (aClass=="Message") return _getNextMessageFrom(src);

 Message* newMessage = NULL;

 list<Message*>::iterator it;

 for(it=_mailBox.begin();it!=_mailBox.end();it++)
 {
  if ((*it)->isA(aClass) && (*it)->getEmitter()==src) { newMessage = *it;
                                                       _mailBox.erase(it);
                                                       _nbMessages--;
                                                        break;
  }
 }

#if USE_nbMessagesMap
 _removeMessage(newMessage);
#endif

 return newMessage;
}
//-----------
//--

//--
//-----------
//-- Et pourquoi pas aussi:
Message* Agent::_getOneMessageFrom(Agent* src) // Classe Message uniquement
{
 Message* newMessage = NULL;

 size_t nb = _getNbMessagesFrom(src);

 if (nb==0) return newMessage; // NULL

 size_t rang = randomMinMax(0,nb-1);

  // Chercher le message (Message) avec le bon emetteur se trouvant
  // a la bonne position (rang)

 list<Message*>::iterator it;

 size_t pos=0;

 for(it=_mailBox.begin();it!=_mailBox.end();it++)
 {
  if ((*it)->getEmitter()==src)
  {
   if (pos==rang) {
     newMessage = *it;
    _mailBox.erase(it);
    _nbMessages--;
     break;
   }
   else pos++;
  }
 }

#if USE_nbMessagesMap
 _removeMessage(newMessage);
#endif

 return newMessage;
}
//--
Message* Agent::getOneMessageFrom(Agent* src,string aClass)
{
 if (aClass=="Message") return _getOneMessageFrom(src);

 Message* newMessage = NULL;

 size_t nb = getNbMessagesFrom(src,aClass);

 if (nb==0) return newMessage; // NULL

 size_t rang = randomMinMax(0,nb-1);

  // Chercher le message du bon type (aClass) avec le bon emetteur se trouvant
  // a la bonne position (rang)

 list<Message*>::iterator it;

 size_t pos=0;

 for(it=_mailBox.begin();it!=_mailBox.end();it++)
 {
  if ((*it)->isA(aClass) && (*it)->getEmitter()==src)
  {
   if (pos==rang) {
     newMessage = *it;
    _mailBox.erase(it);
    _nbMessages--;
     break;
   }
   else pos++;
  }
 }

#if USE_nbMessagesMap
 _removeMessage(newMessage);
#endif

 return newMessage;
}
//-----------
//--

//--
void Agent::clearMessageBox(void)
{
 Message *m;

 m = getNextMessage();
 while (m!=NULL)
 {
  delete m;
  m = getNextMessage();
 }
}
/////////////////////////////////////////////////////////////////////////////
///    FIN Gestion mailBox
/////////////////////////////////////////////////////////////////////////////

//--
void Agent::setSensitivity(string aClass,bool yesNo)
{
 if (yesNo)
 {
  // On memorise que l'Agent est sensible
  _sensitivity.insert(aClass);
 }
 else
 {
  // On indique que l'Agent n'est plus sensible
  _sensitivity.erase(aClass);
 }

 Message::setSensitivity(aClass,this,yesNo);
}

//--
size_t Agent::sendMessageTo(Message& aM,Agent *dest) const
{
 if (!exist(dest)) return 0;

 aM.setEmitter((Agent*)this);
 return aM.sendTo(dest);
}

//--
void   Agent::broadcastMessage(Message& aM) const
{
 aM.setEmitter((Agent*)this);
 aM.broadcast();
}

//--
void Agent::_copy(const Agent& anAgent)
{
 /* 
    Recopie la boite aux lettres en prenant soin de
    dupliquer les messages pointe's
    + _nbMessagesMap 
    + gestion de _sensitivity
    + _isSuspended
    + _liveMethod
 */

#if 1
 list<Message*>::const_iterator itDeque;

 for(itDeque=anAgent._mailBox.begin();
     itDeque!=anAgent._mailBox.end();
     itDeque++)
 {
  Message* m = *itDeque;
  Message *newMessage = (Message*)m->virtualCopy();
  putInMailBox(newMessage);
 }
#else
 Agent& pasBeau=(Agent&)anAgent;

 size_t nbMess= anAgent.getNbMessages();

 for(size_t i = 0; i < nbMess; i++)
 {
  // Si on ne peut pas parcourir la boite directement (avec begin,end,[],...)
  // => On enleve le message et on le remet tout de suite a la fin
  Message *m = pasBeau.getNextMessage();
  pasBeau.putInMailBox(m);

  Message *newMessage = (Message*)m->virtualCopy();
  putInMailBox(newMessage);
 }
#endif

#if USE_nbMessagesMap
// _nbMessagesMap = anAgent._nbMessagesMap;  // Copie de la map ...!
// En fait, ce n'est pas utile car on ajoute dans la boite avec
// putInMailBox... et donc on fait deja ce qu'il faut dans la map..!
#endif

 set<string>::const_iterator itSet;
 for(itSet = anAgent._sensitivity.begin();
     itSet!= anAgent._sensitivity.end();
     itSet++)
 {
  setSensitivity(*itSet,true);
 }

 _isSuspended = anAgent._isSuspended;

 _liveMethod  = anAgent._liveMethod;
}

//--
void Agent::_destroy(void)
{
 /*
    Destruction de la boite aux lettres en prenant soin de
    detruire les messages pointe's
    + gestion de _sensitivity
 */
 clearMessageBox();

 set<string> sensitivity = _sensitivity;
 set<string>::iterator it;
 for(it = sensitivity.begin(); it != sensitivity.end(); it++)
 {
  setSensitivity(*it,false);
 }

 _liveMethod = NULL;
}
