#ifndef _MAS_H_
#define _MAS_H_

//%%%%%%%%%%%%%%%%%%%%%  MACRO DEFCLASS  %%%%%%%%%%%%%%%%%%%%%%%%%%%

#define DEFCLASS(className) \
private: virtual string getClassName(void) const\
{ \
  return #className; \
} \
private: \
friend class InstanceRecorder; \
virtual void* virtualCopy(void) const \
{ \
 return (void*)new className(*this); \
}

//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

class Agent;
class Scheduler;
class Message;

#include "Agent.h"
#include "Message.h"
#include "Scheduler.h"

#include "UtilAgent.h"

extern void   getAllAgents(string aClass, vector<Agent*>& anAgentVector);
extern Agent* getAgent(string aName);
extern bool   exist(const Agent* anAgent);

/* 

*******************************   Utilities  (UtilAgent.h) *******************

///////////////////////////// Gestion du temps ///////////////////////////////

extern double getTimeMicroSeconds(void);               // En microsecondes
extern double getTimeMilliSeconds(void);               // En millisecondes
extern double getTimeSeconds(void);                    // En secondes

///////////////////////////// Generation aleatoire //////////////////////////

extern void   initRandom(void);                    // Initialisation generateur
extern size_t randomMinMax(size_t min, size_t max);// Generation dans [min,max]
extern double random01(void);                      // Generation dans [0.0,1.0]

*******************************  class Agent (Agent.h) ***********************

typedef void (Agent::*liveMethodType)(double dt); // Pour get/setLiveMethod...

class Agent {

 DEFCLASS(Agent)

  friend ostream& operator<<(ostream& os, const Agent& anAgent);

 public:
     
   // Allocateurs/Desallocateurs

           Agent(void);
           Agent(const Agent& anAgent);
           Agent& operator=(const Agent& anAgent);
  virtual ~Agent(void);

  virtual  void live(double dt)               // dt en seconde : temps depuis
           {                                  // la derniere activation
            // Rien pour un Agent de base
            (void)dt;  // Pour eviter un warning
           }

           void suspend(void);
           void restart(void);
           bool isSuspended(void) const;

       // Pour programmeurs avertis:
       // Pour changer de methode appelee par l'ordonnanceur... setLiveMethod
       // Voir exemple :
       // Exemples/ExemplesPourProgrammeursAvertis/Exemple_setLiveMethod
       // --
       // Dans setLiveMethod, si newLiveMethod est NULL alors c'est la methode
       // live qui sera appelee par l'ordonnanceur...
       // C'est le comportement par defaut..!

           void setLiveMethod(liveMethodType newLiveMethod); // Progr. avertis!
           liveMethodType getLiveMethod(void);               // Progr. avertis!
       //

   // Comparaisons

  friend   bool operator==(const Agent& anAgent1, const Agent& anAgent2);
  friend   bool operator!=(const Agent& anAgent1, const Agent& anAgent2);

   // Inspecteurs

           string getName(void) const;  // N'a pas de sens dans le constructeur 
           unsigned long getSuffix(void) const; // Idem : n'a pas de sens ...
           string getClass(void) const; // DEFCLASS: virtual getClassName ... 
           bool   isA(string aClass) const;

   // Gestion des messages

           size_t   getNbMessages(string aClass="Message") const;
           Message* getNextMessage(string aClass="Message"); // Le suivant
           void     clearMessageBox(void);

           void     setSensitivity(string aClass,bool yesNo);
                                                                 // retourne
   virtual size_t   sendMessageTo(Message& aM,Agent *dest) const;// 1(ok),0(ko)
   virtual void     broadcastMessage(Message& aM) const;

 protected:

   // Methodes a appeler par une classe derivee

                                // Methode qui doit etre appelee dans le cons-
           void newAgent(void); // tructeur d'une classe derivee
                                // => Arbre d'heritage

           void newAgent(Agent* This); // Idem mais pour l'heritage multiple

   //  display a appeler dans une classe derivee       // display est une
   virtual void display(ostream& os) const;            // methode appelee
                                                       // dans operator<<

   //  isEqualTo a appeler dans une classe derivee     // isEqualTo est une
           bool isEqualTo(const Agent& anAgent) const; // methode appelee
                                                       // dans operator==
};

******************************  class Message (Message.h) ********************

class Message {

 DEFCLASS(Message)

  friend ostream& operator<<(ostream& os, const Message& aMessage);

 public:
     
   // Allocateurs/Desallocateurs

           Message(void);
           Message(const Message& aMessage);
           Message& operator=(const Message& aMessage);
  virtual ~Message(void);

           void   setPriority(size_t priority);
           size_t getPriority(void) const;

   // Comparaisons

  friend   bool operator==(const Message& aMessage1, const Message& aMessage2);
  friend   bool operator!=(const Message& aMessage1, const Message& aMessage2);

   // Inspecteurs

           string getClass(void)   const; // DEFCLASS: virtual getClassName ...
           bool   isA(string aClass) const;
           Agent* getEmitter(void) const; // Avant l'envoi d'un message,
                                          // l'emetteur est NULL ...!
 protected:

   // Methodes a appeler par une classe derivee

                                // Methode qui doit etre appelee dans le cons-
         void newMessage(void); // tructeur d'une classe derivee
                                // => Arbre d'heritage

         void newMessage(Message* This); // Idem mais pour l'heritage multiple 

   //  display a appeler dans une classe derivee        // display est une
   virtual void display(ostream& os) const;             // methode appelee
                                                        // dans operator<<

   //  isEqualTo a appeler dans une classe derivee      // isEqualTo est une
   virtual bool isEqualTo(const Message& aMessage) const; // methode appelee
                                                          // dans operator==
};

*****************************  class Scheduler (Scheduler.h) *****************

class Scheduler
{

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
};

*/

#endif // _MAS_H_
