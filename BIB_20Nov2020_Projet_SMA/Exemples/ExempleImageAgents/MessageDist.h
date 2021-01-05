#ifndef _MESSAGEDIST_H_
#define _MESSAGEDIST_H_

#include <iostream>

#include "MAS.h"

using namespace std;

class MessageDist : public Message
{
  DEFCLASS(MessageDist)

  friend ostream& operator<<(ostream& os, const MessageDist& aM);

 public :

   // Allocateurs/Desallocateurs

            MessageDist(size_t row, size_t col, bool response);
            MessageDist(size_t row, size_t col, float dist, bool response);
            MessageDist(const MessageDist& aM);
            MessageDist& operator=(const MessageDist& aM);
   virtual ~MessageDist(void);

   // Comparaisons

   friend  bool operator==(const MessageDist& aM1, const MessageDist& aM2);
   friend  bool operator!=(const MessageDist& aM1, const MessageDist& aM2);

   // Inspecteurs/modificateurs

           void setRow(size_t row);
           size_t  getRow(void) const;
           void setCol(size_t col);
           size_t  getCol(void) const;
           void setResponse(bool res);
           bool getResponse(void) const;
           void setDist(float dist);
           float getDist(void) const;

 protected :

   // Methodes a appeler par une classe derivee

   // display: a appeler dans une classe derivee      // display est une
   virtual void display(ostream& os) const;           // methode appelee
                                                      // dans operator<<

   // isEqualTo: a appeler dans une classe derivee (dans operator==)
           bool isEqualTo(const MessageDist& aM) const;

 protected :

      // Pas d'attribut protege

 private :

      // Attribut prive

      size_t _row;
      size_t _col;
      bool _response;
      float _dist;

 private :

   // Methodes privees d'allocation/desallocation

   void _copy(const MessageDist& aM);
   void _destroy(void);

};

#endif // _MessageDist_H_
