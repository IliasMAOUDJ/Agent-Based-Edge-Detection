#include "A.h"

//--
A::A( /* ### */ ) : Agent(),
                    B( /* ### */ )
{
 newAgent();
}

//--
A::A(const A& anA) : Agent(anA),
                     B(anA)
{
 newAgent();
 _copy(anA);
}

//--
A& A::operator=(const A& anA)
{
 if (this != &anA)
 {
  Agent::operator=(anA);
  B::operator=(anA);
  _destroy();
  _copy(anA);
 }
 return *this;
}

//--
A::~A(void)
{
 _destroy();
}

//--
void A::live(double dt)
{
 (void)dt; // Pour eviter un warning si pas utilise

 // ### : "Comportement" d'un Agent de la classe A
}

//--
bool operator==(const A& anA1, const A& anA2)
{
 return anA1.isEqualTo(anA2);
}

//--
bool operator!=(const A& anA1, const A& anA2)
{
 return !(anA1==anA2);
}

//--
ostream& operator<<(ostream& os, const A& anA)
{
 anA.display(os);
 return os;
}

//--
void A::display(ostream& os) const
{
 (void)os; // Pour eviter un warning si pas utilise

 Agent::display(os);
 B::display(os);

 // ### : Affichage des attributs de la classe A
 // Exemple : os << _at;
}

//--
bool A::isEqualTo(const A& anA) const
{
 (void)anA; // Pour eviter un warning si pas utilise

 // ### : Test des attributs de la classe A
 // Exemple : if (_at != anA._at) return false;

 if (!(Agent::isEqualTo(anA))) return false;
 if (!(B::isEqualTo(anA))) return false;
 return true;
}

//--
void A::_copy(const A& anA)
{
 (void)anA; // Pour eviter un warning si pas utilise

 // ### : Affectation des attributs de la classe A
 // Exemple : _at = anA._at;
}

//--
void A::_destroy(void)
{
 // ### : Destruction des attributs de la classe A
 // Exemple : delete _at;
}

