#ifndef _EDGE_FOLLOWING_AGENT_H_
#define _EDGE_FOLLOWING_AGENT_H_

#include <iostream>

#include "MAS.h"
#include <tuple>
#include <vector>
#include "system.h"

using namespace std;

class  EdgeFollowingAgent : public Agent
{
    DEFCLASS( EdgeFollowingAgent)

        friend ostream& operator<<(ostream& os, const  EdgeFollowingAgent& anI);

public:

    // Allocateurs/Desallocateurs

     EdgeFollowingAgent(System* sys, size_t row, size_t col);
    
     EdgeFollowingAgent(const  EdgeFollowingAgent& anI);
     EdgeFollowingAgent& operator=(const  EdgeFollowingAgent& anI);
    virtual ~ EdgeFollowingAgent(void);

    virtual  void live(double dt);

    
    bool getNewPos(size_t& row, size_t& col);       // true si dans l'image
    bool getPosDir(size_t& row, size_t& col) const; // true si dans l'image
    virtual  void draw(Image& im);

    // Comparaisons

    friend  bool operator==(const  EdgeFollowingAgent& anI1, const  EdgeFollowingAgent& anI2);
    friend  bool operator!=(const  EdgeFollowingAgent& anI1, const  EdgeFollowingAgent& anI2);

    // Inspecteurs/modificateurs

protected:

    // Methodes a appeler par une classe derivee

    // display: a appeler dans une classe derivee      // display est une
    virtual void display(ostream& os) const;           // methode appelee
                                                       // dans operator<<
    bool followEdge(void);

    bool Contains(const std::vector<tuple<int,int>> &list, tuple<int,int> x);
    // isEqualTo: a appeler dans une classe derivee (dans operator==)
    bool isEqualTo(const  EdgeFollowingAgent& anI) const;

protected:

private:

    System* _sys;

    // Direction de l'agent   8 1 2
    int _dir;   //                        7 X 3 
                //                        6 5 4

    size_t _row;
    size_t _col;
    vector<tuple<int,int>> _explored;

private:

    // Methodes privees d'allocation/desallocation

    void _copy(const  EdgeFollowingAgent& anI);
    void _destroy(void);

};
#endif // _EDGE_FOLLOWING_AGENT_H_