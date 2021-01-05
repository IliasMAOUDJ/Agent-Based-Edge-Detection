#include <stdio.h>
#include <string.h>
#include "Edge_detection.h"
#include "EdgeFollowingAgent.h"
#include <iostream>  // Fichier ex.cpp
#include <algorithm> // std::sort
#include <vector>    // std::vector
#include <cmath>
//--
KirschAgent::KirschAgent(System *sys) : Agent()
{
    newAgent();
    _sys = sys;
    _dir = randomMinMax(1, 8);
    _row = randomMinMax(1, _sys->preprocessed.getNbRow() - 2);
    _col = randomMinMax(1, _sys->preprocessed.getNbCol() - 2);
}

//--
KirschAgent::KirschAgent(const KirschAgent &anI) : Agent(anI)
{
    newAgent();
    _copy(anI);
}

//--
KirschAgent &KirschAgent::operator=(const KirschAgent &anI)
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
KirschAgent::~KirschAgent(void)
{
    _destroy();
}

//--
void KirschAgent::live(double dt)
{
    (void)dt; // Pour eviter un warning si pas utilise

    // "Comportement" d'un Agent de la classe KirschAgent
    //cout << "My name is : " << getName() << endl;
    
    
    kirschOperator();
    if (_sys->explored[_row][_col] != 255)
        _sys->explored[_row][_col] = 255;
    //-- Calcul nouvelle position

    size_t newRow, newCol;

    if (getNewPos(newRow, newCol))
    {
        _row = newRow;
        _col = newCol;
    }
    else
    {                                                              // L'agent est arrive sur un bord de l'image
        _dir = randomMinMax(1, 8);                                 // Il est remis
        _row = randomMinMax(2, _sys->preprocessed.getNbRow() - 2); // aleatoirement
        _col = randomMinMax(2, _sys->preprocessed.getNbCol() - 2); // dans l'image

        // On peut aussi faire : new ImAgent(_sys);
        // puis                : delete this;
    }
}

void KirschAgent::kirschOperator(void)
{
    // 8 1 2
    // 7 X 3
    // 6 5 4
    int val = 0;

    if (_row > 0 && _row < _sys->preprocessed.getNbRow()-1)
    {

        switch (_dir)
        {
        case 1:
        {
            val = (5 * _sys->preprocessed[_row - 1][_col - 1] + 5 * _sys->preprocessed[_row - 1][_col] + 5 * _sys->preprocessed[_row - 1][_col + 1]     // [-3   -3  5]
                   - 3 * _sys->preprocessed[_row][_col - 1] + 0 * _sys->preprocessed[_row][_col] - 3 * _sys->preprocessed[_row][_col + 1]               // [-3  0 5]
                   - 3 * _sys->preprocessed[_row + 1][_col - 1] - 3 * _sys->preprocessed[_row + 1][_col] - 3 * _sys->preprocessed[_row + 1][_col + 1]); //[-3 -3 5]

            break;
        } // 1
        case 2:
        {
            val = (-3 * _sys->preprocessed[_row - 1][_col - 1] + 5 * _sys->preprocessed[_row - 1][_col] + 5 * _sys->preprocessed[_row - 1][_col + 1]    // [-3   -3  5]
                   - 3 * _sys->preprocessed[_row][_col - 1] + 0 * _sys->preprocessed[_row][_col] + 5 * _sys->preprocessed[_row][_col + 1]               // [-3  0 5]
                   - 3 * _sys->preprocessed[_row + 1][_col - 1] - 3 * _sys->preprocessed[_row + 1][_col] - 3 * _sys->preprocessed[_row + 1][_col + 1]); //[-3 -3 5]
            break;
        } // 2
        case 3:
        {
            val = (-3 * _sys->preprocessed[_row - 1][_col - 1] - 3 * _sys->preprocessed[_row - 1][_col] + 5 * _sys->preprocessed[_row - 1][_col + 1]    // [-3   -3  5]
                   - 3 * _sys->preprocessed[_row][_col - 1] + 0 * _sys->preprocessed[_row][_col] + 5 * _sys->preprocessed[_row][_col + 1]               // [-3  0 5]
                   - 3 * _sys->preprocessed[_row + 1][_col - 1] - 3 * _sys->preprocessed[_row + 1][_col] + 5 * _sys->preprocessed[_row + 1][_col + 1]); //[-3 -3 5]

            break;
        } // 3
        case 4:
        {
            val = (-3 * _sys->preprocessed[_row - 1][_col - 1] - 3 * _sys->preprocessed[_row - 1][_col] - 3 * _sys->preprocessed[_row - 1][_col + 1]    // [-3   -3  -3]
                   - 3 * _sys->preprocessed[_row][_col - 1] + 0 * _sys->preprocessed[_row][_col] + 5 * _sys->preprocessed[_row][_col + 1]               // [-3  0 5]
                   - 3 * _sys->preprocessed[_row + 1][_col - 1] + 5 * _sys->preprocessed[_row + 1][_col] + 5 * _sys->preprocessed[_row + 1][_col + 1]); //[-3 5 5]

            break;
        } // 4
        case 5:
        {
            val = (-3 * _sys->preprocessed[_row - 1][_col - 1] - 3 * _sys->preprocessed[_row - 1][_col] - 3 * _sys->preprocessed[_row - 1][_col + 1]    // [-3   -3  -3]
                   - 3 * _sys->preprocessed[_row][_col - 1] + 0 * _sys->preprocessed[_row][_col] - 3 * _sys->preprocessed[_row][_col + 1]               // [-3  0 5]
                   + 5 * _sys->preprocessed[_row + 1][_col - 1] + 5 * _sys->preprocessed[_row + 1][_col] + 5 * _sys->preprocessed[_row + 1][_col + 1]); //[-3 5 5]

            break;
        } // 5
        case 6:
        {
            val = (-3 * _sys->preprocessed[_row - 1][_col - 1] - 3 * _sys->preprocessed[_row - 1][_col] - 3 * _sys->preprocessed[_row - 1][_col + 1]    // [-3   -3  -3]
                   + 5 * _sys->preprocessed[_row][_col - 1] + 0 * _sys->preprocessed[_row][_col] - 3 * _sys->preprocessed[_row][_col + 1]               // [-3  0 -3]
                   + 5 * _sys->preprocessed[_row + 1][_col - 1] + 5 * _sys->preprocessed[_row + 1][_col] - 3 * _sys->preprocessed[_row + 1][_col + 1]); //[5 5 5]

            break;
        } // 6
        case 7:
        {
            val = (5 * _sys->preprocessed[_row - 1][_col - 1] - 3 * _sys->preprocessed[_row - 1][_col] - 3 * _sys->preprocessed[_row - 1][_col + 1]     // [-3   -3  -3]
                   + 5 * _sys->preprocessed[_row][_col - 1] + 0 * _sys->preprocessed[_row][_col] - 3 * _sys->preprocessed[_row][_col + 1]               // [-3  0 -3]
                   + 5 * _sys->preprocessed[_row + 1][_col - 1] - 3 * _sys->preprocessed[_row + 1][_col] - 3 * _sys->preprocessed[_row + 1][_col + 1]); //[5 5 5]

            break;
        } // 7
        default:
        {
            val = (5 * _sys->preprocessed[_row - 1][_col - 1] + 5 * _sys->preprocessed[_row - 1][_col] - 3 * _sys->preprocessed[_row - 1][_col + 1]     // [-3   -3  -3]
                   + 5 * _sys->preprocessed[_row][_col - 1] + 0 * _sys->preprocessed[_row][_col] - 3 * _sys->preprocessed[_row][_col + 1]               // [-3  0 -3]
                   - 3 * _sys->preprocessed[_row + 1][_col - 1] - 3 * _sys->preprocessed[_row + 1][_col] - 3 * _sys->preprocessed[_row + 1][_col + 1]); //[5 5 5]

            break;
        } // 8
        }
    }
    
    if (abs(val)/15 > 15 && _sys->explored[_row][_col] != 255)
    {        
        new EdgeFollowingAgent(_sys, _row, _col);
    }
}
//--
bool KirschAgent::getNewPos(size_t &row, size_t &col)
{
#if 0 
 int newDir = randomMinMax(_dir-1,_dir+1);

 if (newDir==0) newDir = 8;
 if (newDir==9) newDir = 1;

 _dir = newDir;
#endif

    return getPosDir(row, col);
}

//--
bool KirschAgent::getPosDir(size_t &row, size_t &col) const // true si dans l'image
{
    int dirRow, dirCol;

    switch (_dir)
    {
    case 1:
    {
        dirRow = _row - 1;
        dirCol = _col;
        break;
    } // 1    // 8 1 2
    case 2:
    {
        dirRow = _row - 1;
        dirCol = _col + 1;
        break;
    } // 2    // 7 X 3
    case 3:
    {
        dirRow = _row;
        dirCol = _col + 1;
        break;
    } // 3    // 6 5 4
    case 4:
    {
        dirRow = _row + 1;
        dirCol = _col + 1;
        break;
    } // 4
    case 5:
    {
        dirRow = _row + 1;
        dirCol = _col;
        break;
    } // 5
    case 6:
    {
        dirRow = _row + 1;
        dirCol = _col - 1;
        break;
    } // 6
    case 7:
    {
        dirRow = _row;
        dirCol = _col - 1;
        break;
    } // 7
    default:
    {
        dirRow = _row - 1;
        dirCol = _col - 1;
        break;
    } // 8
    }

    bool in = true;

    if (dirRow < 0 || dirRow >= (int)(_sys->preprocessed.getNbRow()))
        in = false;
    if (dirCol < 0 || dirCol >= (int)(_sys->preprocessed.getNbCol()))
        in = false;

    if (in)
    {
        row = dirRow;
        col = dirCol;
    }

    return in;
}

//--
void KirschAgent::draw(Image &im)
{
    octet val = 1;

    im(_row - 1, _col - 1) = val;
    im(_row - 1, _col) = val;
    im(_row - 1, _col + 1) = val;

    im(_row, _col - 1) = val;
    im(_row, _col) = val;
    im(_row, _col + 1) = val;

    im(_row + 1, _col - 1) = val;
    im(_row + 1, _col) = val;
    im(_row + 1, _col + 1) = val;

    size_t dirRow, dirCol;

    if (getPosDir(dirRow, dirCol))
        im(dirRow, dirCol) = 0;
}

//--
bool operator==(const KirschAgent &anI1, const KirschAgent &anI2)
{
    return anI1.isEqualTo(anI2);
}

//--
bool operator!=(const KirschAgent &anI1, const KirschAgent &anI2)
{
    return !(anI1 == anI2);
}

//--
ostream &operator<<(ostream &os, const KirschAgent &anI)
{
    anI.display(os);
    return os;
}

//--
void KirschAgent::display(ostream &os) const
{
    (void)os; // Pour eviter un warning si pas utilise

    Agent::display(os);

    // Affichage des attributs de la classe ImAgent
    // Exemple : os << _at;

    os << "Direction :" << _dir << endl;
    os << "Row :" << _row << endl;
    os << "Col :" << _col << endl;
}

//--
bool KirschAgent::isEqualTo(const KirschAgent &anI) const
{
    (void)anI; // Pour eviter un warning si pas utilise

    // Test des attributs de la classe ImAgent
    // Exemple : if (_at != anI._at) return false;

    if (_sys != anI._sys)
        return false;
    if (_dir != anI._dir)
        return false;
    if (_row != anI._row)
        return false;
    if (_col != anI._col)
        return false;

    if (!(Agent::isEqualTo(anI)))
        return false;
    return true;
}

//--
void KirschAgent::_copy(const KirschAgent &anI)
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
void KirschAgent::_destroy(void)
{
    // Destruction des attributs de la classe ImAgent
    // Exemple : delete _at;
    // Rien...!
}