#ifndef _AGENT_H
#define _AGENT_H

#include <map>
#include <queue>
#include <set>
#include <string>

using namespace std;

class Message;
class TypeTreeNode;

class Agent {

 private:
  queue<Message *> messages;
  string name;
  static TypeTreeNode * agenttreenode;

 public:
  // Accessing
  static set<Agent *> getAgents();
  static TypeTreeNode * getTypetreenode();
  queue<Message *> & getMessages();
  string getName();
  int getNbMessage();
  Message * getNextMessage();
  virtual string getType();
  void setName(string);

  // Constructing
  Agent();

  // Destructing
  virtual ~Agent();

  // Erasing
  static void eraseAgents();

  // Handling Messages
  void broadcast(Message *);
  void saveMessage(Message *);
  void sendTo(Agent *, Message *);
  void subscribe(TypeTreeNode *);
  void unsubscribe(TypeTreeNode *);

  // Living
  virtual void live() = 0;

  // Testing
  static bool exist(Agent *);
  virtual bool isA(string);
};

#endif
