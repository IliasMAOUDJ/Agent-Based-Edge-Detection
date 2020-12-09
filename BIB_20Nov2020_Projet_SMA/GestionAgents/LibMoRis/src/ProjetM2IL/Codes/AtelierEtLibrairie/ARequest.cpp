#include "ARequest.h"
#include "Agent.h"
#include "TypeTreeNode.h"

TypeTreeNode * ARequest::arequesttreenode = NULL;

// Accessing
set<Agent *> ARequest::getSubscribers() {
  set<Agent *> subscribers;
  ARequest::getTypetreenode()->getAgents(subscribers);
  return subscribers;
}
string ARequest::getType() {
  return ARequest::getTypetreenode()->getType();
}
TypeTreeNode * ARequest::getTypetreenode() {
  if (!arequesttreenode) {
    arequesttreenode = new TypeTreeNode();
    arequesttreenode->initialize(Message::getTypetreenode(), "ARequest");
    Message::getTypetreenode()->addChildNode(ARequest::getTypetreenode());
  }
  return arequesttreenode;
}

// Broadcasting
void ARequest::broadcast(Message * message) {
  set<Agent *> subscribers = ARequest::getSubscribers();
  for (set<Agent *>::iterator it = subscribers.begin(); it != subscribers.end(); it++) {
    if (Agent::exist(*it)) {
      message->sendTo(*it);
    }
  }
  delete message;
}

// Constructing
ARequest::ARequest():HolonMessage() {
  if (!arequesttreenode) {
    arequesttreenode = new TypeTreeNode();
    arequesttreenode->initialize(Message::getTypetreenode(), "ARequest");
    Message::getTypetreenode()->addChildNode(ARequest::getTypetreenode());
  }
  this->setSender(NULL);
}

// Destructing
ARequest::~ARequest() {
}

// Sending
void ARequest::sendTo(Agent * receiver) {
  ARequest * newMessage = new ARequest(*this);
  receiver->saveMessage(newMessage);
}

// Testing
bool ARequest::isA(string type) {
  return ARequest::getTypetreenode()->isA(type);
}
