#include "Message.h"

map<string, set<Agent*> > Message::_mapMembership;

//--
Message::Message(void)
{
 _emitter = NULL;
 _deadMessage = false;        // cf void newMessage(Message* This)

 newMessage();

 setPriority(0); // Par defaut, priorite minimale
}

//--
Message::Message(const Message& aMessage)
{
 _deadMessage = false;        // cf void newMessage(Message* This)
 _copy(aMessage);

 newMessage();
}

//--
Message& Message::operator=(const Message& aMessage)
{
 if (this != &aMessage)
 {
  _destroy();
  _copy(aMessage);
 }
 
 return *this;
}

//--
Message::~Message(void)
{
 _messagesClasses.clear();
 _destroy();
}

//--
void Message::newMessage(void)
{
 if (_deadMessage) return;

 _messagesClasses.push_back(getClassName());
}

//--
void Message::newMessage(Message* This)    // This (pour l'heritage multiple)
{
 if (_deadMessage) return;

 if (This == NULL || this == This) newMessage();   // Cas heritage simple
 else { // if (this != This)   // Cas heritage multiple !

 vector<string>::iterator iter;
 for(iter = _messagesClasses.begin(); iter != _messagesClasses.end(); iter++)
 {
  This->_messagesClasses.push_back(*iter);
 }

 _messagesClasses.clear();

                      // Indique que le Message ("mauvaise" branche de
 _deadMessage = true; // l'heritage multiple) n'est plus a considerer lors
                      // d'un nouvel appel a newMessage
 }
}

//--
bool operator==(const Message& aMessage1, const Message& aMessage2)
{
 return aMessage1.isEqualTo(aMessage2);
}

//--
bool operator!=(const Message& aMessage1, const Message& aMessage2)
{
 return !(aMessage1==aMessage2);
}

//--
string Message::getClass(void) const
{
  return getClassName();
}

bool   Message::isA(string aClass) const
{
            // Rappel: faire un find si autre chose que vector (set,...)

 vector<string>::const_iterator iter;
 for(iter = _messagesClasses.begin(); iter != _messagesClasses.end(); iter++)
 {
  if (*iter==aClass) return true;
 }

 return false;
}

//--
Agent* Message::getEmitter(void) const
{
 return _emitter;
}

//--
void   Message::setEmitter(Agent* emitter)
{
 if (emitter==NULL)
 {
  cerr << ">---------------------------------------------------------" << endl;
  cerr << "Warning: Message::setEmitter(Agent* emitter),\n"
       << "the emitter is NULL !"                                      << endl;
  cerr << "<---------------------------------------------------------" << endl;
 }

 _emitter = emitter;
}

//--
void   Message::setPriority(size_t priority)
{
 _priority = priority;
}

//--
size_t Message::getPriority(void) const
{
 return _priority;
}

//--
size_t Message::sendTo(Agent *dest) const
{
 if (!exist(dest)) return 0;

 if (_emitter==NULL)
 {
  cerr << ">---------------------------------------------------------" << endl;
  cerr << "Warning: Message::sendTo(Agent *dest),\n"
       << "the emitter of the Message is NULL !"                       << endl;
  cerr << "<---------------------------------------------------------" << endl;
 }

 Message *newMessage = (Message*)virtualCopy();

 dest->putInMailBox(newMessage);

 return 1;
}

//--
static ssize_t currentBroadcastNumber = 0;

void Message::broadcast(void) const
{
 if (_emitter==NULL)
 {
  cerr << ">---------------------------------------------------------" << endl;
  cerr << "Warning: Message::broadcast(void),\n"
       << "the emitter of the Message is NULL !"                       << endl;
  cerr << "<---------------------------------------------------------" << endl;
 }

 // On suppose que l'on a fait au moins une fois newMessage() (ds Message.cpp)
 // => On fait ce traitement si le programmeur a oublie de faire newMessage()
 // dans la classe derivee getClassName()
 if (_messagesClasses[_messagesClasses.size()-1]!=getClassName())
 {
  Message* pasBeau = (Message*)this;
  pasBeau->_messagesClasses.push_back(getClassName());
 }

 currentBroadcastNumber++;
 if (currentBroadcastNumber < 0) // Quand on est passe en negatif, il faut
 {                               // faire quelque chose !
  currentBroadcastNumber=0;
  
  // Et mettre a -1 tous les _broadcastNumber des Agents inscrits dans
  // _mapMembership (afin d'etre sur de ne pas omettre de messages ....)

  map<string, set<Agent*> >::iterator iter;
  for(iter = _mapMembership.begin(); iter != _mapMembership.end(); iter++)
  {
   set<Agent*>::iterator iterSet;
   for(iterSet  = (*iter).second.begin();
       iterSet != (*iter).second.end();
       iterSet++)
   {
    (*iterSet)->_broadcastNumber = -1;
   }
  }
 }

 vector<string>::const_iterator iter;
 for(iter = _messagesClasses.begin(); iter != _messagesClasses.end(); iter++)
 {
  map<string, set<Agent*> >::iterator iterM;
  iterM = _mapMembership.find(*iter);
  if (iterM != _mapMembership.end())
  {
   set<Agent*>::iterator iterSet;
   for(iterSet  = (*iterM).second.begin();
       iterSet != (*iterM).second.end();
       iterSet++)
   {
    Message *newMessage = (Message*)virtualCopy();
    if (exist(*iterSet)) {

            if ((*iterSet)->_broadcastNumber!=currentBroadcastNumber)
            {
             (*iterSet)->putInMailBox(newMessage);
             (*iterSet)->_broadcastNumber = currentBroadcastNumber;
            }
    }
   }
  }
 }
}

//--
// static map<string, set <Agent*> > _mapMembership; // Pour les abonnements
void Message::setSensitivity(string aClass,Agent* anAgent,bool yesNo)
{
 if (anAgent==NULL)
 {
  cerr << ">---------------------------------------------------------" << endl;
  cerr << "Warning: void Message::"
       <<       "setSensitivity(string aClass,Agent* anAgent,bool yesNo),\n"
       << "anAgent parameter is NULL !"                                << endl;
  cerr << "<---------------------------------------------------------" << endl;
  return;
 }

 anAgent->_broadcastNumber = currentBroadcastNumber - 1; // Tres tres prive !

 map<string, set <Agent*> >::iterator it;

 it = _mapMembership.find(aClass);

 if (it!=_mapMembership.end()) { // Classe trouvee
        if (yesNo) (*it).second.insert(anAgent); 
        else       (*it).second.erase(anAgent);
 }
 else {
        if (yesNo) {
                    set <Agent*> aNewSet;
                    aNewSet.insert(anAgent);
                   _mapMembership[aClass] = aNewSet;
        }
      //else {
      //  cerr << ">-------------------------------------------------" << endl;
      //  cerr << "Warning, Message::setSensitivity(aClass,anAgent,false),\n"
      //       << "class " << aClass << " is not yet recorded"         << endl;
      //  cerr << "<-------------------------------------------------" << endl;
      //}
 }
}

//--
ostream& operator<<(ostream& os,const Message& aMessage)
{
 aMessage.display(os);
 return os;
}

//--
void Message::_printMapMembership(ostream& os)
{
 os << "Etat de l'abonnement pour le broadcast" << endl;
 map<string, set<Agent*> >::iterator iter;
 for(iter = _mapMembership.begin(); iter != _mapMembership.end(); iter++)
 {
  os << "***** Classe : " << (*iter).first << " ********" << endl;
  set<Agent*>::iterator iterSet;
  for(iterSet  = (*iter).second.begin();
      iterSet != (*iter).second.end();
      iterSet++)
  {
   if (exist(*iterSet)) os << (*iterSet)->getName() << endl;
  }
 }
}

//--
void Message::display(ostream& os) const
{
 if (!_deadMessage)
 {
  string nomEmitter="(NULL)";
  os << "Classe du message : " << getClass() << endl;

  // _printMapMembership(os);

  if (exist(_emitter)) nomEmitter = _emitter->getName();
  os << "Emetteur du message " << nomEmitter << endl;

  os << "Priorite du message " << _priority  << endl;

  os << "Liste des classes heritees par le messsage:" << endl;
  vector<string>::const_iterator iter;
  for(iter = _messagesClasses.begin(); iter != _messagesClasses.end(); iter++)
  {
   os << *iter << endl;
  }
 }
}

//--
bool Message::isEqualTo(const Message& aMessage) const
{
 (void)aMessage;  // Pour eviter un warning

#if 1                                                // A voir ..!
 if (_priority != aMessage._priority) return false;
 return true;
#else                                                //            OU
 if (_priority != aMessage._priority) return false;
 if (_class    != aMessage._class)    return false;
 if (_emitter  != aMessage._emitter)  return false;
 return true;
#endif
}

void Message::_copy(const Message& aMessage)
{
 _emitter  = aMessage._emitter;
 _priority = aMessage._priority;
}

void Message::_destroy(void)
{
}

