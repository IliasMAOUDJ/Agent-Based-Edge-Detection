#ifndef _ENDMESSAGE_H
#define _ENDMESSAGE_H

#include "HolonMessage.h"

class Agent;

class EndMessage: public HolonMessage{
 private:
  static TypeTreeNode * endmessagetreenode;
 
 public:
  // Accessing
  static set<Agent *> getSubscribers();
  static TypeTreeNode * getTypetreenode();
  virtual string getType();

  // Broadcasting
  static void broadcast(Message *);

  // Constructing
  EndMessage();

  // Destructing
  virtual ~EndMessage();
  
  // Sending
  virtual void sendTo(Agent *);

  // Testing
  virtual bool isA(string);
};

#endif
