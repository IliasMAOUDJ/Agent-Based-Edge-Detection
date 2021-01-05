#ifndef _NODE_AGENT_H_
#define _NODE_AGENT_H_

#include <iostream>

#include "MAS.h"
#include <tuple>
#include <vector>
#include "system.h"

using namespace std;

class  NodeAgent : public Agent
{
    DEFCLASS( NodeAgent)

        friend ostream& operator<<(ostream& os, const  NodeAgent& anI);

public:

    // Allocateurs/Desallocateurs

     NodeAgent(System* sys, size_t row, size_t col);
    
     NodeAgent(const  NodeAgent& anI);
     NodeAgent& operator=(const  NodeAgent& anI);
    virtual ~ NodeAgent(void);

    virtual  void live(double dt);


    void drawLine(size_t r1, size_t c1, size_t r2, size_t c2);
    bool getNewPos(size_t& row, size_t& col);       // true si dans l'image
    bool getPosDir(size_t& row, size_t& col) const; // true si dans l'image
    virtual  void draw(Image& im);

    // Comparaisons

    friend  bool operator==(const  NodeAgent& anI1, const  NodeAgent& anI2);
    friend  bool operator!=(const  NodeAgent& anI1, const  NodeAgent& anI2);

    // Inspecteurs/modificateurs

protected:

    // Methodes a appeler par une classe derivee

    // display: a appeler dans une classe derivee      // display est une
    virtual void display(ostream& os) const;           // methode appelee
                                                       // dans operator<<

    bool Contains(const std::vector<tuple<int,int>> &list, tuple<int,int> x);
    // isEqualTo: a appeler dans une classe derivee (dans operator==)
    bool isEqualTo(const  NodeAgent& anI) const;

protected:

private:

    System* _sys;

    // Direction de l'agent   8 1 2
    int _dir;   //                        7 X 3 
                //                        6 5 4

    size_t _row;
    size_t _col;
    int _cycle;
    bool _message;
    vector<tuple<int,int>> _explored;

private:

    // Methodes privees d'allocation/desallocation

    void _copy(const  NodeAgent& anI);
    void _destroy(void);

};
#endif // _NODE_AGENT_H_