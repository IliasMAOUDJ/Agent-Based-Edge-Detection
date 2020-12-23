#include <stdio.h>
#include <string.h>
#include "ExplorationAgent.h"
#include <iostream>             // Fichier ex.cpp
#include <algorithm>    // std::sort
#include <vector>       // std::vector
//--
ExplorationAgent::ExplorationAgent(System *sys) : Agent()
{
 newAgent();
 _sys  = sys;
 _dir  = randomMinMax(1,8);
 _row  = randomMinMax(0,_sys->originale.getNbRow()-1);
 _col  = randomMinMax(0,_sys->originale.getNbCol()-1);
}

//--
ExplorationAgent::ExplorationAgent(const ExplorationAgent& anI) : Agent(anI)
{
 newAgent();
 _copy(anI);
}

//--
ExplorationAgent& ExplorationAgent::operator=(const ExplorationAgent& anI)
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
ExplorationAgent::~ExplorationAgent(void)
{
 _destroy();
}

//--
void ExplorationAgent::live(double dt)
{
 (void)dt; // Pour eviter un warning si pas utilise
 /*
 // "Comportement" d'un Agent de la classe ExplorationAgent
 // Look for an edge: 
 // Dir is defined as follows: dir = Ph* dir - (1-Ph)*dirGVF(curr_pos)
 // where Ph represents the percentage of chaotic component -> if =0, only GVF is important, if =1 total random dir

    curr_pos = = _sys->originale[_row][_col];
    if(curr_pos == edge)
    {
        onEdge = true;
    }
// 1) agent checks if it is on a potential edge, not already visited by another agent. 
//    If it is the case, determine the various possible directions of the edge, it creates a "EdgeFollowingAgent" for the detected directions.
//    The created agents will follow the edge. After detecting an edge, the exploration agent changes into a node agent or disappears.

    if(onEdge)
    {
        directions = determineDirections();
        for d in directions:
            create_EdgeFollowingAgent(pos,d);
        changeTo_NodeAgent();
    }
// 2) The agent moves to a potential gradient thanks to the GVF. It computes its new position. In some places of the GVF, dir =0, the agent
//    follows a random direction. Agents can thus go to edges which do not express gradients sufficiently high to appear in the GVF.
    else
    {
        next_pos = curr_pos + move_speed * dt * dir;
        curr_pos = next_pos;
    }
// 3) if the agent doesn't find any edge before a deadline, it disappears.
*/

if(_sys->resultat[_row][_col] < 255)
{
    _sys->resultat[_row][_col] = 0;
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
bool ExplorationAgent::getNewPos(size_t& row, size_t& col)
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
bool ExplorationAgent::getPosDir(size_t& row, size_t& col) const // true si dans l'image
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
void ExplorationAgent::draw(Image& im)
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
bool operator==(const ExplorationAgent& anI1, const ExplorationAgent& anI2)
{
 return anI1.isEqualTo(anI2);
}

//--
bool operator!=(const ExplorationAgent& anI1, const ExplorationAgent& anI2)
{
 return !(anI1==anI2);
}

//--
ostream& operator<<(ostream& os, const ExplorationAgent& anI)
{
 anI.display(os);
 return os;
}

//--
void ExplorationAgent::display(ostream& os) const
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
bool ExplorationAgent::isEqualTo(const ExplorationAgent& anI) const
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
void ExplorationAgent::_copy(const ExplorationAgent& anI)
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
void ExplorationAgent::_destroy(void)
{
 // Destruction des attributs de la classe ImAgent
 // Exemple : delete _at;
 // Rien...!
}