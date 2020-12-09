#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <iostream>

#include "LibImages.h"

using namespace std;

class Image {

      friend ostream& operator<<(ostream& os, const Image& anImage); 

  public:

    // Allocateurs/Desallocateurs

           Image(size_t nbRow=512, size_t nbCol=512);
           Image(const char* fileName);
           Image(const Image& anImage);
           Image(const ImageRVB& anImageRVB);                // I_NB <- I_RVB
           Image& operator=(const Image& anImage);
           Image& operator=(const ImageRVB& anImageRVB);     // I_NB <- I_RVB
  virtual ~Image(void);

           void  setImageSize(size_t nbRow, size_t nbCol);

           void  loadImage(const char* fileName);
           void  saveImage(const char* fileName);

    // Operateurs

           Image& operator+=(const Image& anImage);
    friend Image  operator+ (const Image& anImage1,const Image& anImage2);
           Image& operator-=(const Image& anImage);
    friend Image  operator- (const Image& anImage1,const Image& anImage2);

    // Comparaisons

  friend   bool operator==(const Image& anImage1, const Image& anImage2);
  friend   bool operator!=(const Image& anImage1, const Image& anImage2);

    // Inspecteurs

           size_t  getNbRow(void) const;
           size_t  getNbCol(void) const;

     const octet*  begin(void) const; // Un pointeur sur le DEBUT
           octet*  begin(void);       // de la zone image

     const octet*  end(void) const;   // Un pointeur sur la FIN
           octet*  end(void);         // de la zone image

    // Modificateurs/Inspecteurs

           void  writePix(size_t row, size_t col, octet val);
           void  readPix(size_t row, size_t col, octet& val) const;
           octet readPix(size_t row, size_t col) const;

           const octet* operator[] (size_t row) const;    // Pour autoriser
                 octet* operator[] (size_t row);          // anImage[row][col]

           octet  operator() (size_t row, size_t col) const; // Pour autoriser
           octet& operator() (size_t row, size_t col);     // anImage(row,col)

           void  setImage(octet val); // Tous les pixels initialises a val

           void  lineImage(int X1,int Y1,  // Dessin d'une ligne
                           int X2,int Y2,octet val); // (x1,y1)-(x2,y2)

  protected:

  //  display a appeler dans une classe derivee        // display est une
   virtual void display(ostream& os) const;            // methode appelee
                                                       // dans operator<<

  //  isEqualTo a appeler dans une classe derivee      // isEqualTo est une
   virtual bool isEqualTo(const Image& anImage) const; // methode appelee
                                                       // dans operator==

  private:

     // Attributs prives pour la gestion des images

   size_t _nbRow;
   size_t _nbCol;

   octet*  _begin;        // Le "vrai" pointeur sur le DEBUT de la zone image
   octet*  _end ;         // Le "vrai" pointeur sur la FIN de la zone image

   octet** _zone;         // Un pointeur qui permet : zone[ligne][colonne]

   void _mallocImage(size_t nbRow, size_t nbCol); // _mallocImage appelle 
   void _freeImage(void);                         // _freeImage si necessaire

   void _ImageRVB2Image(const ImageRVB& anImageRVB); // I_NB <- I_RVB

   void _copy(const Image& anImage);
   void _destroy(void);

};

#endif // _IMAGE_H_
