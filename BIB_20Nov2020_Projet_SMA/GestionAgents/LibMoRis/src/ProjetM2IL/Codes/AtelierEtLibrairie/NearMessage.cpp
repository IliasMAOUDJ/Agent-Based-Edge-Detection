#include "Agent.h"
#include "NearMessage.h"
#include "TypeTreeNode.h"

TypeTreeNode * NearMessage::nearmessagetreenode = NULL;

// Accessing
set<Agent *> NearMessage::getSubscribers() {
  set<Agent *> subscribers;
  NearMessage::getTypetreenode()->getAgents(subscribers);
  return subscribers;
}
string NearMessage::getType() {
  return NearMessage::getTypetreenode()->getType();
}
TypeTreeNode * NearMessage::getTypetreenode() {
  if (!nearmessagetreenode) {
    nearmessagetreenode = new TypeTreeNode();
    nearmessagetreenode->initialize(Message::getTypetreenode(), "NearMessage");
    Message::getTypetreenode()->addChildNode(NearMessage::getTypetreenode());
  }
  return nearmessagetreenode;
}

// Broadcasting
void NearMessage::broadcast(Message * message) {
  set<Agent *> subscribers = NearMessage::getSubscribers();
  for (set<Agent *>::iterator it = subscribers.begin(); it != subscribers.end(); it++) {
    if (Agent::exist(*it)) {
      message->sendTo(*it);
    }
  }
  delete message;
}

// Constructing
NearMessage::NearMessage():HolonMessage() {
  if (!nearmessagetreenode) {
    nearmessagetreenode = new TypeTreeNode();
    nearmessagetreenode->initialize(Message::getTypetreenode(), "NearMessage");
    Message::getTypetreenode()->addChildNode(NearMessage::getTypetreenode());
  }
  this->setSender(NULL);
}

// Destructing
NearMessage::~NearMessage() {
}

// Sending
void NearMessage::sendTo(Agent * receiver) {
  NearMessage * newMessage = new NearMessage(*this);
  receiver->saveMessage(newMessage);
}

// Testing
bool NearMessage::isA(string type) {
  return NearMessage::getTypetreenode()->isA(type);
}
