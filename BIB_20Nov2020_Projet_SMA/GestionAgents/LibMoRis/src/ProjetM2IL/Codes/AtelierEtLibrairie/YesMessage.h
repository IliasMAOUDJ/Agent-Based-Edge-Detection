#ifndef _YESMESSAGE_H
#define _YESMESSAGE_H

#include "HolonMessage.h"

class Agent;

class YesMessage: public HolonMessage{
 private:
	static TypeTreeNode * yesmessagetreenode;
 
 public:
	// Accessing
	static set<Agent *> getSubscribers();
	static TypeTreeNode * getTypetreenode();
	virtual string getType();
	
	// Broadcasting
  static void broadcast(Message *);
  
  // Constructing
  YesMessage();
  
  // Destructing
  virtual ~YesMessage();
  
  // Sending
  virtual void sendTo(Agent *);
  
  // Testing
  virtual bool isA(string);
};

#endif
