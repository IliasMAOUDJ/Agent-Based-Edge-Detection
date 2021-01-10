#include <stdio.h>
#include <string.h>
#include "NodeAgent.h"
#include "MessageDist.h"
#include <iostream>  // Fichier ex.cpp
#include <algorithm> // std::sort
#include <vector>    // std::vector
#include <cmath>
//--
NodeAgent::NodeAgent(System *sys, size_t row, size_t col) : Agent()
{
    newAgent();
    _sys = sys;
    _dir = 0;
    _row = row;
    _col = col;
    _cycle = 0;
    _message = false;
    _explored.push_back(make_tuple(row,col));
    setSensitivity("MessageDist", true);
}

//--
NodeAgent::NodeAgent(const NodeAgent &anI) : Agent(anI)
{
    newAgent();
    _copy(anI);
}

//--
NodeAgent &NodeAgent::operator=(const NodeAgent &anI)
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
NodeAgent::~NodeAgent(void)
{
    _destroy();
}

//--
void NodeAgent::live(double dt)
{
    (void)dt; // Pour eviter un warning si pas utilise

    if (_cycle % 50 == 0) {
    	MessageDist m(_row, _col, false);
        broadcastMessage(m);
 	}

 	if (_cycle > 250) {
 		delete this;
 	}

 	string agentName = getName();
	string className = getClass();
	string prefixe   = "["+agentName+"]";

	Agent* bestEmitter = NULL;
	float bestDist = 5;
	bool firstTime = true;
	size_t bestRow = 0;
	size_t bestCol = 0;
	bool receive_response = false;

    while (getNbMessages())
	{
		MessageDist*  message = (MessageDist*)getNextMessage();

		Agent*       emitter = message->getEmitter();
		string   emitterName = "(NULL)";

		if (exist(emitter)) emitterName = emitter->getName();
		if (emitterName == agentName) continue;

		if (message->getResponse()) {
			receive_response = true;
			float d = (float)(((message->getRow()-_row)*(message->getRow()-_row)) + (float)((message->getCol()-_col)*(message->getCol()-_col)));
			if (d < 25) {
				if (firstTime) {
					firstTime = false;
					bestDist = d;
					bestRow = message->getRow();
					bestCol = message->getCol();
					bestEmitter = message->getEmitter();
				}
				if (d < bestDist) {

					bestDist = d;
					bestRow = message->getRow();
					bestCol = message->getCol();
					bestEmitter = message->getEmitter();
				}
			}

		} else {

			size_t row = message->getRow();
			size_t col = message->getCol();

			//cout << "=====================================" << endl;
			//cout << row << endl;
			//cout << col << endl;
			//cout << _row << endl;
			//cout << _col << endl;
			float dist = (float)(((row-_row)*(row-_row)) + (float)((col-_col)*(col-_col)));
			//cout << dist << endl;
			//cout << "=====================================" << endl;

			if (dist < 25) {
				/*cout << prefixe << " => I reply to " << emitterName
			              << " with value "    << _row
			              << " and " << _col << endl;*/

				MessageDist m(_row, _col, dist, true);
				sendMessageTo(m,emitter);
			}

		}
		/*cout << prefixe << " I receive a message from : " << emitterName
		              << " with value : " << message->getRow()
		              << " and : " << message->getCol() << endl;*/

		delete message;
	}
	if (exist(bestEmitter) && receive_response) {
		drawLine(_row, _col, bestRow, bestCol);
		delete this;
	}

	_cycle++;
}

void NodeAgent::drawLine(size_t r1, size_t c1, size_t r2, size_t c2) {

    int r = r2 - r1;
    int c = c2 - c1;
    if (r == 0 || c == 0) 
    	return;
    int max = 0;
    if ( fabs(r) > fabs(c) ) {
    	max = fabs(r);
    } else {
    	max = fabs(c);
    }
    if (max != 0) {
    	r = (r/max);
    	c = (c/max);

	    for (int i=0; i<max;i++) {
	    	r1 = (int) r1;
	    	c1 = (int) c1;
	    	_sys->resultat[r1][c1] = 255;
            _sys->superposed.writePix(_row, _col, 0, 255, 0);
	    	r1 += r;
	    	c1 += c;
	    }
	}


}

bool NodeAgent::Contains(const std::vector<tuple<int,int>> &list,  tuple<int,int> x)
{
	return std::find(list.begin(), list.end(), x) != list.end();
}

//--
bool NodeAgent::getNewPos(size_t &row, size_t &col)
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
bool NodeAgent::getPosDir(size_t &row, size_t &col) const // true si dans l'image
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

    if (dirRow < 1 || dirRow >= (int)(_sys->preprocessed.getNbRow()-1))
        in = false;
    if (dirCol < 1 || dirCol >= (int)(_sys->preprocessed.getNbCol()-1))
        in = false;

    if (in)
    {
        row = dirRow;
        col = dirCol;
    }

    return in;
}

//--
void NodeAgent::draw(Image &im)
{
    octet val = 255;

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
        im(dirRow, dirCol) = val;
}

//--
bool operator==(const NodeAgent &anI1, const NodeAgent &anI2)
{
    return anI1.isEqualTo(anI2);
}

//--
bool operator!=(const NodeAgent &anI1, const NodeAgent &anI2)
{
    return !(anI1 == anI2);
}

//--
ostream &operator<<(ostream &os, const NodeAgent &anI)
{
    anI.display(os);
    return os;
}

//--
void NodeAgent::display(ostream &os) const
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
bool NodeAgent::isEqualTo(const NodeAgent &anI) const
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
void NodeAgent::_copy(const NodeAgent &anI)
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
void NodeAgent::_destroy(void)
{
    // Destruction des attributs de la classe ImAgent
    // Exemple : delete _at;
    // Rien...!
}