#include "AckMessage.h"
#include "Atelier.h"
#include "ConfirmMessage.h"
#include "EndMessage.h"
#include "HolonMessage.h"
#include "Machine.h"
#include "NoMessage.h"
#include "ReadyMessage.h"
#include "TypeTreeNode.h"
#include "Utilities.h"

#include <sstream>

using namespace std;

TypeTreeNode * Machine::machinetreenode = NULL;

// Accessing
set<Agent *> Machine::getAgents() {
  set<Agent *> agents;
  Machine::getTypetreenode()->getAgents(agents);
  return agents;
}
TypeTreeNode * Machine::getTypetreenode() {
  if (!machinetreenode) {
    machinetreenode = new TypeTreeNode();
    machinetreenode->initialize(Holon::getTypetreenode(), "Machine");
  }
  return machinetreenode;
}
string Machine::getType() {
  return Machine::getTypetreenode()->getType();
}

// Constructing
Machine::Machine():Holon() {
  Holon::getTypetreenode()->addChildNode(Machine::getTypetreenode());
}
void Machine::create(int number) {
  for (int i=0; i<number; i++) {
    Machine * machine = new Machine();
    string s;
    stringstream ss;
    
    ss << "Machine" << Machine::getTypetreenode()->getNumber();
    ss >> s;
    machine->setName(s);
    Machine::getTypetreenode()->addAgent(machine);
  }
}
void Machine::create(Atelier* atelier){
  Machine * machine = new Machine();
  string s;
  stringstream ss;
  
  ss << "Machine" << Machine::getTypetreenode()->getNumber();
  ss >> s;
  machine->setName(s);
  Machine::getTypetreenode()->addAgent(machine);
	
  double x; double y;
  machine->_atelier = atelier;
  machine->initLocation(atelier,x,y);
  machine->setLocation(x,y,0);
  machine->attachTo(*machine->_atelier);
  machine->on();
}

// Destructing
Machine::~Machine() {}

// Erasing
void Machine::eraseAgents() {
  Machine::getTypetreenode()->eraseAgents();
}

// Events handling
void Machine::keyPress(string key) {
  if (!key.compare("F1")) event_F1();
  else if (!key.compare("F2")) event_F2();
  else if ((!key.compare("H")) || (!key.compare("h"))) event_H();
}
void Machine::event_F1() {
  cout << this->getName() << " : ";
  if (_hs) cout << "HS" << endl;
  else cout << "OK" << endl;
}
void Machine::event_F2() {
  if (_hs) on();
  else off();
}
void Machine::event_H() {
  cout << endl;
  cout << "-----------------------------------" << endl;
  cout << "Touches disponibles sur une Machine:" << endl;
  cout << "-----------------------------------" << endl;
  cout << "F1 : Donne l'etat d'une machine" << endl;
  cout << "F2 : Change l'etat (on/off)" << endl;
  cout << "h/H : Help ! " << endl;
  cout << "-----------------------------------" << endl;
}

// Handling Messages
void Machine::setSensible() {} 
void Machine::unsetSensible() {}

// Initializing
void Machine::initLocation(Atelier* atelier, double& x,double& y){
  srand(getTime());
  
  double hauteur = atelier->_size/20;
  double largeur = atelier->_size/10;
  double sizeZoneCreationH = atelier->_size - hauteur - 1;
  double sizeZoneCreationL = atelier->_size - largeur - 1;
  
  x = realRand()*sizeZoneCreationL - sizeZoneCreationL/2;
  y = realRand()*sizeZoneCreationH - sizeZoneCreationH/2;	
}

// Living
void Machine::changeShape(int i){
  double hauteur = _atelier->_size/20;
  double largeur = _atelier->_size/10;
  
  double x1[] = {-largeur/2, -largeur/2, largeur/2, largeur/2, largeur/2, -largeur/2};
  double y1[] = {-hauteur/2, hauteur/2, -hauteur/2, hauteur/2, hauteur/2, -hauteur/2};
  
  double x2[] = {-largeur/2, -largeur/6, 0, largeur/6, largeur/2, largeur/2, -largeur/2};
  double y2[] = {hauteur/2, -hauteur/2, hauteur/2, -hauteur/2, hauteur/2, -hauteur/2, -hauteur/2};
	
  switch(i){
  case 0:
    this->rectangle(largeur, hauteur, 1);
    break;
  case 1:
    this->rectangle(largeur, hauteur, 0);
    break;
  case 2:
    this->polygon(6, x1, y1, 0);
    break;
  case 3:
    this->polygon(7, x2, y2, 1);
    break;
  }
}
void Machine::live() {	
  if (!_hs) {
    if(_currentState == INIT)    { _currentState = init();    return; }
    if(_currentState == CHOICE)  { _currentState = choice();  return; }
    if(_currentState == CONFIRM) { _currentState = confirm(); return; }
    if(_currentState == TRANSIT) { _currentState = transit(); return; }
    if(_currentState == READY)   { _currentState = ready();   return; }
    if(_currentState == BUSY)    { _currentState = busy();    return; }
  }
}
void Machine::on() {
  _hs = false;
  changeShape(0);
  setSensible();
  _currentState = INIT;
}
void Machine::off(void) {
  _hs = true;
  changeShape(2);
  unsetSensible();
}
void Machine::workOn(void) {
  _timeout = getTime();
  srand(getTime());
  _workingTime = integerRand(WORKINGTIME);
  changeShape(3);
}

int Machine::init(){
  int result = INIT;
  
  _currentPiece     = NULL;
  _currentPieceName = "";  
  changeShape(0); 
  clearMailBox();
  setSensible();       
  result = CHOICE;
  
  return result;
}
int Machine::choice() {
  return 0;
}
int Machine::confirm() {
  int result = CONFIRM;
	
  queue<HolonMessage *> correctMessages;
  
  while (getNbMessage()) {
    HolonMessage * message = (HolonMessage*)this->getNextMessage();
    if (message->isA("ConfirmMessage")) {
      correctMessages.push(message);
    }
  }
  
  if (correctMessages.size()) {
    bool reserved = false;
    while(correctMessages.size()) {
      HolonMessage * nextMessage = correctMessages.front();
      correctMessages.pop();
      Agent * piece = nextMessage->getSender();
      string pieceName;
      
      if (piece) pieceName = piece->getName();
      
      if (!reserved) {
	_currentPiece = piece;
	_currentPieceName = pieceName;
	reserved = true;
                       
	ConfirmMessage * confirmMessage = new ConfirmMessage();
	this->sendTo(_currentPiece, confirmMessage);
      }
      else {
	NoMessage * noMessage = new NoMessage();
	this->sendTo(piece, noMessage);
      }
    }
	
    changeShape(1);
    _timeout = getTime();
    result  = TRANSIT;
  }
  else {
    if (getTime() - _timeout > CONFIRMTIMEOUT) {
      result = INIT;
    }
  }
  return result;
}
int Machine::transit() {
  int result = TRANSIT;
  queue<HolonMessage *> correctMessages;
	
  while (getNbMessage()) {
    HolonMessage * message = (HolonMessage *)getNextMessage();
    if ((message->isA("AckMessage")) || (message->isA("NearMessage"))) {
      if ((message->getSender()) && (!message->getSender()->getName().compare(_currentPieceName))) {
	correctMessages.push(message);
      }
    }
  }	  			  
  
  if (correctMessages.size()) {
    HolonMessage * nextMessage = correctMessages.front();
    correctMessages.pop();
    if (nextMessage->isA("AckMessage")) {
      AckMessage* ackMessage = new AckMessage();
      this->sendTo(_currentPiece, ackMessage);
      _timeout = getTime();
      result  = TRANSIT;
    }
    else {
      ReadyMessage * readyMessage = new ReadyMessage();
      this->sendTo(_currentPiece, readyMessage);
      _timeout = getTime();
      result  = READY;
    }
  }
  else {		
    if (getTime() - _timeout > ACKTIMEOUT) {
      result = INIT;
    }
  }
  return result;
}		
int Machine::ready() {
  int result = READY;
  queue<HolonMessage *> correctMessages;

  while (getNbMessage()) {
    HolonMessage * message = (HolonMessage *)getNextMessage();
    if (message->isA("ReadyMessage")) {
      if ((message->getSender()) && (!message->getSender()->getName().compare(_currentPieceName))) {
	correctMessages.push(message);
      }
    }
  }
    
  if (correctMessages.size()) {
    workOn();
    result = BUSY;
  }
  else {
    if (getTime() - _timeout > READYTIMEOUT) {
      result = INIT;
    }
  }
  return result;
}

int Machine::busy() {
  int result = BUSY;
  
  if(getTime() - _timeout > _workingTime) {
    EndMessage * endMessage = new EndMessage();
    this->sendTo(_currentPiece, endMessage);
    result = INIT;
  }
  return result;
}

// Testing
bool Machine::isA(string type) {
  return (Machine::getTypetreenode()->isA(type));
}
