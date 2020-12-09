#ifndef _A_H_
#define _A_H_

#include <iostream>

#include "MAS.h"

#include "Agent.h"
#include "B.h"

using namespace std;

class A : public Agent,
          public B
{
  DEFCLASS(A)

   friend ostream& operator<<(ostream& os, const A& anA);

 public :

   // Allocateurs/Desallocateurs

            A( /* ### */ );
            A(const A& anA);
            A& operator=(const A& anA);
   virtual ~A(void);

   virtual  void live(double dt);

   // Comparaisons

   friend  bool operator==(const A& anA1, const A& anA2);
   friend  bool operator!=(const A& anA1, const A& anA2);

   // Inspecteurs/modificateurs

 protected :

   // Methodes a appeler par une classe derivee

   // display: a appeler dans une classe derivee      // display est une
   virtual void display(ostream& os) const;           // methode appelee
                                                      // dans operator<<

   // isEqualTo: a appeler dans une classe derivee (dans operator==)
           bool isEqualTo(const A& anA) const;

 protected :

             // ###

 private :

             // ###

 private :

   // Methodes privees d'allocation/desallocation

   void _copy(const A& anA);
   void _destroy(void);

};

#endif // _A_H_
