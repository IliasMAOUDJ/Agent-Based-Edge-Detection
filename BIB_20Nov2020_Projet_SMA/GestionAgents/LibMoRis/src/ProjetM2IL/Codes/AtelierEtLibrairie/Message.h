#ifndef _MESSAGE_H
#define _MESSAGE_H

#include <iostream>
#include <set>
#include <string>

using namespace std;

class Agent;
class TypeTreeNode;

class Message {
 private:
  Agent * sender;
  static TypeTreeNode * messagetreenode;

 public:
  // Accessing
  static set<Agent *> getSubscribers();
  static TypeTreeNode * getTypetreenode();
  Agent * getSender();
  virtual string getType();
  void setSender(Agent *);

  // Broadcasting
  static void broadcast(Message *);

  // Constructing
  Message();
  Message(const Message &);
  
  // Destructing
  virtual ~Message();
  
  // Sending
  virtual void sendTo(Agent *);
  
  // Testing
  virtual bool isA(string);
};

#endif
