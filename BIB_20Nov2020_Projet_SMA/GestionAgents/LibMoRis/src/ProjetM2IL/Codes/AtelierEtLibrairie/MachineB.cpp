#include "Atelier.h"
#include "BRequest.h"
#include "MachineB.h"
#include "TypeTreeNode.h"
#include "Utilities.h"
#include "YesMessage.h"

#include <sstream>

using namespace std;

TypeTreeNode * MachineB::machinebtreenode = NULL;

// Accessing
set<Agent *> MachineB::getAgents() {
  set<Agent *> agents;
  MachineB::getTypetreenode()->getAgents(agents);
  return agents;
}
TypeTreeNode * MachineB::getTypetreenode() {
  if (!machinebtreenode) {
    machinebtreenode = new TypeTreeNode();
    machinebtreenode->initialize(Machine::getTypetreenode(), "MachineB");
  }
  return machinebtreenode;
}
string MachineB::getType() {
  return MachineB::getTypetreenode()->getType();
}

// Constructing
MachineB::MachineB():Machine() {
  Machine::getTypetreenode()->addChildNode(MachineB::getTypetreenode());
}

void MachineB::create(int number) {
  for (int i=0; i<number; i++) {
    MachineB * machine = new MachineB();
    string s;
    stringstream ss;

    ss << "MachineB" << MachineB::getTypetreenode()->getNumber();
    ss >> s;
    machine->setName(s);
    MachineB::getTypetreenode()->addAgent(machine);
  }
}

void MachineB::create(Atelier* atelier){
  MachineB * machine = new MachineB();
  string s;
  stringstream ss;
  
  ss << "MachineB" << MachineB::getTypetreenode()->getNumber();
  ss >> s;
  machine->setName(s);
  MachineB::getTypetreenode()->addAgent(machine);
  
  double x; double y;
  machine->_atelier = atelier;
  machine->initLocation(atelier,x,y);
  machine->setLocation(x,y,0);
  machine->attachTo(*machine->_atelier);
  machine->on();

  machine->setColor("blue");
}

// Destructing
MachineB::~MachineB() {}

// Erasing
void MachineB::eraseAgents() {
  MachineB::getTypetreenode()->eraseAgents();
}

// Handling Messages
void MachineB::setSensible() {
  _isSensible = true;
  subscribe(BRequest::getTypetreenode());
}
void MachineB::unsetSensible() {
  _isSensible = false;
  unsubscribe(BRequest::getTypetreenode());
}

// Living
int  MachineB::choice(){
  int result = CHOICE;
  
  queue<HolonMessage *> correctMessages;
  
  while (getNbMessage()) {
    HolonMessage* message = (HolonMessage*)this->getNextMessage();
    if (message->isA("BRequest")){
      correctMessages.push(message);
    }
  }
  
  
  if (correctMessages.size()) {    
    while(correctMessages.size())
      {
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
void MachineB::live() {
  this->Machine::live();
}

// Testing
bool MachineB::isA(string type) {
  return (MachineB::getTypetreenode()->isA(type));
}
