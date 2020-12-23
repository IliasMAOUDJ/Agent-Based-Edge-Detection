#ifndef _KirschAGENT_H_
#define _KirschAGENT_H_

#include <iostream>

#include "MAS.h"

#include "system.h"

using namespace std;

class KirschAgent : public Agent
{
    DEFCLASS(KirschAgent)

        friend ostream& operator<<(ostream& os, const KirschAgent& anI);

public:

    // Allocateurs/Desallocateurs

    KirschAgent(System* sys);
    
    KirschAgent(const KirschAgent& anI);
    KirschAgent& operator=(const KirschAgent& anI);
    virtual ~KirschAgent(void);

    virtual  void live(double dt);

    
    bool getNewPos(size_t& row, size_t& col);       // true si dans l'image
    bool getPosDir(size_t& row, size_t& col) const; // true si dans l'image
    virtual  void draw(Image& im);

    // Comparaisons

    friend  bool operator==(const KirschAgent& anI1, const KirschAgent& anI2);
    friend  bool operator!=(const KirschAgent& anI1, const KirschAgent& anI2);

    // Inspecteurs/modificateurs

protected:

    // Methodes a appeler par une classe derivee

    // display: a appeler dans une classe derivee      // display est une
    virtual void display(ostream& os) const;           // methode appelee
                                                       // dans operator<<
    void kirschOperator(void);
    // isEqualTo: a appeler dans une classe derivee (dans operator==)
    bool isEqualTo(const KirschAgent& anI) const;

protected:

private:

    System* _sys;

    // Direction de l'agent   8 1 2
    int _dir;   //                        7 X 3 
                //                        6 5 4

    size_t _row;
    size_t _col;
    bool _edge_found;
private:

    // Methodes privees d'allocation/desallocation

    void _copy(const KirschAgent& anI);
    void _destroy(void);

};
#endif // _EDGE_DETECTION_H_