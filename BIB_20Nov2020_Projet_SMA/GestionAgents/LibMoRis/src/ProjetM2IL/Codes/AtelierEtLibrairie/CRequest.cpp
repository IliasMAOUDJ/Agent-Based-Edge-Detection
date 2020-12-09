#include "CRequest.h"
#include "Agent.h"
#include "TypeTreeNode.h"

TypeTreeNode * CRequest::crequesttreenode = NULL;

// Accessing
set<Agent *> CRequest::getSubscribers() {
  set<Agent *> subscribers;
  CRequest::getTypetreenode()->getAgents(subscribers);
  return subscribers;
}
string CRequest::getType() {
  return CRequest::getTypetreenode()->getType();
}
TypeTreeNode * CRequest::getTypetreenode() {
  if (!crequesttreenode) {
    crequesttreenode = new TypeTreeNode();
    crequesttreenode->initialize(Message::getTypetreenode(), "CRequest");
    Message::getTypetreenode()->addChildNode(CRequest::getTypetreenode());
  }
  return crequesttreenode;
}

// Broadcasting
void CRequest::broadcast(Message * message) {
  set<Agent *> subscribers = CRequest::getSubscribers();
  for (set<Agent *>::iterator it = subscribers.begin(); it != subscribers.end(); it++) {
    if (Agent::exist(*it)) {
      message->sendTo(*it);
    }
  }
  delete message;
}

// Constructing
CRequest::CRequest():HolonMessage() {
  if (!crequesttreenode) {
    crequesttreenode = new TypeTreeNode();
    crequesttreenode->initialize(Message::getTypetreenode(), "CRequest");
    Message::getTypetreenode()->addChildNode(CRequest::getTypetreenode());
  }
  this->setSender(NULL);
}

// Destructing
CRequest::~CRequest() {
}

// Sending
void CRequest::sendTo(Agent * receiver) {
  CRequest * newMessage = new CRequest(*this);
  receiver->saveMessage(newMessage);
}

// Testing
bool CRequest::isA(string type) {
  return CRequest::getTypetreenode()->isA(type);
}
