#ifndef _TYPETREENODE_H
#define _TYPETREENODE_H

#include <iostream>
#include <set>
#include <string>

using namespace std;

class Agent;

class TypeTreeNode {
 private:
  int number;
  set<Agent *> agentset;
  set<TypeTreeNode *> childrenNodes;
  TypeTreeNode * parentNode;
  string type;
  
 public:
  // Accessing
  void getAgents(set<Agent *> &);
  set<Agent *> & getAgentset();
  set<TypeTreeNode *> & getChildrenNodes();
  int getNumber();
  TypeTreeNode * getParentNode();
  string getType();

  // Adding
  void addAgent(Agent *);
  void addChildNode(TypeTreeNode *);

  // Constructing
  TypeTreeNode();

  // Destructing
  ~TypeTreeNode();

  // Erasing
  void removeAgent(Agent * agent);
  void eraseAgent(Agent *);
  void eraseAgents();

  // Finding
  TypeTreeNode * findNode(string);

  // Initializing
  void initialize(TypeTreeNode *, string);

  // Printing
  void print();

  // Testing
  bool isA(string);
  bool exist(Agent* agent);
  //bool TypeTreeNode::isNil();
};

#endif
