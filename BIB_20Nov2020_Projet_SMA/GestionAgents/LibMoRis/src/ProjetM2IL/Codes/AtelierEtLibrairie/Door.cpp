#include "Atelier.h"
#include "Door.h"

Door::Door(Atelier* atelier, double x, double y):Or2DEntity(x, y, 0) {
  _atelier = atelier;
  this->attachTo(*_atelier);
  
  string color = "red";
  double cote = _atelier->_size/20;
  
  this->square(cote, 0);
  
  _ouverte = true;
}

