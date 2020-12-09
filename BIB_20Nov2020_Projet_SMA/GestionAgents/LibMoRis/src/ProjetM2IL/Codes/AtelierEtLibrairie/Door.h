#ifndef _DOOR_H
#define _DOOR_H

#include "Or2DEntity.h"

class Atelier;

class Door: public Or2DEntity{
	
 public:
	Door(Atelier* atelier, double x, double y);

	void open();
	void close();

	void main();


	bool    _ouverte;

	Atelier* _atelier;
};

#endif
