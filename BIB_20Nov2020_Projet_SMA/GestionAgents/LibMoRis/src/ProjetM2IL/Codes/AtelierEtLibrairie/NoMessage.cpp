#include "Agent.h"
#include "NoMessage.h"
#include "TypeTreeNode.h"

TypeTreeNode * NoMessage::nomessagetreenode = NULL;

// Accessing
set<Agent *> NoMessage::getSubscribers() {
  set<Agent *> subscribers;
  NoMessage::getTypetreenode()->getAgents(subscribers);
  return subscribers;
}
string NoMessage::getType() {
  return NoMessage::getTypetreenode()->getType();
}
TypeTreeNode * NoMessage::getTypetreenode() {
  if (!nomessagetreenode) {
    nomessagetreenode = new TypeTreeNode();
    nomessagetreenode->initialize(Message::getTypetreenode(), "NoMessage");
    Message::getTypetreenode()->addChildNode(NoMessage::getTypetreenode());
  }
  return nomessagetreenode;
}

// Broadcasting
void NoMessage::broadcast(Message * message) {
  set<Agent *> subscribers = NoMessage::getSubscribers();
  for (set<Agent *>::iterator it = subscribers.begin(); it != subscribers.end(); it++) {
    if (Agent::exist(*it)) {
      message->sendTo(*it);
    }
  }
  delete message;
}

// Constructing
NoMessage::NoMessage():HolonMessage() {
  if (!nomessagetreenode) {
    nomessagetreenode = new TypeTreeNode();
    nomessagetreenode->initialize(Message::getTypetreenode(), "NoMessage");
    Message::getTypetreenode()->addChildNode(NoMessage::getTypetreenode());
  }
  this->setSender(NULL);
}

// Destructing
NoMessage::~NoMessage() {
}

// Sending
void NoMessage::sendTo(Agent * receiver) {
  NoMessage * newMessage = new NoMessage(*this);
  receiver->saveMessage(newMessage);
}

// Testing
bool NoMessage::isA(string type) {
  return NoMessage::getTypetreenode()->isA(type);
}
