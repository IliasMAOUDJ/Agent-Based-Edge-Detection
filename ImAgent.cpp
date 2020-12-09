#include "ImAgent.h"

//--
ImAgent::ImAgent(System *sys) : Agent()
{
 newAgent();
 _sys  = sys;
 _dir  = randomMinMax(1,8);
 _row  = randomMinMax(0,_sys->originale.getNbRow()-1);
 _col  = randomMinMax(0,_sys->originale.getNbCol()-1);
}

//--
ImAgent::ImAgent(const ImAgent& anI) : Agent(anI)
{
 newAgent();
 _copy(anI);
}

//--
ImAgent& ImAgent::operator=(const ImAgent& anI)
{
 if (this != &anI)
 {
  Agent::operator=(anI);
  _destroy();
  _copy(anI);
 }
 return *this;
}

//--
ImAgent::~ImAgent(void)
{
 _destroy();
}

//--
void ImAgent::live(double dt)
{
 (void)dt; // Pour eviter un warning si pas utilise
 
 // "Comportement" d'un Agent de la classe ImAgent
// cout << "My name is : " << getName() << endl;

 octet valPix, newValPix;

//--

 valPix = _sys->originale[_row][_col];           // lecture valeur

 newValPix = 255 - valPix;                       //-- Inversion pixel

 _sys->resultat[_row][_col] = newValPix;         // ecriture valeur

//--

//-- Calcul nouvelle position

 size_t newRow, newCol;

 if (getNewPos(newRow,newCol)) 
 { 
     _row = newRow; _col = newCol;
 }
 else {  // L'agent est arrive sur un bord de l'image

      _dir  = randomMinMax(1,8);                             // Il est remis
      _row  = randomMinMax(0,_sys->originale.getNbRow()-1);  // aleatoirement 
      _col  = randomMinMax(0,_sys->originale.getNbCol()-1);  // dans l'image

                              // On peut aussi faire : new ImAgent(_sys);
                              // puis                : delete this;
 }
}

//--
bool ImAgent::getNewPos(size_t& row, size_t& col)
{
#if 0 
 int newDir = randomMinMax(_dir-1,_dir+1);

 if (newDir==0) newDir = 8;
 if (newDir==9) newDir = 1;

 _dir = newDir;
#endif

 return getPosDir(row,col);
}

//--
bool ImAgent::getPosDir(size_t& row, size_t& col) const // true si dans l'image
{
 int dirRow, dirCol;

 switch (_dir) {
  case 1 : { dirRow = _row-1; dirCol = _col;   break; }  // 1    // 8 1 2
  case 2 : { dirRow = _row-1; dirCol = _col+1; break; }  // 2    // 7 X 3
  case 3 : { dirRow = _row;   dirCol = _col+1; break; }  // 3    // 6 5 4
  case 4 : { dirRow = _row+1; dirCol = _col+1; break; }  // 4
  case 5 : { dirRow = _row+1; dirCol = _col;   break; }  // 5
  case 6 : { dirRow = _row+1; dirCol = _col-1; break; }  // 6
  case 7 : { dirRow = _row;   dirCol = _col-1; break; }  // 7
  default: { dirRow = _row-1; dirCol = _col-1; break; }  // 8
 }

 bool  in = true;

 if (dirRow < 0 || dirRow >= (int)(_sys->originale.getNbRow())) in = false;
 if (dirCol < 0 || dirCol >= (int)(_sys->originale.getNbCol())) in = false;

 if (in) { row = dirRow ; col = dirCol; }

 return in;
}

//--
void ImAgent::draw(Image& im)
{
 octet val = 254;

 im(_row-1,_col-1)=val;
 im(_row-1,_col  )=val;
 im(_row-1,_col+1)=val;

 im(_row  ,_col-1)=val;
 im(_row  ,_col  )=val;
 im(_row  ,_col+1)=val;

 im(_row+1,_col-1)=val;
 im(_row+1,_col  )=val;
 im(_row+1,_col+1)=val;

 size_t dirRow, dirCol;

 if (getPosDir(dirRow,dirCol)) im(dirRow,dirCol)=0;
}

//--
bool operator==(const ImAgent& anI1, const ImAgent& anI2)
{
 return anI1.isEqualTo(anI2);
}

//--
bool operator!=(const ImAgent& anI1, const ImAgent& anI2)
{
 return !(anI1==anI2);
}

//--
ostream& operator<<(ostream& os, const ImAgent& anI)
{
 anI.display(os);
 return os;
}

//--
void ImAgent::display(ostream& os) const
{
 (void)os; // Pour eviter un warning si pas utilise

 Agent::display(os);

 // Affichage des attributs de la classe ImAgent
 // Exemple : os << _at;

 os << "Direction :" << _dir  << endl;
 os << "Row :"       << _row << endl;
 os << "Col :"       << _col << endl;
}

//--
bool ImAgent::isEqualTo(const ImAgent& anI) const
{
 (void)anI; // Pour eviter un warning si pas utilise

 // Test des attributs de la classe ImAgent
 // Exemple : if (_at != anI._at) return false;

 if (_sys != anI._sys) return false;
 if (_dir != anI._dir) return false;
 if (_row != anI._row) return false;
 if (_col != anI._col) return false;

 if (!(Agent::isEqualTo(anI))) return false;
 return true;
}

//--
void ImAgent::_copy(const ImAgent& anI)
{
 (void)anI; // Pour eviter un warning si pas utilise

 // Affectation des attributs de la classe ImAgent
 // Exemple : _at = anI._at;

 _sys = anI._sys;
 _dir = anI._dir;
 _row = anI._row;
 _col = anI._col;
}

//--
void ImAgent::_destroy(void)
{
 // Destruction des attributs de la classe ImAgent
 // Exemple : delete _at;
 // Rien...!
}
