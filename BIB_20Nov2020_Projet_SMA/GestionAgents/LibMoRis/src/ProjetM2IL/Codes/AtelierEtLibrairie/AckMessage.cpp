#include "AckMessage.h"
#include "Agent.h"
#include "TypeTreeNode.h"

TypeTreeNode * AckMessage::ackmessagetreenode = NULL;

// Accessing
set<Agent *> AckMessage::getSubscribers() {
  set<Agent *> subscribers;
  AckMessage::getTypetreenode()->getAgents(subscribers);
  return subscribers;
}

string AckMessage::getType() {
  return AckMessage::getTypetreenode()->getType();
}

TypeTreeNode * AckMessage::getTypetreenode() {
  if (!ackmessagetreenode) {
    ackmessagetreenode = new TypeTreeNode();
    ackmessagetreenode->initialize(Message::getTypetreenode(), "AckMessage");
    Message::getTypetreenode()->addChildNode(AckMessage::getTypetreenode());
  }
  return ackmessagetreenode;
}

// Broadcasting
void AckMessage::broadcast(Message * message) {
  set<Agent *> subscribers = AckMessage::getSubscribers();
  for (set<Agent *>::iterator it = subscribers.begin(); it != subscribers.end(); it++) {
    if (Agent::exist(*it)) {
      message->sendTo(*it);
    }
  }
  delete message;
}

// Constructing
AckMessage::AckMessage():HolonMessage() {
  if (!ackmessagetreenode) {
    ackmessagetreenode = new TypeTreeNode();
    ackmessagetreenode->initialize(Message::getTypetreenode(), "AckMessage");
    Message::getTypetreenode()->addChildNode(AckMessage::getTypetreenode());
  }
  this->setSender(NULL);
}

// Destructing
AckMessage::~AckMessage() {
}

// Sending
void AckMessage::sendTo(Agent * receiver) {
  AckMessage * newMessage = new AckMessage(*this);
  receiver->saveMessage(newMessage);
}

// Testing
bool AckMessage::isA(string type) {
  return AckMessage::getTypetreenode()->isA(type);
}
