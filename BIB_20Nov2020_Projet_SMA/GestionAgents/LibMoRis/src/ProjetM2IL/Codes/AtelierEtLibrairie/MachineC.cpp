#include "Atelier.h"
#include "CRequest.h"
#include "MachineC.h"
#include "TypeTreeNode.h"
#include "Utilities.h"
#include "YesMessage.h"

#include <sstream>

using namespace std;

TypeTreeNode * MachineC::machinectreenode = NULL;

// Accessing
set<Agent *> MachineC::getAgents() {
  set<Agent *> agents;
  MachineC::getTypetreenode()->getAgents(agents);
  return agents;
}
TypeTreeNode * MachineC::getTypetreenode() {
  if (!machinectreenode) {
    machinectreenode = new TypeTreeNode();
    machinectreenode->initialize(Machine::getTypetreenode(), "MachineC");
  }
  return machinectreenode;
}
string MachineC::getType() {
  return MachineC::getTypetreenode()->getType();
}

// Constructing
MachineC::MachineC():Machine() {
  Machine::getTypetreenode()->addChildNode(MachineC::getTypetreenode());
}
void MachineC::create(int number) {
  for (int i=0; i<number; i++) {
    MachineC * machine = new MachineC();
    string s;
    stringstream ss;
    
    ss << "MachineC" << MachineC::getTypetreenode()->getNumber();
    ss >> s;
    machine->setName(s);
    MachineC::getTypetreenode()->addAgent(machine);
  }
}
void MachineC::create(Atelier * atelier){
  MachineC * machine = new MachineC();
  string s;
  stringstream ss;
  
  ss << "MachineC" << MachineC::getTypetreenode()->getNumber();
  ss >> s;
  machine->setName(s);
  MachineC::getTypetreenode()->addAgent(machine);
  
  double x; double y;
  machine->_atelier = atelier;
  machine->initLocation(atelier,x,y);
  machine->setLocation(x,y,0);
  machine->attachTo(*machine->_atelier);
  machine->on();
  
  machine->setColor("green");
}

// Destructing
MachineC::~MachineC() {}

// Erasing
void MachineC::eraseAgents() {
  MachineC::getTypetreenode()->eraseAgents();
}

// Handling Messages
void MachineC::setSensible() {
  _isSensible = true;
  subscribe(CRequest::getTypetreenode());
}
void MachineC::unsetSensible() {
  _isSensible = false;
  unsubscribe(CRequest::getTypetreenode());
}

// Living
int  MachineC::choice(){
  int result = CHOICE;
  
  queue<HolonMessage *> correctMessages;
  
  while (getNbMessage()) {
    HolonMessage* message = (HolonMessage*)this->getNextMessage();
    if (message->isA("CRequest")){
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
void MachineC::live() {
  this->Machine::live();
}

// Testing
bool MachineC::isA(string type){
  return (MachineC::getTypetreenode()->isA(type));
}
