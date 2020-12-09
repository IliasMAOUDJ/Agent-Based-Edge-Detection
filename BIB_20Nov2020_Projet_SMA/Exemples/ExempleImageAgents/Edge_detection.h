#ifndef _EDGE_DETECTION_H_
#define _EDGE_DETECTION_H_

#include <iostream>

#include "MAS.h"

#include "system.h"

using namespace std;

class Edge_detection_Agent : public Agent
{
    DEFCLASS(Edge_detection_Agent)

        friend ostream& operator<<(ostream& os, const Edge_detection_Agent& anI);

public:

    // Allocateurs/Desallocateurs

    Edge_detection_Agent(System* sys);
    Edge_detection_Agent(const Edge_detection_Agent& anI);
    Edge_detection_Agent& operator=(const Edge_detection_Agent& anI);
    virtual ~Edge_detection_Agent(void);

    virtual  void live(double dt);

    bool getNewPos(size_t& row, size_t& col);       // true si dans l'image
    bool getPosDir(size_t& row, size_t& col) const; // true si dans l'image
    virtual  void draw(Image& im);

    // Comparaisons

    friend  bool operator==(const Edge_detection_Agent& anI1, const Edge_detection_Agent& anI2);
    friend  bool operator!=(const Edge_detection_Agent& anI1, const Edge_detection_Agent& anI2);

    // Inspecteurs/modificateurs

protected:

    // Methodes a appeler par une classe derivee

    // display: a appeler dans une classe derivee      // display est une
    virtual void display(ostream& os) const;           // methode appelee
                                                       // dans operator<<

    // isEqualTo: a appeler dans une classe derivee (dans operator==)
    bool isEqualTo(const Edge_detection_Agent& anI) const;

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

    void _copy(const Edge_detection_Agent& anI);
    void _destroy(void);

};
#endif // _EDGE_DETECTION_H_