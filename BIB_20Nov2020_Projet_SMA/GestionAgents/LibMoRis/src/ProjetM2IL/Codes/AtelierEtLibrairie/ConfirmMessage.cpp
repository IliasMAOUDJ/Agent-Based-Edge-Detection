#include "ConfirmMessage.h"
#include "Agent.h"
#include "TypeTreeNode.h"

TypeTreeNode * ConfirmMessage::confirmmessagetreenode = NULL;

// Accessing
set<Agent *> ConfirmMessage::getSubscribers() {
  set<Agent *> subscribers;
  ConfirmMessage::getTypetreenode()->getAgents(subscribers);
  return subscribers;
}
string ConfirmMessage::getType() {
  return ConfirmMessage::getTypetreenode()->getType();
}
TypeTreeNode * ConfirmMessage::getTypetreenode() {
  if (!confirmmessagetreenode) {
    confirmmessagetreenode = new TypeTreeNode();
    confirmmessagetreenode->initialize(Message::getTypetreenode(), "ConfirmMessage");
    Message::getTypetreenode()->addChildNode(ConfirmMessage::getTypetreenode());
  }
  return confirmmessagetreenode;
}

// Broadcasting
void ConfirmMessage::broadcast(Message * message) {
  set<Agent *> subscribers = ConfirmMessage::getSubscribers();
  for (set<Agent *>::iterator it = subscribers.begin(); it != subscribers.end(); it++) {
    if (Agent::exist(*it)) {
      message->sendTo(*it);
    }
  }
  delete message;
}

// Constructing
ConfirmMessage::ConfirmMessage():HolonMessage() {
  if (!confirmmessagetreenode) {
    confirmmessagetreenode = new TypeTreeNode();
    confirmmessagetreenode->initialize(Message::getTypetreenode(), "ConfirmMessage");
    Message::getTypetreenode()->addChildNode(ConfirmMessage::getTypetreenode());
  }
  this->setSender(NULL);
}

// Destructing
ConfirmMessage::~ConfirmMessage() {
}

// Sending
void ConfirmMessage::sendTo(Agent * receiver) {
  ConfirmMessage * newMessage = new ConfirmMessage(*this);
  receiver->saveMessage(newMessage);
}

// Testing
bool ConfirmMessage::isA(string type) {
  return ConfirmMessage::getTypetreenode()->isA(type);
}
