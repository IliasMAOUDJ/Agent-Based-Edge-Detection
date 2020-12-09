#include "Atelier.h"
#include "Input.h"

// Constructing
Input::Input(Atelier * atelier):Door(atelier, -atelier->_size/2, 0) {
}
