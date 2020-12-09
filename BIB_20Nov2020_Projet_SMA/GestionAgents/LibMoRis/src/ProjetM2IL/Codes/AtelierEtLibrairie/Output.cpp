#include "Atelier.h"
#include "Output.h"
#include "Utilities.h"


// Constructing
Output::Output(Atelier* atelier):Door(atelier, atelier->_size/2, 0) {
	double _pi_ =  3.14159265;
	this->rotate(_pi_);
}
