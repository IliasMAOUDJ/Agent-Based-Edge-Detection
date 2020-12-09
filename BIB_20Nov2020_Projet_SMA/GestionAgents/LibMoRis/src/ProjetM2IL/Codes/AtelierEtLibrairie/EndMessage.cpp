#include "EndMessage.h"
#include "Agent.h"
#include "TypeTreeNode.h"

TypeTreeNode * EndMessage::endmessagetreenode = NULL;

// Accessing
set<Agent *> EndMessage::getSubscribers() {
  set<Agent *> subscribers;
  EndMessage::getTypetreenode()->getAgents(subscribers);
  return subscribers;
}
string EndMessage::getType() {
  return EndMessage::getTypetreenode()->getType();
}
TypeTreeNode * EndMessage::getTypetreenode() {
  if (!endmessagetreenode) {
    endmessagetreenode = new TypeTreeNode();
    endmessagetreenode->initialize(Message::getTypetreenode(), "EndMessage");
    Message::getTypetreenode()->addChildNode(EndMessage::getTypetreenode());
  }
  return endmessagetreenode;
}

// Broadcasting
void EndMessage::broadcast(Message * message) {
  set<Agent *> subscribers = EndMessage::getSubscribers();
  for (set<Agent *>::iterator it = subscribers.begin(); it != subscribers.end(); it++) {
    if (Agent::exist(*it)) {
      message->sendTo(*it);
    }
  }
  delete message;
}

// Constructing
EndMessage::EndMessage():HolonMessage() {
  if (!endmessagetreenode) {
    endmessagetreenode = new TypeTreeNode();
    endmessagetreenode->initialize(Message::getTypetreenode(), "EndMessage");
    Message::getTypetreenode()->addChildNode(EndMessage::getTypetreenode());
  }
  this->setSender(NULL);
}

// Destructing
EndMessage::~EndMessage() {
}

// Sending
void EndMessage::sendTo(Agent * receiver) {
  EndMessage * newMessage = new EndMessage(*this);
  receiver->saveMessage(newMessage);
}

// Testing
bool EndMessage::isA(string type) {
  return EndMessage::getTypetreenode()->isA(type);
}
