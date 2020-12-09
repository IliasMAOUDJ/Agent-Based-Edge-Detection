#include "Atelier.h"

#include "Input.h"
#include "Output.h"

#include "MachineA.h"
#include "MachineB.h"
#include "MachineC.h"
#include "Piece.h"

// Constructing
Atelier::Atelier() {
  this->_size = WORK_SIZE;
  this->setLocation(0,0,0);
  this->setColor("White");
  this->square(_size, false);
  
  _input  = new Input(this);
  _output = new Output(this);
}

// Destructing
Atelier::~Atelier() {}

// Events handling
void Atelier::keyPress(string key) {
  if ((!key.compare("A")) || (!key.compare("a"))) event_A();
  else if ((!key.compare("B")) || (!key.compare("b"))) event_B();
  else if ((!key.compare("C")) || (!key.compare("c"))) event_C();
  else if ((!key.compare("P")) || (!key.compare("p"))) event_P();
  else if ((!key.compare("H")) || (!key.compare("h"))) event_H();
}
void Atelier::event_A() {	
  cout << "a/A : Creation d'une machine A (orange)" << endl;
  MachineA::create(this);
}
void Atelier::event_B() {
  cout << "b/B : Creation d'une machine B (bleue)" << endl;
  MachineB::create(this);
}
void Atelier::event_C() {
  cout << "c/C : Creation d'une machine C (verte)" << endl;
  MachineC::create(this);
}
void Atelier::event_P() {
  cout << "p/P : Creation d'une Piece" << endl;
  Piece::create(this);
}
void Atelier::event_H() {
  cout << endl;
  cout << "-----------------------------------" << endl;
  cout << "Touches disponibles sur un Atelier:" << endl;
  cout << "-----------------------------------" << endl;
  cout << "a/A : Creation d'une machine A (orange)" << endl;
  cout << "b/B : Creation d'une machine B (bleue)" << endl;
  cout << "c/C : Creation d'une machine C (verte)" << endl;
  cout << "p/P : Creation d'une Piece" << endl;
  cout << "h/H : Help ! " << endl;
  cout << "-----------------------------------" << endl;
}
