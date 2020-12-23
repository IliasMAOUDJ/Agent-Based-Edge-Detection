#ifndef _EXPLORATIONAGENT_H_
#define _EXPLORATIONAGENT_H_

#include <iostream>

#include "MAS.h"

#include "system.h"

using namespace std;

class ExplorationAgent : public Agent
{
    DEFCLASS(ExplorationAgent)

        friend ostream& operator<<(ostream& os, const ExplorationAgent& anI);

public:

    // Allocateurs/Desallocateurs

    ExplorationAgent(System* sys);
    ExplorationAgent(const ExplorationAgent& anI);
    ExplorationAgent& operator=(const ExplorationAgent& anI);
    virtual ~ExplorationAgent(void);

    virtual  void live(double dt);

    bool getNewPos(size_t& row, size_t& col);       // true si dans l'image
    bool getPosDir(size_t& row, size_t& col) const; // true si dans l'image
    virtual  void draw(Image& im);

    // Comparaisons

    friend  bool operator==(const ExplorationAgent& anI1, const ExplorationAgent& anI2);
    friend  bool operator!=(const ExplorationAgent& anI1, const ExplorationAgent& anI2);

    // Inspecteurs/modificateurs

protected:

    // Methodes a appeler par une classe derivee

    // display: a appeler dans une classe derivee      // display est une
    virtual void display(ostream& os) const;           // methode appelee
                                                       // dans operator<<

    // isEqualTo: a appeler dans une classe derivee (dans operator==)
    bool isEqualTo(const ExplorationAgent& anI) const;

protected:

private:

    System* _sys;

    // Direction de l'agent   8 1 2
    int _dir;   //                        7 X 3 
                //                        6 5 4

    size_t _row;
    size_t _col;

private:

    // Methodes privees d'allocation/desallocation

    void _copy(const ExplorationAgent& anI);
    void _destroy(void);

};
#endif // _EXPLORATIONAGENT_H_