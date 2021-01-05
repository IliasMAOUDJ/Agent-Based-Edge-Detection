#include <stdio.h>
#include <string.h>
#include "EdgeFollowingAgent.h"
#include "NodeAgent.h"
#include <iostream>  // Fichier ex.cpp
#include <algorithm> // std::sort
#include <vector>    // std::vector
#include <cmath>
#include <map>
//--
EdgeFollowingAgent::EdgeFollowingAgent(System *sys, size_t row, size_t col) : Agent()
{
    newAgent();
    _sys = sys;
    _dir = 0;
    _row = row;
    _col = col;
    _explored.push_back(make_tuple(row, col));
}

//--
EdgeFollowingAgent::EdgeFollowingAgent(const EdgeFollowingAgent &anI) : Agent(anI)
{
    newAgent();
    _copy(anI);
}

//--
EdgeFollowingAgent &EdgeFollowingAgent::operator=(const EdgeFollowingAgent &anI)
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
EdgeFollowingAgent::~EdgeFollowingAgent(void)
{
    _destroy();
}

//--
void EdgeFollowingAgent::live(double dt)
{
    (void)dt; // Pour eviter un warning si pas utilise
              // Ajout de la position actuelle à la liste des positions explorées

    if (_sys->explored[_row][_col] != 255)
    {
        _sys->explored[_row][_col] = 255;
        _sys->pixelExplored++;
    }
    

    _explored.push_back(make_tuple(_row, _col));

    //map<int, int> hist = computeLocalHist(3);
    bool backtrack = followEdge();
    //-- Calcul nouvelle position
    getNewPos(_row, _col);

    tuple<int, int> currentPos = make_tuple(_row, _col);
    if (Contains(_explored, currentPos) || backtrack || _sys->explored[_row][_col] == 255)
    {
		new NodeAgent(_sys, _row, _col);
        delete this;
    }
}

bool EdgeFollowingAgent::Contains(const std::vector<tuple<int, int>> &list, tuple<int, int> x)
{
    return std::find(list.begin(), list.end(), x) != list.end();
}

bool EdgeFollowingAgent::followEdge()
{
    // 8 1 2
    // 7 X 3
    // 6 5 4
    int val = 0;
    int minVal = 0;
    int origin = _dir;
    /*
    int cdf = 0;
    // hist: - first is pixel value [0;255]
    //      - second is number of pixels with first value
    // normalized_hist : - first is pixel value
    //                   - second is normalized value
    
    map<int, int> normalized_hist;
    int cdfmin = hist.begin()->second;

    for (auto item : hist)
    {
        if (cdfmin == k * k)
        {
            normalized_hist[item.first] = item.first;
        }
        else
        {
            cdf += item.second;
            float D = (cdf - cdfmin);
            float N = (k * k) - cdfmin;
            normalized_hist[item.first] = (int)(D / N * 255);
        }
    }
    */
    for (int i = 0; i < 8; i++)
    {
        if (i == 3)
        {
            val = (5 * _sys->preprocessed[_row - 1][_col - 1] + 5 * _sys->preprocessed[_row - 1][_col] + 5 * _sys->preprocessed[_row - 1][_col + 1]     // [-3   -3  5]
                   - 3 * _sys->preprocessed[_row][_col - 1] + 0 * _sys->preprocessed[_row][_col] - 3 * _sys->preprocessed[_row][_col + 1]               // [-3  0 5]
                   - 3 * _sys->preprocessed[_row + 1][_col - 1] - 3 * _sys->preprocessed[_row + 1][_col] - 3 * _sys->preprocessed[_row + 1][_col + 1]); //[-3 -3 5]
        }                                                                                                                                               // 1
        if (i == 4)
        {
            val = (-3 * _sys->preprocessed[_row - 1][_col - 1] + 5 * _sys->preprocessed[_row - 1][_col] + 5 * _sys->preprocessed[_row - 1][_col + 1]    // [-3   -3  5]
                   - 3 * _sys->preprocessed[_row][_col - 1] + 0 * _sys->preprocessed[_row][_col] + 5 * _sys->preprocessed[_row][_col + 1]               // [-3  0 5]
                   - 3 * _sys->preprocessed[_row + 1][_col - 1] - 3 * _sys->preprocessed[_row + 1][_col] - 3 * _sys->preprocessed[_row + 1][_col + 1]); //[-3 -3 5]

        } // 2
        if (i == 5)
        {
            val = (-3 * _sys->preprocessed[_row - 1][_col - 1] - 3 * _sys->preprocessed[_row - 1][_col] + 5 * _sys->preprocessed[_row - 1][_col + 1]    // [-3   -3  5]
                   - 3 * _sys->preprocessed[_row][_col - 1] + 0 * _sys->preprocessed[_row][_col] + 5 * _sys->preprocessed[_row][_col + 1]               // [-3  0 5]
                   - 3 * _sys->preprocessed[_row + 1][_col - 1] - 3 * _sys->preprocessed[_row + 1][_col] + 5 * _sys->preprocessed[_row + 1][_col + 1]); //[-3 -3 5]

        } // 3
        if (i == 6)
        {
            val = (-3 * _sys->preprocessed[_row - 1][_col - 1] - 3 * _sys->preprocessed[_row - 1][_col] - 3 * _sys->preprocessed[_row - 1][_col + 1]    // [-3   -3  -3]
                   - 3 * _sys->preprocessed[_row][_col - 1] + 0 * _sys->preprocessed[_row][_col] + 5 * _sys->preprocessed[_row][_col + 1]               // [-3  0 5]
                   - 3 * _sys->preprocessed[_row + 1][_col - 1] + 5 * _sys->preprocessed[_row + 1][_col] + 5 * _sys->preprocessed[_row + 1][_col + 1]); //[-3 5 5]

        } // 4
        if (i == 7)
        {
            val = (-3 * _sys->preprocessed[_row - 1][_col - 1] - 3 * _sys->preprocessed[_row - 1][_col] - 3 * _sys->preprocessed[_row - 1][_col + 1]    // [-3   -3  -3]
                   - 3 * _sys->preprocessed[_row][_col - 1] + 0 * _sys->preprocessed[_row][_col] - 3 * _sys->preprocessed[_row][_col + 1]               // [-3  0 -3]
                   + 5 * _sys->preprocessed[_row + 1][_col - 1] + 5 * _sys->preprocessed[_row + 1][_col] + 5 * _sys->preprocessed[_row + 1][_col + 1]); //[5 5 5]

        } // 5
        if (i == 0)
        {
            val = (-3 * _sys->preprocessed[_row - 1][_col - 1] - 3 * _sys->preprocessed[_row - 1][_col] - 3 * _sys->preprocessed[_row - 1][_col + 1]    // [-3   -3  -3]
                   + 5 * _sys->preprocessed[_row][_col - 1] + 0 * _sys->preprocessed[_row][_col] - 3 * _sys->preprocessed[_row][_col + 1]               // [5  0 -3]
                   + 5 * _sys->preprocessed[_row + 1][_col - 1] + 5 * _sys->preprocessed[_row + 1][_col] - 3 * _sys->preprocessed[_row + 1][_col + 1]); //[5 5 -3]

        } // 6
        if (i == 1)
        {
            val = (5 * _sys->preprocessed[_row - 1][_col - 1] - 3 * _sys->preprocessed[_row - 1][_col] - 3 * _sys->preprocessed[_row - 1][_col + 1]     // [5   -3  -3]
                   + 5 * _sys->preprocessed[_row][_col - 1] + 0 * _sys->preprocessed[_row][_col] - 3 * _sys->preprocessed[_row][_col + 1]               // [5  0 -3]
                   + 5 * _sys->preprocessed[_row + 1][_col - 1] - 3 * _sys->preprocessed[_row + 1][_col] - 3 * _sys->preprocessed[_row + 1][_col + 1]); //[5 -3 -3]

        } // 7
        if (i == 2)
        {
            val = (5 * _sys->preprocessed[_row - 1][_col - 1] + 5 * _sys->preprocessed[_row - 1][_col] - 3 * _sys->preprocessed[_row - 1][_col + 1]     // [5   5  -3]
                   + 5 * _sys->preprocessed[_row][_col - 1] + 0 * _sys->preprocessed[_row][_col] - 3 * _sys->preprocessed[_row][_col + 1]               // [5  0 -3]
                   - 3 * _sys->preprocessed[_row + 1][_col - 1] - 3 * _sys->preprocessed[_row + 1][_col] - 3 * _sys->preprocessed[_row + 1][_col + 1]); //[-3 -3 -3]

        } // 8

        if (abs(val) > abs(minVal))
        {
            minVal = val;
            _dir = i;
        }
        else if (abs(val) == abs(minVal))
        {
            /*
            cout << "[" << _row << "][" << _col << "]" << endl;
            cout << "for i= " << i << " and _dir=" << _dir << "\t value is " << val <<  endl;
            cout << "(" << (int)_sys->preprocessed[_row - 1][_col - 1] << "\t" << (int)_sys->preprocessed[_row - 1][_col] << "\t" << (int)_sys->preprocessed[_row - 1][_col + 1] << ")" << endl
                 << "(" << (int)_sys->preprocessed[_row][_col - 1] << "\t" << (int)_sys->preprocessed[_row][_col] << "\t" << (int)_sys->preprocessed[_row][_col + 1] << ")" << endl
                 << "(" << (int)_sys->preprocessed[_row + 1][_col - 1] << "\t" <<(int) _sys->preprocessed[_row + 1][_col] << "\t" << (int)_sys->preprocessed[_row + 1][_col + 1] << ")" << endl;
                 */
        }
        
    }

    if (abs(minVal) / 15 > 10)
    {
        if (minVal < 0)
        {
            _sys->resultat[_row][_col] = 255;
            _sys->superposed.writePix(_row, _col, 0, 0, 255);
        }

        else if (minVal > 0)
        {
            _sys->resultat[_row][_col] = 0;
            _sys->superposed.writePix(_row, _col, 255, 0, 0);
        }
    }

    if (origin == (_dir + 4) % 8)
    {
        return true;
    }

    return false;
}
//--

map<int, int> EdgeFollowingAgent::computeLocalHist(size_t k) // compute local histogram of k*k pixels around the current pixel
{
    map<int, int> hist;
    size_t l = (k - 1) / 2;
    for (size_t r = _row - l; r <= _row + l; r++)
    {
        for (size_t c = _col - l; c <= _col + l; c++)
        {

            int o = _sys->preprocessed[r][c];
            hist[o]++;
        }
    }
    return hist;
}

bool EdgeFollowingAgent::getNewPos(size_t &row, size_t &col)
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
bool EdgeFollowingAgent::getPosDir(size_t &row, size_t &col) const // true si dans l'image
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

    if (dirRow < 1 || dirRow >= (int)(_sys->preprocessed.getNbRow() - 1))
        in = false;
    if (dirCol < 1 || dirCol >= (int)(_sys->preprocessed.getNbCol() - 1))
        in = false;

    if (in)
    {
        row = dirRow;
        col = dirCol;
    }

    return in;
}

//--
void EdgeFollowingAgent::draw(Image &im)
{
    octet val = 255;

    //im(_row - 1, _col - 1) = val;
    //im(_row - 1, _col) = val;
    //im(_row - 1, _col + 1) = val;

    //im(_row, _col - 1) = val;
    im(_row, _col) = val;
    //im(_row, _col + 1) = val;

    //im(_row + 1, _col - 1) = val;
    //im(_row + 1, _col) = val;
    //im(_row + 1, _col + 1) = val;

    size_t dirRow, dirCol;

    if (getPosDir(dirRow, dirCol))
        im(dirRow, dirCol) = 255;
}

//--
bool operator==(const EdgeFollowingAgent &anI1, const EdgeFollowingAgent &anI2)
{
    return anI1.isEqualTo(anI2);
}

//--
bool operator!=(const EdgeFollowingAgent &anI1, const EdgeFollowingAgent &anI2)
{
    return !(anI1 == anI2);
}

//--
ostream &operator<<(ostream &os, const EdgeFollowingAgent &anI)
{
    anI.display(os);
    return os;
}

//--
void EdgeFollowingAgent::display(ostream &os) const
{
    (void)os; // Pour eviter un warning si pas utilise

    Agent::display(os);

    // Affichage des attributs de la classe ImAgent
    // Exemple : os << _at;
    /*
    os << "Direction :" << _dir << endl;
    os << "Row :" << _row << endl;
    os << "Col :" << _col << endl;
    */
}

//--
bool EdgeFollowingAgent::isEqualTo(const EdgeFollowingAgent &anI) const
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
void EdgeFollowingAgent::_copy(const EdgeFollowingAgent &anI)
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
void EdgeFollowingAgent::_destroy(void)
{
    // Destruction des attributs de la classe ImAgent
    // Exemple : delete _at;
    // Rien...!
}
