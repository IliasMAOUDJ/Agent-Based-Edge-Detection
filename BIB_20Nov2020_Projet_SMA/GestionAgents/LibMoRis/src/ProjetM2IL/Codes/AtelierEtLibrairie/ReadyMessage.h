#ifndef _READYMESSAGE_H
#define _READYMESSAGE_H

#include "HolonMessage.h"

class Agent;

class ReadyMessage: public HolonMessage{
 private:
	static TypeTreeNode * readymessagetreenode;
 
 public:
	// Accessing
	static set<Agent *> getSubscribers();
	static TypeTreeNode * getTypetreenode();
	virtual string getType();
	
	// Broadcasting
	static void broadcast(Message *);
	
	// Constructing
	ReadyMessage();
	
	// Destructing
	virtual ~ReadyMessage();
	
	// Sending
	virtual void sendTo(Agent *);
	
	// Testing
	virtual bool isA(string);
};

#endif
