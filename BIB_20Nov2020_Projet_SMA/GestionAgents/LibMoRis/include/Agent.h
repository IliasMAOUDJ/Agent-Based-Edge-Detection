#ifndef _AGENT_H_
#define _AGENT_H_

#include <iostream>

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>

#include <sys/types.h> // Pour ssize_t...

#include "MAS.h"
#include "Scheduler.h"

using namespace std;

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

       // Pour programmeurs avertis :
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

           string getName(void) const; // N'a pas de sens dans le constructeur 
           unsigned long getSuffix(void) const;// Idem : n'a pas de sens ...
           string getClass(void) const;// DEFCLASS: virtual getClassName ... 
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
 private:

   // Attributs et methodes prives d'un Agent

   liveMethodType _liveMethod;

   string     _baseClass;
   string         _class;

   void            setName(unsigned long instanceNumber);
   string         _name;          // Nom de l'instance (classe.noInstance)

   bool      _deadAgent;  // Voir le commentaire a la fin du code
                          // de void newAgent(Agent* This)

   vector<string> _agentsClasses; // Liste des classes dont l'agent depend

 private:

  // Attributs prives pour la gestion des Messages

   friend class Message;

   list<Message*> _mailBox;             // Boite a messages de l'Agent
   size_t         _nbMessages;          // Le nombre de messages dedans
   void putInMailBox(Message* message); // Methode privee...

   size_t   _getNbMessages(void) const;
   Message* _getNextMessage(void);// Le suivant

#if 1         // 1 : pour rendre publique les 4 methodes suivantes
 public:      // getOneMessage, getNbMessagesFrom, getNextMessageFrom et
#endif        // getOneMessageFrom

   Message*  getOneMessage(string aClass="Message"); // Un aleatoirement

   size_t    getNbMessagesFrom(Agent* src,string aClass="Message")const;
   Message*  getNextMessageFrom(Agent* src,string aClass="Message");
   Message*  getOneMessageFrom(Agent* src,string aClass="Message");

 private:

   Message* _getOneMessage(void); // Un aleatoirement

   size_t   _getNbMessagesFrom(Agent* src) const;
   Message* _getNextMessageFrom(Agent* src);
   Message* _getOneMessageFrom(Agent* src);

   map<string,size_t> _nbMessagesMap;  // Pour chaque classe, le nb de messages
   void   _addMessage(Message* message);  // 3 methodes pour connaitre le
   void   _removeMessage(Message* message); // nb de messages d'un type
   size_t _hmMessages(string aClass) const; // particulier presents ds _mailBox

                                 // Gestion des classes de message pour
   set<string>     _sensitivity; // lesquelles un agent est sensible lors
                                 // d'un broadcast

   void _copy(const Agent& anAgent); // Recopie de la boite _mailBox
                                     // + gestion _sensitivity
   void _destroy(void);              // Destruction de la boite _mailBox
                                     // + gestion _sensitivity

 private:

   // Des attributs et methodes tres tres prives liees a l'ordonnanceur
   //                           ----------------

     friend class Scheduler;

     bool       _isSuspended;

     Scheduler* _theAgentsSched; // L'ordonnanceur auquel l'agent est attache

                                 // Pour accelerer le delete :) ! afin
     ssize_t _posInCurrentCycle; // d'enlever rapidement un agent en cours
                                 // de cycle (voir Scheduler::removeInstance)

     double     _lastActivation; // Permet live(double dt) // dt en secondes

   // Des attributs et methodes tres tres prives liees aux messages
   //                           ----------------

                                 // Permet de n'envoyer un message en broadcast
     ssize_t _broadcastNumber;   // a un Agent qu'une seule fois meme si cet
                                 // Agent est abonne a une classe de message et
                                 // a une classe derivee de cette classe
};

#endif // _AGENT_H_
