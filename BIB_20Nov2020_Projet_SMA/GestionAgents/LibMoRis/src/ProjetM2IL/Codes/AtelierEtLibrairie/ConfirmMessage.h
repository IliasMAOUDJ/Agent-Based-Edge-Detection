#ifndef _CONFIRMMESSAGE_H
#define _CONFIRMMESSAGE_H

#include "HolonMessage.h"

class Agent;

class ConfirmMessage: public HolonMessage{
 private:
  static TypeTreeNode * confirmmessagetreenode;
 
 public:
  // Accessing
  static set<Agent *> getSubscribers();
  static TypeTreeNode * getTypetreenode();
  virtual string getType();

  // Broadcasting
  static void broadcast(Message *);

  // Constructing
  ConfirmMessage();

  // Destructing
  virtual ~ConfirmMessage();
  
  // Sending
  virtual void sendTo(Agent *);

  // Testing
  virtual bool isA(string);
};

#endif
