#include "BRequest.h"
#include "Agent.h"
#include "TypeTreeNode.h"

TypeTreeNode * BRequest::brequesttreenode = NULL;

// Accessing
set<Agent *> BRequest::getSubscribers() {
  set<Agent *> subscribers;
  BRequest::getTypetreenode()->getAgents(subscribers);
  return subscribers;
}
string BRequest::getType() {
  return BRequest::getTypetreenode()->getType();
}
TypeTreeNode * BRequest::getTypetreenode() {
  if (!brequesttreenode) {
    brequesttreenode = new TypeTreeNode();
    brequesttreenode->initialize(Message::getTypetreenode(), "BRequest");
    Message::getTypetreenode()->addChildNode(BRequest::getTypetreenode());
  }
  return brequesttreenode;
}

// Broadcasting
void BRequest::broadcast(Message * message) {
  set<Agent *> subscribers = BRequest::getSubscribers();
  for (set<Agent *>::iterator it = subscribers.begin(); it != subscribers.end(); it++) {
    if (Agent::exist(*it)) {
      message->sendTo(*it);
    }
  }
  delete message;
}

// Constructing
BRequest::BRequest():HolonMessage() {
  if (!brequesttreenode) {
    brequesttreenode = new TypeTreeNode();
    brequesttreenode->initialize(Message::getTypetreenode(), "BRequest");
    Message::getTypetreenode()->addChildNode(BRequest::getTypetreenode());
  }
  this->setSender(NULL);
}

// Destructing
BRequest::~BRequest() {
}

// Sending
void BRequest::sendTo(Agent * receiver) {
  BRequest * newMessage = new BRequest(*this);
  receiver->saveMessage(newMessage);
}

// Testing
bool BRequest::isA(string type) {
  return BRequest::getTypetreenode()->isA(type);
}
