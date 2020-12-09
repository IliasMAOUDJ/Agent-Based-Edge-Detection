#include <stdio.h>
#include <string.h>
#include "Edge_detection.h"
#include <iostream>             // Fichier ex.cpp
#include <algorithm>    // std::sort
#include <vector>       // std::vector
//--
Edge_detection_Agent::Edge_detection_Agent(System *sys) : Agent()
{
 newAgent();
 _sys  = sys;
 _dir  = randomMinMax(1,8);
 _row  = randomMinMax(1,_sys->originale.getNbRow()-2);
 _col  = randomMinMax(1,_sys->originale.getNbCol()-2);
}

//--
Edge_detection_Agent::Edge_detection_Agent(const Edge_detection_Agent& anI) : Agent(anI)
{
 newAgent();
 _copy(anI);
}

//--
Edge_detection_Agent& Edge_detection_Agent::operator=(const Edge_detection_Agent& anI)
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
Edge_detection_Agent::~Edge_detection_Agent(void)
{
 _destroy();
}

//--
void Edge_detection_Agent::live(double dt)
{
 (void)dt; // Pour eviter un warning si pas utilise
 
 // "Comportement" d'un Agent de la classe Edge_detection_Agent
 //cout << "My name is : " << getName() << endl;

 octet v, newValPix;
 vector<octet> valPixs; // liste de pixels pour le filtre médian

 if (_row > 0 && _row < _sys->originale.getNbRow() - 1) // Si l'agent n'est pas au bord de l'image
 {
     for (int i = (int)_row - 1; i <= (int)_row + 1; i++)
     {
         for (int j = (int)_col - 1; j <= (int)_col + 1; j++) // le filtre median a un noyau de 3x3
         {
             v = _sys->originale[i][j];
             valPixs.push_back(v);
         }
     }   
     sort(valPixs.begin(), valPixs.end());
     newValPix = valPixs.at(4); // filtre 3x3 donc 9 valeurs -> le median est la 5e valeur donc à l'indice 4
     _sys->resultat[_row][_col] = newValPix;
 }
//-- Calcul nouvelle position

 size_t newRow, newCol;

 if (getNewPos(newRow,newCol)) 
 { 
     _row = newRow; _col = newCol;
 }
 else {  // L'agent est arrive sur un bord de l'image

      _dir  = randomMinMax(1,8);                             // Il est remis
      _row  = randomMinMax(1,_sys->originale.getNbRow()-2);  // aleatoirement 
      _col  = randomMinMax(1,_sys->originale.getNbCol()-2);  // dans l'image

                              // On peut aussi faire : new ImAgent(_sys);
                              // puis                : delete this;
 }
}

//--
bool Edge_detection_Agent::getNewPos(size_t& row, size_t& col)
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
bool Edge_detection_Agent::getPosDir(size_t& row, size_t& col) const // true si dans l'image
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
void Edge_detection_Agent::draw(Image& im)
{
 octet val = 127;

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
bool operator==(const Edge_detection_Agent& anI1, const Edge_detection_Agent& anI2)
{
 return anI1.isEqualTo(anI2);
}

//--
bool operator!=(const Edge_detection_Agent& anI1, const Edge_detection_Agent& anI2)
{
 return !(anI1==anI2);
}

//--
ostream& operator<<(ostream& os, const Edge_detection_Agent& anI)
{
 anI.display(os);
 return os;
}

//--
void Edge_detection_Agent::display(ostream& os) const
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
bool Edge_detection_Agent::isEqualTo(const Edge_detection_Agent& anI) const
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
void Edge_detection_Agent::_copy(const Edge_detection_Agent& anI)
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
void Edge_detection_Agent::_destroy(void)
{
 // Destruction des attributs de la classe ImAgent
 // Exemple : delete _at;
 // Rien...!
}