#ifndef _NOMESSAGE_H
#define _NOMESSAGE_H

#include "HolonMessage.h"

class Agent;

class NoMessage: public HolonMessage{
 private:
  static TypeTreeNode * nomessagetreenode;
 
 public:
  // Accessing
  static set<Agent *> getSubscribers();
  static TypeTreeNode * getTypetreenode();
  virtual string getType();

  // Broadcasting
  static void broadcast(Message *);

  // Constructing
  NoMessage();

  // Destructing
  virtual ~NoMessage();
  
  // Sending
  virtual void sendTo(Agent *);

  // Testing
  virtual bool isA(string);
};

#endif
