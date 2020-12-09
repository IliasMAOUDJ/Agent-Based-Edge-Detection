#include "Agent.h"
#include "Message.h"
#include "TypeTreeNode.h"

TypeTreeNode * Message::messagetreenode = NULL;

//Accessing
Agent * Message::getSender(){
  return sender;
}
set<Agent *> Message::getSubscribers() {
  set<Agent *> subscribers;
  Message::getTypetreenode()->getAgents(subscribers);
  return subscribers;
}
string Message::getType() {
  return Message::getTypetreenode()->getType();
}
TypeTreeNode * Message::getTypetreenode() {
  if (!messagetreenode) {
    messagetreenode = new TypeTreeNode();
    messagetreenode->initialize(NULL, "Message");
  }
  return messagetreenode;
}
void Message::setSender(Agent * agent){
  this->sender = agent;
}

// Broadcasting
void Message::broadcast(Message * message) {
  set<Agent *> subscribers = Message::getSubscribers();
  for (set<Agent *>::iterator it = subscribers.begin(); it != subscribers.end(); it++) {
    if (Agent::exist(*it)) {
      message->sendTo(*it);
    }
  }
  delete message;
}

// Constructing
Message::Message() {
  if (!messagetreenode) {
    messagetreenode = new TypeTreeNode();
    messagetreenode->initialize(NULL, "Message");
  }
  this->setSender(NULL);
}
Message::Message(const Message & message) {
  this->setSender(message.sender);
}

// Destructing
Message::~Message() {
}

// Sending
void Message::sendTo(Agent * receiver) {
  Message * newMessage = new Message(*this);
  receiver->saveMessage(newMessage);
}

// Testing
bool Message::isA(string type) {
  return Message::getTypetreenode()->isA(type);
}
