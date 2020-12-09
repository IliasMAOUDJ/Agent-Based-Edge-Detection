#ifndef _IMAGERVB_H_
#define _IMAGERVB_H_

#include <iostream>

#include "LibImages.h"

using namespace std;

class ImageRVB {

      friend ostream& operator<<(ostream& os, const ImageRVB& anImageRVB); 

  public:

    // Allocateurs/Desallocateurs

           ImageRVB(size_t nbRow=512, size_t nbCol=512);
           ImageRVB(const char* fileName);
           ImageRVB(const ImageRVB& anImageRVB);
           ImageRVB(const Image& anImage);                  // I_RVB <- I_NB
           ImageRVB& operator=(const ImageRVB& anImageRVB);
           ImageRVB& operator=(const Image& anImage);       // I_RVB <- I_NB
  virtual ~ImageRVB(void);

           void  setImageSize(size_t nbRow, size_t nbCol);

           void  loadImage(const char* fileName);
           void  saveImage(const char* fileName);

           ImageRVB& operator+=(const ImageRVB& anImageRVB);
    friend ImageRVB  operator+ (const ImageRVB& anImageRVB1,
                                const ImageRVB& anImageRVB2);
           ImageRVB& operator-=(const ImageRVB& anImageRVB);
    friend ImageRVB  operator- (const ImageRVB& anImageRVB1,
                                const ImageRVB& anImageRVB2);

    // Comparaisons

  friend   bool operator==(const ImageRVB& anImageRVB1,
                           const ImageRVB& anImageRVB2);
  friend   bool operator!=(const ImageRVB& anImageRVB1,
                           const ImageRVB& anImageRVB2);

    // Inspecteurs

           size_t  getNbRow(void) const;
           size_t  getNbCol(void) const;

     const Image&  getR(void) const;          // Pour obtenir 
           Image&  getR(void);                // le plan R

     const Image&  getV(void) const;          // Pour obtenir
           Image&  getV(void);                // le plan V

     const Image&  getB(void) const;          // Pour obtenir
           Image&  getB(void);                // le plan B

    // Modificateurs/Inspecteurs

           void  writePix(size_t row, size_t col, octet valR,
                                                  octet valV,
                                                  octet valB);
           void  readPix(size_t row, size_t col, octet& valR,
                                                 octet& valV,
                                                 octet& valB) const;

           void  setImage(octet valR, // Tous les pixels initialises a valR
                          octet valV, //                               valV
                          octet valB);//                               valB

           void  lineImage(int X1,int Y1,  // Dessin d'une ligne
                           int X2,int Y2, octet valR, // (x1,y1)-(x2,y2)
                                          octet valV,
                                          octet valB);

  protected:

  //  display a appeler dans une classe derivee        // display est une
   virtual void display(ostream& os) const;            // methode appelee
                                                       // dans operator<<

  //  isEqualTo a appeler dans une classe derivee          // isEqualTo est une
   virtual bool isEqualTo(const ImageRVB& anImageRVB) const; // methode appelee
                                                             // dans operator==

  private:

     // Attributs prives pour la gestion des images

   size_t _nbRow;
   size_t _nbCol;

   Image *R;
   Image *V;
   Image *B;
   bool  _allocated;

   void _Image2ImageRVB(const Image& anImage); // I_RVB <- I_NB

   void _copy(const ImageRVB& anImageRVB);
   void _destroy(void);

};

#endif // _IMAGERVB_H_
