#ifndef _CREQUEST_H
#define _CREQUEST_H

#include "HolonMessage.h"

class Agent;

class CRequest: public HolonMessage{
 private:
  static TypeTreeNode * crequesttreenode;
 
 public:
  // Accessing
  static set<Agent *> getSubscribers();
  static TypeTreeNode * getTypetreenode();
  virtual string getType();

  // Broadcasting
  static void broadcast(Message *);

  // Constructing
  CRequest();

  // Destructing
  virtual ~CRequest();
  
  // Sending
  virtual void sendTo(Agent *);

  // Testing
  virtual bool isA(string);
};

#endif
