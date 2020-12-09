#ifndef _BREQUEST_H
#define _BREQUEST_H

#include "HolonMessage.h"

class Agent;

class BRequest: public HolonMessage{
 private:
  static TypeTreeNode * brequesttreenode;
 
 public:
  // Accessing
  static set<Agent *> getSubscribers();
  static TypeTreeNode * getTypetreenode();
  virtual string getType();

  // Broadcasting
  static void broadcast(Message *);

  // Constructing
  BRequest();

  // Destructing
  virtual ~BRequest();
  
  // Sending
  virtual void sendTo(Agent *);

  // Testing
  virtual bool isA(string);
};

#endif
