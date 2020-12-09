#ifndef _ATELIER_H
#define _ATELIER_H

#define WORK_SIZE 100

#include "Object2D.h"

class Input;
class Output;

class Atelier: public Object2D {
 public:
	// Constructing
	Atelier();

	// Destructing
	~Atelier();

	Input * _input;
	Output * _output;
	int _size;
	
	// Events handling
	void keyPress(string key);
	void event_A();
	void event_B();
	void event_C();
	void event_P();
	void event_H();  // help
};

#endif
