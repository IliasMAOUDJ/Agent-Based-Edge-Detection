#ifndef _AREQUEST_H
#define _AREQUEST_H

#include "HolonMessage.h"

class Agent;

class ARequest: public HolonMessage{
 private:
  static TypeTreeNode * arequesttreenode;
 
 public:
  // Accessing
  static set<Agent *> getSubscribers();
  static TypeTreeNode * getTypetreenode();
  virtual string getType();

  // Broadcasting
  static void broadcast(Message *);

  // Constructing
  ARequest();

  // Destructing
  virtual ~ARequest();
  
  // Sending
  virtual void sendTo(Agent *);

  // Testing
  virtual bool isA(string);
};

#endif
