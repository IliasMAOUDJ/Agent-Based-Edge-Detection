#include "Agent.h"
#include "Message.h"
#include "TypeTreeNode.h"

TypeTreeNode * Agent::agenttreenode = NULL;

// Accessing
set<Agent *> Agent::getAgents() {
  set<Agent *> agents;
  Agent::getTypetreenode()->getAgents(agents);
  return agents;
}
queue<Message *> & Agent::getMessages() {
  return messages;
}
string Agent::getName() {
  return (name);
}
int Agent::getNbMessage() {
  return this->messages.size();
}
Message * Agent::getNextMessage() {
  if (this->getNbMessage() > 0) {
    Message * nextMessage = this->getMessages().front();
    this->getMessages().pop();
    if(!Agent::exist(nextMessage->getSender())) {
      nextMessage->setSender(NULL);
    }
    return nextMessage;
  }
  else {
    return NULL;
  }
}
string Agent::getType() {
  return Agent::getTypetreenode()->getType();
}
TypeTreeNode * Agent::getTypetreenode() {
  if (!agenttreenode) {
    agenttreenode = new TypeTreeNode();
    agenttreenode->initialize(NULL, "Agent");
  }
  return agenttreenode;
}
void Agent::setName(string s) {
  name = s;
}

// Constructing
Agent::Agent() {
  if (!agenttreenode) {
    agenttreenode = new TypeTreeNode();
    agenttreenode->initialize(NULL, "Agent");
  }
}

// Destructing
Agent::~Agent() {
}

// Erasing
void Agent::eraseAgents() {
  Agent::getTypetreenode()->eraseAgents();
}

// Handling Messages
void Agent::broadcast(Message * message){
  message->setSender(this);
  
  Message::broadcast(message);
}
void Agent::saveMessage(Message * message){
  this->getMessages().push(message);
}
void Agent::sendTo(Agent * agent, Message * message){
  if (Agent::exist(agent)) {
    message->setSender(this);
    message->sendTo(agent);
    delete message;	
  }
}
void Agent::subscribe(TypeTreeNode * node) {
  node->addAgent(this);
}
void Agent::unsubscribe(TypeTreeNode * node) {
  node->removeAgent(this);
}

// Testing
bool Agent::exist(Agent * agent) {
  if (agent == NULL) {
    return false;
  }
  else {
    return Agent::getTypetreenode()->exist(agent);
  } 
}
bool Agent::isA(string type) {
  return (Agent::getTypetreenode()->isA(type));
}
