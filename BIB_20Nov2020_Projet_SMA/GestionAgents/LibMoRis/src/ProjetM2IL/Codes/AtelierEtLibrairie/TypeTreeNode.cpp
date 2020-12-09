#include "Agent.h"
#include "TypeTreeNode.h"

// Constructing
TypeTreeNode::TypeTreeNode() {
  number = 1;
  parentNode = NULL;
  type = "";
}

// Destructing
TypeTreeNode::~TypeTreeNode() {
  delete parentNode;
}

// Accessing
void TypeTreeNode::getAgents(set<Agent *> & agentsCollection) {
  for (set<Agent *>::iterator it = this->getAgentset().begin(); it != this->getAgentset().end(); it++) {
    agentsCollection.insert(agentsCollection.end(), (*it));
  }
  for (set<TypeTreeNode *>::iterator it = this->getChildrenNodes().begin(); it != this->getChildrenNodes().end(); it++) {
    (*it)->getAgents(agentsCollection);
  }
}
set<Agent *> & TypeTreeNode::getAgentset() {
  return agentset;
}
set<TypeTreeNode *> & TypeTreeNode::getChildrenNodes() {
  return childrenNodes;
}
int TypeTreeNode::getNumber() {
  return number;
}
TypeTreeNode * TypeTreeNode::getParentNode() {
  return parentNode;
}
string TypeTreeNode::getType() {
  return type;
}

// Adding
void TypeTreeNode::addAgent(Agent * agent) {
  number++;
  this->getAgentset().insert(this->getAgentset().begin(), agent);
}
void TypeTreeNode::addChildNode(TypeTreeNode * node) {
  this->getChildrenNodes().insert(childrenNodes.end(), node);
}

// Erasing

void TypeTreeNode::removeAgent(Agent * agent) {
  this->getAgentset().erase(agent);
}

void TypeTreeNode::eraseAgent(Agent * agent) {
  set<Agent *>::iterator it = this->getAgentset().find(agent);
  if (it != this->getAgentset().end()) {
    this->getAgentset().erase(it);
    delete agent;
    agent = NULL;
  }
  else {
    cout << "Echec de la suppression : Agent non trouve" << endl;
  }
}
void TypeTreeNode::eraseAgents() {
	for(set<Agent *>::iterator it = this->getAgentset().begin(); it != this->getAgentset().end(); it++){
		delete (*it);
	}
  this->getAgentset().clear(); 
  for (set<TypeTreeNode *>::iterator it = this->getChildrenNodes().begin(); it != this->getChildrenNodes().end(); it++) {
    (*it)->eraseAgents();
  }
}

// Finding
TypeTreeNode * TypeTreeNode::findNode(string s) {
  cout << "Recherche : " << endl;
  this->print();
  cout << endl;
  if (this->getType().compare(s) == 0) {
    return (this);
  }
  else if (this->getChildrenNodes().size() == 0) {
    return (NULL);
  }
  else {
    set<TypeTreeNode *> nodes = this->getChildrenNodes();
    set<TypeTreeNode *>::iterator it = nodes.begin();
    TypeTreeNode * result;
    while ((it != nodes.end()) && (!result)) {
      result = (*it)->findNode(s);
    }
    return (result);
  }
}

// Initializing
void TypeTreeNode::initialize(TypeTreeNode * parent, string t) {
  parentNode = parent;
  type = t;
}

// Printing
void TypeTreeNode::print() {
  cout << "  Noeud : " << this->getType() << endl;
  if (this->getParentNode()) {
    cout << "    Parent  : " << this->getParentNode()->getType() << endl;
  }
  else {
    cout << "    Parent  : NULL" << endl;
  }
  cout << "    Enfants : ";
  for (set<TypeTreeNode *>::iterator it = this->getChildrenNodes().begin(); it != this->getChildrenNodes().end(); it++) {
    cout << (*it)->getType();
  }
  cout << endl;
  cout << "    Agents  : ";
  for (set<Agent *>::iterator it = this->getAgentset().begin(); it != this->getAgentset().end(); it++) {
    cout << (*it)->getName() << " ";
  }
  cout << endl;
}

// Testing
bool TypeTreeNode::exist(Agent* agent){
  bool found = false;
  set<Agent *>::iterator it = this->getAgentset().find(agent);

  if (it != this->getAgentset().end()) {
    found = true;
  }
  else {
    set<TypeTreeNode *>::iterator it = this->getChildrenNodes().begin();
    while ((it != this->getChildrenNodes().end()) && (found != true)) {
      found = (*it)->exist(agent);
      it++;
    }
  }

  return found;
}
bool TypeTreeNode::isA(string s) {
  if (s.compare(type) == 0) {
    return true;
  }
  else if (!this->getParentNode()) {
    return false;
  }
  else {
	  return (this->getParentNode()->isA(s));
  }
}
