#ifndef _ACKMESSAGE_H
#define _ACKMESSAGE_H

#include "HolonMessage.h"

class Agent;

class AckMessage: public HolonMessage{
 private:
  static TypeTreeNode * ackmessagetreenode;
 
 public:
  // Accessing
  static set<Agent *> getSubscribers();
  static TypeTreeNode * getTypetreenode();
  virtual string getType();

  // Broadcasting
  static void broadcast(Message *);

  // Constructing
  AckMessage();

  // Destructing
  virtual ~AckMessage();
  
  // Sending
  virtual void sendTo(Agent *);

  // Testing
  virtual bool isA(string);
};

#endif
