#include "AckMessage.h"
#include "ARequest.h"
#include "Atelier.h"
#include "BRequest.h"
#include "ConfirmMessage.h"
#include "CRequest.h"
#include "Input.h"
#include "HolonMessage.h"
#include "NearMessage.h"
#include "Output.h"
#include "Piece.h"
#include "ReadyMessage.h"
#include "TypeTreeNode.h"
#include "Utilities.h"

#include <sstream>

using namespace std;

TypeTreeNode * Piece::piecetreenode = NULL;

// Accessing
set<Agent *> Piece::getAgents() {
	set<Agent *> agents;
	Piece::getTypetreenode()->getAgents(agents);
	return agents;
}
TypeTreeNode * Piece::getTypetreenode() {
	if (!piecetreenode) {
		piecetreenode = new TypeTreeNode();
		piecetreenode->initialize(Holon::getTypetreenode(), "Piece");
	}
	return piecetreenode;
}
string Piece::getType() {
	return Piece::getTypetreenode()->getType();
}

// Constructing
Piece::Piece():Holon() {
	Holon::getTypetreenode()->addChildNode(Piece::getTypetreenode());
}
void Piece::create(int number) {
	for (int i=0; i<number; i++) {
	  Piece * piece = new Piece();
	  string s;
	  stringstream ss;
	  
	  ss << "Piece" << Piece::getTypetreenode()->getNumber();
	  ss >> s;
	  piece->setName(s);
	  Piece::getTypetreenode()->addAgent(piece);
	}
}
void Piece::create(Atelier* atelier) {
	Piece * piece = new Piece();
	string s;
	stringstream ss;
	
	ss << "Piece" << Piece::getTypetreenode()->getNumber();
	ss >> s;
	piece->setName(s);
	Piece::getTypetreenode()->addAgent(piece);

	// Placement
	piece->_atelier = atelier;
	piece->attachTo(*piece->_atelier);
	double x, y;
	atelier->_input->getXY(x, y);
	piece->setLocation(x, y, 0);
	
	piece->setColor("red");
	piece->initGamme();
	piece->_currentState = INIT;
	piece->changeShape(0);
}

// Destructing
Piece::~Piece() {}

// Erasing
void Piece::eraseAgents() {
	Piece::getTypetreenode()->eraseAgents();
}

// Initializing
void Piece::initGamme() {
	int nboperation = integerRand(5)+3;
	double proba;
	for(int n = 0; n < nboperation; n++){
		proba = realRand()*3;
		if(proba < 1) {
			_gamme.push("A");
		}
		else if(proba < 2){
			_gamme.push("B");
		}
		else{
			_gamme.push("C");
		}
	}
}

// Living
void Piece::changeShape(int i) {
	double hauteur = 0.66*_atelier->_size/20;
	double largeur = 0.66*_atelier->_size/10;
	
	double x[] = {-largeur/2, largeur/2, -largeur/2, 0};
	double y[] = {hauteur/2, 0, -hauteur/2, 0};
	
	switch(i){
	case 0:
		this->polygon(4, x, y, 0);
		break;
	}
}
void Piece::live() {
	this->_Kinematic(getTime());

	if(_currentState == INIT)        { _currentState = init();        return; }
	if(_currentState == CHOICE)      { _currentState = choice();      return; }
	if(_currentState == CONFIRM)     { _currentState = confirm();     return; }
	if(_currentState == TRANSIT)     { _currentState = transit();     return; }
	if(_currentState == READY)       { _currentState = ready();       return; }
	if(_currentState == BUSY)        { _currentState = busy();        return; }
	if(_currentState == END)         { _currentState = end();         return; }
	if(_currentState == EXIT)        {                 exit();        return; }
}
int Piece::init() { 
	int result = INIT;
	
	clearMailBox();
	
	_currentMachine     = NULL;
	_currentMachineName = "";
	
	if (_gamme.size()) {
		HolonMessage * message;
		string messageType = _gamme.front();
		
		if(!messageType.compare("A")) message = new ARequest();
		else if(!messageType.compare("B")) message = new BRequest();
		else if(!messageType.compare("C")) message = new CRequest();
		this->broadcast(message);
		_timeout = getTime();
		result   = CHOICE;
	}
	else result = EXIT;
	setLinearVelocity(0,0);
	setLinearAcceleration(0,0);
	return result;
}
int Piece::choice() {
	int result = CHOICE;
	
	queue<HolonMessage *> correctMessages;
	while (getNbMessage()) {
    HolonMessage * message = (HolonMessage *)this->getNextMessage();    
    if(message->isA("YesMessage")){
		 correctMessages.push(message);
    }
	}
	
	if (correctMessages.size()) { 
		double dMinimum = -1;
		while(correctMessages.size()) {
			HolonMessage * nextMessage = correctMessages.front();
			correctMessages.pop();
			Holon * machine = (Holon*)nextMessage->getSender();
			string machineName;
			if(Agent::exist(machine)) {
				machineName = machine->getName();
				double xEmitter, yEmitter;
				machine->getXY(xEmitter, yEmitter);
				
				double dMachine = distancePosition(xEmitter,yEmitter);
				if (dMachine!=-1) {
					if ((dMinimum==-1) || (dMinimum > dMachine)) {  
						dMinimum = dMachine;
						_currentMachine = machine;
						_currentMachineName = machineName;
					}
				}
			}
		}
		ConfirmMessage * confirmMessage = new ConfirmMessage();
		this->sendTo(_currentMachine, confirmMessage);
		_timeout = getTime();
		result = CONFIRM;
	}
	else { 
		if (getTime() - _timeout > YESTIMEOUT) { 
			result = INIT;
		}
	}
	return result;
}
int Piece::confirm() {
	int result = CONFIRM;
	
	queue<HolonMessage *> correctMessages;

	while (getNbMessage()) {
		HolonMessage * message = (HolonMessage *)this->getNextMessage();
		if ((message->isA("ConfirmMessage")) || (message->isA("NoMessage"))) {
			if ((message->getSender()) && (!message->getSender()->getName().compare(_currentMachineName))) {
				correctMessages.push(message);
			}
		}
	}
	
	if (correctMessages.size()) {
		HolonMessage * nextMessage = correctMessages.front();
		correctMessages.pop();
		
		if (nextMessage->isA("ConfirmMessage")) {
			AckMessage* ackMessage = new AckMessage();
			this->sendTo(_currentMachine, ackMessage);
			
			_timeout = getTime();
			result = TRANSIT;
		}
		else {
			result = INIT;
		}
	}
	else {
		if (getTime() - _timeout > CONFIRMTIMEOUT) { 
			result = INIT;
		}
	}
	return result;
}

int Piece::transit() {
	int result = TRANSIT;

	queue<HolonMessage *> correctMessages;

	while (getNbMessage()) {
		HolonMessage * message = (HolonMessage *)this->getNextMessage();

		if (message->isA("AckMessage")) {
			if ((message->getSender()) && (!message->getSender()->getName().compare(_currentMachineName))) {
				correctMessages.push(message);
			}
		}
	}

	if (correctMessages.size()) {
		HolonMessage * nextMessage = correctMessages.front();
		correctMessages.pop();
		
		Holon * machine = (Holon *)nextMessage->getSender();
		if(machine) {
			double xCurrentMachine, yCurrentMachine;
		
			_currentMachine->getXY(xCurrentMachine, yCurrentMachine);
			double currentDistance = towardsPosition(xCurrentMachine,yCurrentMachine);
			
			if (currentDistance!=-1) {
				if (currentDistance < 10) {
					setLinearVelocity(0,0);        
					setLinearAcceleration(0,0);
					setXY(xCurrentMachine,yCurrentMachine);
					
					NearMessage * nearMessage = new NearMessage();
					this->sendTo(_currentMachine, nearMessage);
					
					_timeout = getTime();
					result  = READY;
				}
				else { 
					AckMessage * ackMessage = new AckMessage();
					this->sendTo(_currentMachine, ackMessage);
					
					_timeout = getTime();
					result  = TRANSIT;
				}
			}
		}
	}
	else {
		if (getTime() - _timeout > ACKTIMEOUT) {
			result = INIT;
		}
	}
	
	return result;
}
int Piece::ready() {
	int result = READY;
	queue<HolonMessage *> correctMessages;
	
	while (getNbMessage()) {
		HolonMessage * message = (HolonMessage *)getNextMessage();
		if (message->isA("ReadyMessage")) {
			if ((message->getSender()) && (!message->getSender()->getName().compare(_currentMachineName))) {
				correctMessages.push(message);
			}
		}
	}

	if (correctMessages.size()) {
		ReadyMessage * readyMessage = new ReadyMessage();
		this->sendTo(_currentMachine, readyMessage);

		result = BUSY;
		_timeout = getTime();
	}
	else { 
		if (getTime() - _timeout > READYTIMEOUT) {
			result = INIT;
		}
	}
	return result;	 
}
int Piece::busy() {
	int result = BUSY;
	queue<HolonMessage *> correctMessages;
	
	while (getNbMessage()) {
		HolonMessage * message = (HolonMessage *)getNextMessage();

		if (message->isA("EndMessage")) {
			if ((message->getSender()) && (!message->getSender()->getName().compare(_currentMachineName))) {
				correctMessages.push(message);
			}
		}
	}
	
	if (correctMessages.size()) {
		result  = END;
	}
	else {
		if (getTime() - _timeout > BUSYTIMEOUT) {
			result = INIT;
		}
	}
	return result;
}
int Piece::end() {
	int result = END;
	
	_gamme.pop();
	translate(5,5);	
	changeShape(0);
	
	result = INIT;
	return result;
}
void Piece::exit() {
	double xOut, yOut;
	_atelier->_output->getXY(xOut, yOut);
	
	double dOutput = towardsPosition(xOut, yOut);
	if(dOutput < 1) { 
		setLinearVelocity(0,0);
		setLinearAcceleration(0,0);   
		setXY(xOut,yOut);
		Piece::getTypetreenode()->eraseAgent(this);
	}		
}

// Testing
bool Piece::isA(string type) {
	return (Piece::getTypetreenode()->isA(type));
}
