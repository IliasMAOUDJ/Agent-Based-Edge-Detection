#ifndef _HOLONMESSAGE_H
#define _HOLONMESSAGE_H

#include "Message.h"

class Agent;

class HolonMessage: public Message{
 private:
  static TypeTreeNode * holonmessagetreenode;
 
 public:
  // Accessing
  static set<Agent *> getSubscribers();
  static TypeTreeNode * getTypetreenode();
  virtual string getType();

  // Broadcasting
  static void broadcast(Message *);

  // Constructing
  HolonMessage();

  // Destructing
  virtual ~HolonMessage();
  
  // Sending
  virtual void sendTo(Agent *);

  // Testing
  virtual bool isA(string);
};

#endif
