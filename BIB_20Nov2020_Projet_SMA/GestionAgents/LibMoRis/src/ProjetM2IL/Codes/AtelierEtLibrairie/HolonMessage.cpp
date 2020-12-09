#include "Agent.h"
#include "HolonMessage.h"
#include "TypeTreeNode.h"

TypeTreeNode * HolonMessage::holonmessagetreenode = NULL;

// Accessing
set<Agent *> HolonMessage::getSubscribers() {
  set<Agent *> subscribers;
  HolonMessage::getTypetreenode()->getAgents(subscribers);
  return subscribers;
}
string HolonMessage::getType() {
  return HolonMessage::getTypetreenode()->getType();
}
TypeTreeNode * HolonMessage::getTypetreenode() {
  if (!holonmessagetreenode) {
    holonmessagetreenode = new TypeTreeNode();
    holonmessagetreenode->initialize(Message::getTypetreenode(), "HolonMessage");
    Message::getTypetreenode()->addChildNode(HolonMessage::getTypetreenode());
  }
  return holonmessagetreenode;
}

// Broadcasting
void HolonMessage::broadcast(Message * message) {
  set<Agent *> subscribers = HolonMessage::getSubscribers();
  for (set<Agent *>::iterator it = subscribers.begin(); it != subscribers.end(); it++) {
    if (Agent::exist(*it)) {
      message->sendTo(*it);
    }
  }
  delete message;
}

// Constructing
HolonMessage::HolonMessage():Message() {
  if (!holonmessagetreenode) {
    holonmessagetreenode = new TypeTreeNode();
    holonmessagetreenode->initialize(Message::getTypetreenode(), "HolonMessage");
    Message::getTypetreenode()->addChildNode(HolonMessage::getTypetreenode());
  }
  this->setSender(NULL);
}

// Destructing
HolonMessage::~HolonMessage() {
}

// Sending
void HolonMessage::sendTo(Agent * receiver) {
  HolonMessage * newMessage = new HolonMessage(*this);
  receiver->saveMessage(newMessage);
}

// Testing
bool HolonMessage::isA(string type) {
  return HolonMessage::getTypetreenode()->isA(type);
}
