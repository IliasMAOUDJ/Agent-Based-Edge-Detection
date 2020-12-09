#ifndef _NEARMESSAGE_H
#define _NEARMESSAGE_H

#include "HolonMessage.h"

class Agent;

class NearMessage: public HolonMessage{
 private:
  static TypeTreeNode * nearmessagetreenode;
 
 public:
  // Accessing
  static set<Agent *> getSubscribers();
  static TypeTreeNode * getTypetreenode();
  virtual string getType();

  // Broadcasting
  static void broadcast(Message *);

  // Constructing
  NearMessage();

  // Destructing
  virtual ~NearMessage();
  
  // Sending
  virtual void sendTo(Agent *);

  // Testing
  virtual bool isA(string);
};

#endif
