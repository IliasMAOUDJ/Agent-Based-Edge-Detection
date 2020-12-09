#include "Atelier.h"
#include "ARequest.h"
#include "MachineA.h"
#include "TypeTreeNode.h"
#include "Utilities.h"
#include "YesMessage.h"

#include <sstream>

using namespace std;

TypeTreeNode * MachineA::machineatreenode = NULL;

// Accessing
set<Agent *> MachineA::getAgents() {
  set<Agent *> agents;
  MachineA::getTypetreenode()->getAgents(agents);
  return agents;
}
TypeTreeNode * MachineA::getTypetreenode() {
  if (!machineatreenode) {
    machineatreenode = new TypeTreeNode();
    machineatreenode->initialize(Machine::getTypetreenode(), "MachineA");
  }
  return machineatreenode;
}
string MachineA::getType() {
  return MachineA::getTypetreenode()->getType();
}

// Constructing
MachineA::MachineA():Machine() {
  Machine::getTypetreenode()->addChildNode(MachineA::getTypetreenode());
}
void MachineA::create(int number) {
  for (int i=0; i<number; i++) {
    MachineA * machine = new MachineA();
    string s;
    stringstream ss;
    
    ss << "MachineA" << MachineA::getTypetreenode()->getNumber();
    ss >> s;
    machine->setName(s);
    MachineA::getTypetreenode()->addAgent(machine);
  }
}
void MachineA::create(Atelier* atelier){
  MachineA * machine = new MachineA();
  string s;
  stringstream ss;
  
  ss << "MachineA" << MachineA::getTypetreenode()->getNumber();
  ss >> s;
  machine->setName(s);
  MachineA::getTypetreenode()->addAgent(machine);
  
  double x; double y;
  machine->_atelier = atelier;
  machine->initLocation(atelier,x,y);
  machine->setLocation(x,y,0);
  machine->attachTo(*machine->_atelier);
  machine->on();
  
  machine->setColor("orange");
}

// Destructing
MachineA::~MachineA() {}

// Erasing
void MachineA::eraseAgents() {
  MachineA::getTypetreenode()->eraseAgents();
}

// Handling Messages
void MachineA::setSensible() {
  _isSensible = true;
  subscribe(ARequest::getTypetreenode());
}
void MachineA::unsetSensible() {
  _isSensible = false;
  unsubscribe(ARequest::getTypetreenode());
}

// Living
int MachineA::choice() {
  int result = CHOICE;
  
  queue<HolonMessage *> correctMessages;
  
  while (getNbMessage()) {
    HolonMessage* message = (HolonMessage*)this->getNextMessage();
    if (message->isA("ARequest")){
      correctMessages.push(message);
    }
  }
  
  if (correctMessages.size()) {
    while(correctMessages.size()) {
      HolonMessage* nextMessage = correctMessages.front();
      correctMessages.pop();
      Agent * piece = nextMessage->getSender();
      YesMessage * yesMessage = new YesMessage();
      this->sendTo(piece, yesMessage);
    }

    this->unsetSensible();
    _timeout = getTime();
    result  = CONFIRM;
  }
  return result;
}
void MachineA::live() {
  this->Machine::live();
}

// Testing
bool MachineA::isA(string type) {
  return (MachineA::getTypetreenode()->isA(type));
}
