#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <iostream>

#include <string>
#include <vector>
#include <map>
#include <set>

#include "MAS.h"
#include "Agent.h"

using namespace std;

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
           bool isEqualTo(const Message& aMessage) const; // methode appelee
                                                          // dans operator==
 private:

    friend class Agent;

   // Operations

           void   setEmitter(Agent *emitter);

           size_t sendTo(Agent *dest) const;      // retourne 1 (ok), 0 (ko)
           void   broadcast(void) const;          // retourne 1 (ok), 0 (ko)

           static void setSensitivity(string aClass,Agent* anAgent,bool yesNo);

   // Attributs prives

   Agent* _emitter; // L'emetteur du message

   size_t _priority;

           bool      _deadMessage;  // Voir le commentaire a la fin du code
                                    // de void newMessage(Message* This)

   vector<string> _messagesClasses; // Liste des classes dont le Message depend

   static map<string, set<Agent*> > _mapMembership; // Pour les abonnements

        //        ^            ^         ^
        //      nom de    Ensemble des instances
        //      classe    abonnees au message de type "classe"
        // => Dans un agent, il faut l'ensemble des messages
        //    pour lesquels il est sensible !

   static void _printMapMembership(ostream& os);

   // Methodes privees

   void _copy(const Message& aMessage);
   void _destroy(void);

};

#endif // _MESSAGE_H_
