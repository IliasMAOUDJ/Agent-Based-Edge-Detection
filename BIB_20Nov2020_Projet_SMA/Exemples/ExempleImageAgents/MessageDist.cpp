#include "MessageDist.h"

//--
MessageDist::MessageDist(size_t row, size_t col, bool response) : Message(), _row(row), _col(col), _response(response)
{
 	newMessage();
}

MessageDist::MessageDist(size_t row, size_t col, float dist, bool response) : Message(), _row(row), _col(col), _response(response), _dist(dist)
{
	newMessage();
}

//--
MessageDist::MessageDist(const MessageDist& aM) : Message(aM)
{
 newMessage();
 _copy(aM);
}

//--
MessageDist& MessageDist::operator=(const MessageDist& aM)
{
 if (this != &aM)
 {
  Message::operator=(aM);
  _destroy();
  _copy(aM);
 }
 return *this;
}

//--
MessageDist::~MessageDist(void)
{
 _destroy();
}

//--
bool operator==(const MessageDist& aM1, const MessageDist& aM2)
{
 return aM1.isEqualTo(aM2);
}

//--
bool operator!=(const MessageDist& aM1, const MessageDist& aM2)
{
 return !(aM1==aM2);
}

//--
void MessageDist::setRow(size_t row)
{
 _row = row;
}

//--
size_t  MessageDist::getRow(void) const
{
 return _row;
}

void MessageDist::setCol(size_t col)
{
 _col = col;
}

//--
size_t  MessageDist::getCol(void) const
{
 return _col;
}

void MessageDist::setResponse(bool response)
{
	_response = response;
}

bool MessageDist::getResponse(void) const
{
	return _response;
}

void MessageDist::setDist(float dist)
{
	_dist = dist;
}

float MessageDist::getDist(void) const
{
	return _dist;
}

//--
ostream& operator<<(ostream& os, const MessageDist& aM)
{
 aM.display(os);
 return os;
}

//--
void MessageDist::display(ostream& os) const
{
 (void)os; // Pour eviter un warning si pas utilise

 Message::display(os);

 // Affichage des attributs de la classe MessageDist

 os << "La valeur de row est : " << _row << endl;
 os << "La valeur de col est : " << _col << endl;
 os << "La valeur de response est : " << _response << endl;
}

//--
bool MessageDist::isEqualTo(const MessageDist& aM) const
{
 (void)aM; // Pour eviter un warning si pas utilise

 // ### : Test des attributs de la classe MessageDist

 if (_col != aM._col && _row != aM._row && _response != aM._response) return false;

 if (!(Message::isEqualTo(aM))) return false;
 return true;
}

//--
void MessageDist::_copy(const MessageDist& aM)
{
 (void)aM; // Pour eviter un warning si pas utilise

 // ### : Affectation des attributs de la classe MessageDist

 _row = aM._row;
 _col = aM._col;
 _response = aM._response;
}

//--
void MessageDist::_destroy(void)
{
 // Destruction des attributs de la classe MessageDist
 // rien !
}

