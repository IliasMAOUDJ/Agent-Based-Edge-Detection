#ifndef __XAFFICHAGE_H
#define __XAFFICHAGE_H


#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <string>

#include "LibImages.h"

#include <stdlib.h>

#define  NB  0
#define COUL 1

#define NBRE_COUL 256

#define BLANC 255
#define ROUGE 254
#define VERT  253
#define BLEU  252

using namespace std;

typedef struct {
                char key;
                int  button; /* 1,2 ou 3 */
                int  row;
                int  col;
                bool onButtonPress;
                bool onButtonRelease;
                bool onButtonMotion;
        } XAffichageEvent;

class XAffichage
{
 public:
  // Constructeur et destructeur

   XAffichage(size_t nbRow, size_t nbCol,
              size_t posX = 0, size_t posY = 0, int nature = NB);
  ~XAffichage();

  void   setLabel(string label);
  string getLabel(void);

  // Methode d'affichage

  void Afficher(const Image&    anImage);
  void Afficher(const ImageRVB& animageRVB);

  // XEvenement: retour d'un int (-1 ou un char) 
  int XEvenement(const Image&    anImage, XAffichageEvent *event=NULL);
  int XEvenement(const ImageRVB& anImageRVB, XAffichageEvent *event=NULL);

 private:
  // Attributs

  // Taille de l'image
  size_t _lignes;
  size_t _colonnes;

  // Coordonnees de la fenetre sur l'ecran
  size_t _posX;
  size_t _posY;

  // Nature de l'affichage NB ou COUL

  int _nature;
  
  // Parametres lies a Xlib
  Window _theWin, _theRoot;
  Display *_theDisplay;
  Visual *_theVisual;
  XImage *_image;
  int _theScreen;
  GC _theGC;
  unsigned long _bpx,_wpx;
  Colormap _theColormap;

  string _label;

  // Profondeur de l'ecran
  int _depth;

  // Et pour afficher une ImageRVB : Im24to8

  Image *Im24to8;
};

#endif
