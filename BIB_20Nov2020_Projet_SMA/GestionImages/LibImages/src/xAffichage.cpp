#include "xAffichage.h"


static int
Conv24to8(const ImageRVB& anImageRVB, Image *anImage,
          int w,int h,int nc,octet *rm,octet *gm,octet *bm);

/*
#define NBRE_COUL 256

#define BLANC 255
#define ROUGE 254
#define VERT  253
#define BLEU  252

*/

// Constructeur et destructeur

XAffichage::XAffichage(size_t nbRow, size_t nbCol,
                       size_t posX, size_t posY, int nature)
  : _lignes(nbRow),
    _colonnes(nbCol),
    _posX(posX),
    _posY(posY),
    _nature(nature)
{
   XColor XCouleur;
  _theDisplay = XOpenDisplay(""); // ""
  _theScreen = DefaultScreen(_theDisplay);
  _theRoot = DefaultRootWindow(_theDisplay);
  _wpx = WhitePixel(_theDisplay, _theScreen);
  _bpx = BlackPixel(_theDisplay, _theScreen);
  _theWin = XCreateSimpleWindow(_theDisplay,
				_theRoot,
				_posX, _posY,
                                _colonnes,
				_lignes,
				8, // Bordure de la fenetre
				_bpx, _wpx);

  _theGC = DefaultGC(_theDisplay, _theScreen);
  XSetForeground(_theDisplay,_theGC,_bpx);
  XSetBackground(_theDisplay,_theGC,_wpx);
  XSetGraphicsExposures(_theDisplay, _theGC, False);
  // Selection des evenements auxquels la fenetre est sensible
  XSelectInput(_theDisplay, _theWin, ExposureMask | KeyPressMask |
               ButtonPressMask | ButtonReleaseMask | ButtonMotionMask);

  _theVisual = DefaultVisual (_theDisplay,_theScreen);
  
  _depth = DefaultDepth(_theDisplay, _theScreen);  

  _image = XCreateImage(_theDisplay,
			_theVisual,
			DefaultDepth( _theDisplay, _theScreen),
			ZPixmap,
			0,
			(char *)NULL,
			_colonnes,
			_lignes,
			8,
			0);

  // Si la profondeur de la fenetre est de 8 plans 
  if(_depth == 8)
    {
      _theColormap = XCreateColormap (_theDisplay,_theRoot,_theVisual,AllocAll);
      
      XCouleur.flags = 7; // DoBlue | DoRed | DoGreen

      /*  Calcul des niveaux de gris associes aux couleurs                 */
      for (unsigned long i=0; i< NBRE_COUL; i++)
	{
	  XCouleur.pixel = i;
	  // Declage de 8 bits pour maximiser le contraste
	  XCouleur.red   = XCouleur.green = XCouleur.blue = i<<8;
	  XStoreColor(_theDisplay,_theColormap,&XCouleur);
	}
      
      if(_nature == COUL)
	{
	  XCouleur.pixel = (unsigned long) (BLANC);
	  XCouleur.red   = 65535;
	  XCouleur.green = 65535;
	  XCouleur.blue  = 65535;
	  XStoreColor (_theDisplay,_theColormap,&XCouleur);

	  XCouleur.pixel = (unsigned long) (ROUGE);
	  XCouleur.red   = 65535;
	  XCouleur.green = 0;
	  XCouleur.blue  = 0;
	  XStoreColor (_theDisplay,_theColormap,&XCouleur);
	  
	  XCouleur.pixel = (unsigned long) (VERT);
	  XCouleur.red   = 0;
	  XCouleur.green = 65535;
	  XCouleur.blue  = 0;
	  XStoreColor (_theDisplay,_theColormap,&XCouleur);
	  
	  XCouleur.pixel = (unsigned long) (BLEU);
	  XCouleur.red   = 0;
	  XCouleur.green = 0;
	  XCouleur.blue  = 65535;
	  XStoreColor (_theDisplay,_theColormap,&XCouleur);
	}

      // Allocation de la memoire necessaire a l'image
      _image->data = (char *) malloc (_lignes*_colonnes*sizeof(char));
    }
  // Si la profondeur de la fenetre est de 16 plans 
  if(_depth == 16)
    {
      _theColormap = XCreateColormap (_theDisplay,
				      _theRoot,
				      _theVisual,
				      AllocNone);
      //  Allocation de la memoire necessaire a l'image
      _image->data = (char *) malloc (2*_lignes*_colonnes*sizeof(char));
    }


  // Si la profondeur de la fenetre est de 24 plans 
  if(_depth == 24)
    {
      _theColormap = XCreateColormap (_theDisplay,
				      _theRoot,
				      _theVisual,
				      AllocNone);
      //  Allocation de la memoire necessaire a l'image
      _image->data = (char *) malloc (4*_lignes*_colonnes*sizeof(char)); // 3
    }

  XSetWindowColormap(_theDisplay,_theWin,_theColormap);


  XMapWindow(_theDisplay,_theWin);

  Im24to8 = new Image(_lignes,_colonnes);
}

 
XAffichage::~XAffichage ()
{
  XDestroyImage(_image);
  XCloseDisplay(_theDisplay);

  delete Im24to8;
}

void XAffichage::setLabel(string label)
{
 _label = label;
 XStoreName(_theDisplay,_theWin,_label.c_str());
}

string XAffichage::getLabel(void)
{
 return _label;
}

// Methodes d'affichage

void XAffichage::Afficher(const Image& anImage)
{
  const octet *src = anImage.begin();
  int taille = _lignes * _colonnes;

  // Si la profondeur de la fenetre est de 8 bits
  if(_depth == 8)
    {
      char *currentAddr = _image->data;

      for (int i = 0; i < taille; i++)
      {
       *currentAddr++ = *src;
        src++;
      }
    }


  // Si la profondeur de la fenetre est de 16 plans 
  if(_depth == 16)
    {
      short *currentAddr = (short *)_image->data; 

      // Si l'affichage est en niveaux de gris
      if (_nature == NB)

	for (int i = 0; i < taille; i++)   
	  {
	    *currentAddr++ = ((*src)*31/255 << 11)
	                   | (((*src)*63/255) << 5)
	                   | ((*src)*31/255);
	    src++;
	  }

      // Si l'affichage est en couleurs 
      else
	
	for (int i = 0; i < taille; i++)   
	  {

          if(*src == ROUGE)
                *currentAddr = (short)0xF800;
          else 
          if(*src == VERT)
                *currentAddr = (short)0x7E0;
          else
          if(*src == BLEU)
                *currentAddr = (short)0x1F;
          else
                *currentAddr = (short)(((*src)*31/255 << 11)
                           | (((*src)*63/255) << 5)
                           | ((*src)*31/255));
	  currentAddr++;
	  src++;

	  }

    }
  

  // Si la profondeur de la fenetre est de 24 plans 
  if(_depth == 24)
    {
      char *currentAddr = (char *)_image->data; 
      // Si l'affichage est en niveaux de gris
      if (_nature == NB)
      {
	for (int i = 0; i < taille; i++)   
	  {
	    *currentAddr++ = (char)*src;
	    *currentAddr++ = (char)*src;
	    *currentAddr++ = (char)*src;
             currentAddr++;
	     src++;
	  }
      }

      // Si l'affichage est en couleurs 
      else
      {
	for (int i = 0; i < taille; i++)   
	  {
            if (*src == ROUGE)
	    {
	      *currentAddr++ = (char)0;
	      *currentAddr++ = (char)0;
	      *currentAddr++ = (char)255;
               currentAddr++;
            }
            else
            if(*src == VERT)
            {
              *currentAddr++ = (char)0;
              *currentAddr++ = (char)255;
              *currentAddr++ = (char)0;
               currentAddr++;
            }
            else
	    if(*src == BLEU) 
	    {
	      *currentAddr++ = (char)255;
	      *currentAddr++ = (char)0;
	      *currentAddr++ = (char)0;
               currentAddr++;
	    }
            else
            {
	      *currentAddr++ = (char)*src;
	      *currentAddr++ = (char)*src;
	      *currentAddr++ = (char)*src;
               currentAddr++;
            }
	  src++;
	  }
       }
    }

  XPutImage (_theDisplay,
	     _theWin,
	     _theGC,
	     _image,0,0,0,0,
             _colonnes,
	     _lignes
            );
}


void XAffichage::Afficher(const ImageRVB& anImageRVB)
{
 int coul=1; /* coul=1: Si _depth=8 => conversion "couleur" 24bits -> 8bits */
             /* coul=0: Si _depth=8 => conversion "R+V+B/3" 24bits -> 8bits */

 if(_depth == 8)
 {
  XColor XCouleur;

  octet red[256];
  octet green[256];
  octet blue[256];

  if (coul)
  {
   Conv24to8(anImageRVB,Im24to8,
             anImageRVB.getNbCol(),anImageRVB.getNbRow(),256,red,green,blue);

   for (unsigned long i=0; i< 256; i++)
   {
     XCouleur.pixel = i;

          // Declage de 8 bits pour maximiser le contraste

     XCouleur.red   = ((unsigned long)red[i])<<8;
     XCouleur.green = ((unsigned long)green[i])<<8;
     XCouleur.blue  = ((unsigned long)blue[i])<<8;

     XStoreColor(_theDisplay,_theColormap,&XCouleur);
   }

   XSetWindowColormap(_theDisplay,_theWin,_theColormap);
  }
  else *Im24to8 = anImageRVB;

  Afficher(*Im24to8);
 }

 if(_depth == 16)
 {
  int taille = _lignes * _colonnes;

  const octet *srcR = anImageRVB.getR().begin();         
  const octet *srcV = anImageRVB.getV().begin();         
  const octet *srcB = anImageRVB.getB().begin();         

  short *currentAddr = (short *)_image->data; 

  for (int i = 0; i < taille; i++)
  {
   *currentAddr++ = (short)(((*srcR++)*31/255 << 11)
                  | (((*srcV++)*63/255) << 5)
                  | ((*srcB++)*31/255));
  }
  XPutImage (_theDisplay,
             _theWin,
             _theGC,
             _image,0,0,0,0,
             _colonnes,
             _lignes
            );
 }
 if(_depth == 24)
 {
  int taille = _lignes * _colonnes;

  const octet *srcR = anImageRVB.getR().begin();         
  const octet *srcV = anImageRVB.getV().begin();         
  const octet *srcB = anImageRVB.getB().begin();         

  char *currentAddr = (char *)_image->data; 
  for (int i = 0; i < taille; i++)   
   {
      *currentAddr++ = (char)*srcB++;
      *currentAddr++ = (char)*srcV++;
      *currentAddr++ = (char)*srcR++;
       currentAddr++;
   }
   XPutImage (_theDisplay,
              _theWin,
              _theGC,
              _image,0,0,0,0,
              _colonnes,
              _lignes
             );
 }
}


int XAffichage::XEvenement(const Image& anImage, XAffichageEvent *event)
{
  int returnValue = -1;
  static int butNumber = -1;

  if (event!=NULL) {
                    event->key=-1;
                    event->button=-1;
                    event->row=-1;
                    event->col=-1;
                    event->onButtonPress=false;
                    event->onButtonRelease=false;
                    event->onButtonMotion=false;
  }

  if (XPending(_theDisplay)) {
    XEvent evt;
    int i;
    char text[2];
    KeySym theKey;
      
    XNextEvent(_theDisplay,&evt);
    
    switch ( evt.type )
      {
      case Expose :
	if( evt.xexpose.window == _theWin)
	  Afficher(anImage);
	break;
	
      case KeyPress :
	i = XLookupString(&(evt.xkey), text, 2, &theKey, 0);
	if (i==1) { returnValue=*text;
                    if (event!=NULL) event->key = returnValue;
        }
	break;
      case ButtonPress:
        {
         XButtonEvent *but_event = (XButtonEvent *) &evt;
         if (but_event->window == _theWin)
         {
          if (but_event->button == Button1) butNumber=1;
          else
          if (but_event->button == Button2) butNumber=2;
          else
          if (but_event->button == Button3) butNumber=3;
          else butNumber=3;  /* Si pb */

          if (event!=NULL) {
            int nbRows=_lignes;
            int nbCols=_colonnes;
            event->onButtonPress=true;
            event->button=butNumber;
            if (but_event->x>=0 && but_event->y>=0 &&
                but_event->x<nbCols && but_event->y<nbRows)
            {
             event->row=but_event->y; event->col=but_event->x;
            } 
          }
         }
         break;
        }
      case ButtonRelease:
        {
         XButtonEvent *but_event = (XButtonEvent *) &evt;
         if (but_event->window == _theWin)
         {
          if (but_event->button == Button1) butNumber=1;
          else
          if (but_event->button == Button2) butNumber=2;
          else
          if (but_event->button == Button3) butNumber=3;
          else butNumber=3;  /* Si pb */

          if (event!=NULL) {
            int nbRows=_lignes;
            int nbCols=_colonnes;
            event->onButtonRelease=true;
            event->button=butNumber;
            if (but_event->x>=0 && but_event->y>=0 &&
                but_event->x<nbCols && but_event->y<nbRows)
            {
             event->row=but_event->y; event->col=but_event->x;
            }
          }
         }
         break;
        }
      case MotionNotify:
        {
         XMotionEvent *mot_event = (XMotionEvent*) &evt;
         if (mot_event->window == _theWin)
         {
          if (event!=NULL) {
            int nbRows=_lignes;
            int nbCols=_colonnes;
            event->onButtonMotion=true;
            event->button=butNumber;
            if (mot_event->x>=0 && mot_event->y>=0 &&
                mot_event->x<nbCols && mot_event->y<nbRows)
            {
             event->row=mot_event->y; event->col=mot_event->x;
            }
          }
         }
         break;
        }
      }
#if 0
          if (event!=NULL)
          {
           printf("--------------- NOIR ET BLANC ------------>\n");
           if (event->key==-1) printf("key=(NONE)\n");
           else printf("key=%c\n",event->key);
           printf("Button=%d\n",event->button);
           printf("row=%d col=%d\n",event->row,event->col);
           if (event->onButtonPress)   printf("onButtonPress=true\n");
           else                        printf("onButtonPress=false\n");
           if (event->onButtonRelease) printf("onButtonRelease=true\n");
           else                        printf("onButtonRelease=false\n");
           if (event->onButtonMotion)  printf("onButtonMotion=true\n");
           else                        printf("onButtonMotion=false\n");
           printf("<------------------------------------------\n");
          }
#endif
  }

  return returnValue;
}

int XAffichage::XEvenement(const ImageRVB& anImageRVB, XAffichageEvent *event)
{
  int returnValue = -1;
  static int butNumber = -1;

  if (event!=NULL) {
                    event->key=-1;
                    event->button=-1;
                    event->row=-1;
                    event->col=-1;
                    event->onButtonPress=false;
                    event->onButtonRelease=false;
                    event->onButtonMotion=false;
  }

  if (XPending(_theDisplay)) {
    XEvent evt;
    int i;
    char text[2];
    KeySym theKey;

    XNextEvent(_theDisplay,&evt);

    switch ( evt.type )
      {
      case Expose :
        if( evt.xexpose.window == _theWin)
          Afficher(anImageRVB);
        break;

      case KeyPress :
        i = XLookupString(&(evt.xkey), text, 2, &theKey, 0);
	if (i==1) { returnValue=*text;
                    if (event!=NULL) event->key = returnValue;
        }
        break;
      case ButtonPress:
        {
         XButtonEvent *but_event = (XButtonEvent *) &evt;
         if (but_event->window == _theWin)
         {
          if (but_event->button == Button1) butNumber=1;
          else
          if (but_event->button == Button2) butNumber=2;
          else
          if (but_event->button == Button3) butNumber=3;
          else butNumber=3;  /* Si pb */

          if (event!=NULL) {
            int nbRows=_lignes;
            int nbCols=_colonnes;
            event->onButtonPress=true;
            event->button=butNumber;
            if (but_event->x>=0 && but_event->y>=0 &&
                but_event->x<nbCols && but_event->y<nbRows)
            {
             event->row=but_event->y; event->col=but_event->x;
            } 
          }
         }
         break;
        }
      case ButtonRelease:
        {
         XButtonEvent *but_event = (XButtonEvent *) &evt;
         if (but_event->window == _theWin)
         {
          if (but_event->button == Button1) butNumber=1;
          else
          if (but_event->button == Button2) butNumber=2;
          else
          if (but_event->button == Button3) butNumber=3;
          else butNumber=3;  /* Si pb */

          if (event!=NULL) {
            int nbRows=_lignes;
            int nbCols=_colonnes;
            event->onButtonRelease=true;
            event->button=butNumber;
            if (but_event->x>=0 && but_event->y>=0 &&
                but_event->x<nbCols && but_event->y<nbRows)
            {
             event->row=but_event->y; event->col=but_event->x;
            }
          }
         }
         break;
        }
      case MotionNotify:
        {
         XMotionEvent *mot_event = (XMotionEvent*) &evt;
         if (mot_event->window == _theWin)
         {
          if (event!=NULL) {
            int nbRows=_lignes;
            int nbCols=_colonnes;
            event->onButtonMotion=true;
            event->button=butNumber;
            if (mot_event->x>=0 && mot_event->y>=0 &&
                mot_event->x<nbCols && mot_event->y<nbRows)
            {
             event->row=mot_event->y; event->col=mot_event->x;
            }
          }
         }
         break;
        }
      }
#if 0
          if (event!=NULL)
          {
           printf("-------------- COULEUR ------------------->\n");
           if (event->key==-1) printf("key=(NONE)\n");
           else printf("key=%c\n",event->key);
           printf("Button=%d\n",event->button);
           printf("row=%d col=%d\n",event->row,event->col);
           if (event->onButtonPress)   printf("onButtonPress=true\n");
           else                        printf("onButtonPress=false\n");
           if (event->onButtonRelease) printf("onButtonRelease=true\n");
           else                        printf("onButtonRelease=false\n");
           if (event->onButtonMotion)  printf("onButtonMotion=true\n");
           else                        printf("onButtonMotion=false\n");
           printf("<------------------------------------------\n");
          }
#endif
  }

  return returnValue;
}

/* Et pour Conv24to8 : */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* RANGE forces a to be in the range b..c (inclusive) */
#define RANGE(a,b,c) { if (a < b) a = b;  if (a > c) a = c; }

#define CONV24_FAST  5
#define CONV24_FAST  5
#define CONV24_SLOW  6


int noqcheck=0;
int conv24=CONV24_FAST;  /* CONV24_FAST, CONV24_BEST, CONV24_SLOW */

static
int
Conv24to8(const ImageRVB& anImageRVB, Image *anImage,
          int w,int h,int nc,octet *rm,octet *gm,octet *bm);

static
int
quick_check(const ImageRVB& anImageRVB, Image *anImage,
            int w,int h,
            octet *rmap,octet *gmap,octet *bmap, int maxcol);

static
int
quick_quant(const ImageRVB& anImageRVB, Image *anImage,
            int w,int h,
            octet *rmap,octet *gmap,octet *bmap, int nc);

/*
 * xv24to8.c  -  contains the 24-to-8-bit Conv24to8() procedure
 *               and the 8-to-24-bit Conv8to24() procedure
 *
 * The Conv24to8 procedure takes a pointer to a 24-bit image (loaded
 * previously).  The image will be a w * h * 3 octet array of
 * octets.  The image will be arranged with 3 octets per pixel (in order
 * R, G, and B), pixel 0 at the top left corner.  (As normal.)
 * The procedure also takes a maximum number of colors to use (numcols)
 * and pointers to three 256-long arrays of octets (to hold the returned
 * colormap)
 *
 * Note that Conv24to8() does NOT free the pic24 image under any circumstances
 *
 * The Conv24to8 procedure will set up the following:  it will allocate, make
 * & return 'pic8', a 'w' by 'h' (passed in values) 8-bit picture.
 * it will load up the rmap, gmap and bmap colormap arrays.  it will NOT
 * calculate numcols, since the cmap sort procedure has to be called anyway
 *
 * Conv24to8 returns 'pic8' if successful, 'NULL' on failure (presumably on a
 * malloc())
 *
 * The 'slow' code, while still based on Heckbert's Median Cut algorithm,
 * has been shamelessly lifted from the Independent JPEG Group's software
 * (jquant2.c), as (for a variety of reasons) theirs was far better than
 * the version I was previously using.  Thanks guys!
 *
 * Also, as is my way, I've stripped out most of the IJG's well-written
 * comments regarding their algorithm.  Folks interested in learning how it
 * works are encouraged to look at the original source.  (jpeg/jquant2.c)
 *
 * contains:
 *   Cont24to8()
 *   Init24to8()
 */


static
int  /* Retourne 0 si ok , -1 sinon */
Conv24to8(const ImageRVB& anImageRVB,Image *anImage,
          int w,int h,int nc,octet *rm,octet *gm,octet *bm)
{
  int   i;

  anImage->setImageSize(h,w); // Si necessaire

  if (nc<=0) nc = 255;  /* 'nc == 0' breaks code */

  if (!noqcheck && quick_check(anImageRVB,anImage,w,h, rm,gm,bm, nc)) {
    /*fprintf(stderr,"No color compression was necessary.\n");*/
    return 0;
  }

  switch (conv24) {
  case CONV24_FAST:
    /*fprintf(stderr,"Doing 'quick' 24-bit to 8-bit conversion.\n");*/
    i = quick_quant(anImageRVB,anImage,w, h, rm, gm, bm, nc);
    break;

/*
  case CONV24_BEST:
    fprintf(stderr,"Doing 'best' 24-bit to 8-bit conversion.\n");
    i = ppm_quant(anImageRVB,anImage, w, h, rm, gm, bm, nc);
    break;

  case CONV24_SLOW:
  default:
    fprintf(stderr,"Doing 'slow' 24-bit to 8-bit conversion.\n");
    i = slow_quant(anImageRVB,anImage, w, h, im, rm, gm, bm, nc);
    break;
*/
  }

  if (i) { return -1; }
  return 0;
}

static
int
quick_check(const ImageRVB& anImageRVB, Image *anImage,
            int w,int h,
            octet *rmap,octet *gmap,octet *bmap, int maxcol)
{
  /* scans picture until it finds more than 'maxcol' different colors.  If it
     finds more than 'maxcol' colors, it returns '0'.  If it DOESN'T, it does
     the 24-to-8 conversion by simply sticking the colors it found into
     a colormap, and changing instances of a color in pic24 into colormap
     indicies (in pic8) */

  unsigned long colors[256],col;
  int           i, nc, low, high, mid;
  const octet  *pR, *pV, *pB;
  octet        *pix;

  if (maxcol>256) maxcol = 256;

  /* put the first color in the table by hand */
  nc = 0;  mid = 0;  

  pR = anImageRVB.getR().begin();         
  pV = anImageRVB.getV().begin();         
  pB = anImageRVB.getB().begin();         

  for (i=w*h; i; i--) {
    col  = (((u_long) *pR++) << 16);  
    col += (((u_long) *pV++) << 8);
    col +=  *pB++;

    /* binary search the 'colors' array to see if it's in there */
    low = 0;  high = nc-1;
    while (low <= high) {
      mid = (low+high)/2;
      if      (col < colors[mid]) high = mid - 1;
      else if (col > colors[mid]) low  = mid + 1;
      else break;
    }

    if (high < low) { /* didn't find color in list, add it. */
      if (nc>=maxcol) return 0;
      memmove((char *) &colors[low+1],
              (char *) &colors[low],
	      (nc - low) * sizeof(u_long));
      colors[low] = col;
      nc++;
    }
  }

  /* run through the data a second time, this time mapping pixel values in
     pic24 into colormap offsets into 'colors' */

  pR = anImageRVB.getR().begin();
  pV = anImageRVB.getV().begin();
  pB = anImageRVB.getB().begin();

  pix=anImage->begin();

  for (i=w*h; i; i--,pix++) {
    col  = (((u_long) *pR++) << 16);  
    col += (((u_long) *pV++) << 8);
    col +=  *pB++;

    /* binary search the 'colors' array.  It *IS* in there */
    low = 0;  high = nc-1;
    while (low <= high) {
      mid = (low+high)/2;
      if      (col < colors[mid]) high = mid - 1;
      else if (col > colors[mid]) low  = mid + 1;
      else break;
    }

    if (high < low) {
      fprintf(stderr,"quick_check:  impossible situation!\n");
      exit(1);
    }
    *pix = mid;
  }

  /* and load up the 'desired colormap' */
  for (i=0; i<nc; i++) {
    rmap[i] =  colors[i]>>16;  
    gmap[i] = (colors[i]>>8) & 0xff;
    bmap[i] =  colors[i]     & 0xff;
  }

  return 1;
}

static
int
quick_quant(const ImageRVB& anImageRVB, Image *anImage,
            int w,int h,
            octet *rmap,octet *gmap,octet *bmap, int nc)
{
  /* called after 'pic8' (im) has been alloced, pWIDE,pHIGH set up, mono/1-bit
     checked already */
  
/* up to 256 colors:     3 bits R, 3 bits G, 2 bits B  (RRRGGGBB) */
#define RMASK      0xe0
#define RSHIFT        0
#define GMASK      0xe0
#define GSHIFT        3
#define BMASK      0xc0
#define BSHIFT        6

  octet *pp;
  int  r1, g1, b1;
  int  *thisline, *nextline, *thisptr, *nextptr, *tmpptr;
  int  i, j, val, pwide3;
  int  imax, jmax;

  const octet    *pR, *pV, *pB;
  int indRVB=0;

  (void)nc;

  pp = anImage->begin();
  pwide3 = w * 3;  imax = h-1;  jmax = w-1;

  /* load up colormap:
   *   note that 0 and 255 of each color are always in the map;
   *   intermediate values are evenly spaced.
   */

  for (i=0; i<256; i++) {
    rmap[i] = (((i<<RSHIFT) & RMASK) * 255 + RMASK/2) / RMASK;
    gmap[i] = (((i<<GSHIFT) & GMASK) * 255 + GMASK/2) / GMASK;
    bmap[i] = (((i<<BSHIFT) & BMASK) * 255 + BMASK/2) / BMASK;
  }

  thisline = (int *) malloc(pwide3 * sizeof(int));
  nextline = (int *) malloc(pwide3 * sizeof(int));
  if (!thisline || !nextline) {
    if (thisline) free(thisline);
    if (nextline) free(nextline);
    fprintf(stderr,"unable to allocate memory in quick_quant()\n");
    return(1);
  }

  pR = anImageRVB.getR().begin();         
  pV = anImageRVB.getV().begin();         
  pB = anImageRVB.getB().begin();         
  
  /* get first line of picture */
  /* for (j=pwide3, tmpptr=nextline; j; j--) *tmpptr++ = (int) *p24++; */

  indRVB=0;
  for (j=pwide3, tmpptr=nextline; j; j--) 
  {
   if (indRVB==0) *tmpptr++ = (int) *pR++;
   if (indRVB==1) *tmpptr++ = (int) *pV++;
   if (indRVB==2) *tmpptr++ = (int) *pB++;
   indRVB++;
   if (indRVB==3) indRVB=0;
  }
  
  for (i=0; i<h; i++) {
    tmpptr = thisline;  thisline = nextline;  nextline = tmpptr;   /* swap */
    
    /* if ((i&0x3f) == 0) WaitCursor(); */

    if (i!=imax)   /* get next line */
    {
     /*
      for (j=pwide3, tmpptr=nextline; j; j--)
	*tmpptr++ = (int) *p24++;
     */
     indRVB=0;
     for (j=pwide3, tmpptr=nextline; j; j--) 
     {
      if (indRVB==0) *tmpptr++ = (int) *pR++;
      if (indRVB==1) *tmpptr++ = (int) *pV++;
      if (indRVB==2) *tmpptr++ = (int) *pB++;
      indRVB++;
      if (indRVB==3) indRVB=0;
     }
    }
    
    for (j=0, thisptr=thisline, nextptr=nextline; j<w; j++,pp++) {
      r1 = *thisptr++;  g1 = *thisptr++;  b1 = *thisptr++;
      RANGE(r1,0,255);  RANGE(g1,0,255);  RANGE(b1,0,255);  
      
      /* choose actual pixel value */
      val = (((r1&RMASK)>>RSHIFT) | ((g1&GMASK)>>GSHIFT) | 
	     ((b1&BMASK)>>BSHIFT));
      *pp = val;
      
      /* compute color errors */
      r1 -= rmap[val];
      g1 -= gmap[val];
      b1 -= bmap[val];
      
      /* Add fractions of errors to adjacent pixels */
      if (j!=jmax) {  /* adjust RIGHT pixel */
	thisptr[0] += (r1*7) / 16;
	thisptr[1] += (g1*7) / 16;
	thisptr[2] += (b1*7) / 16;
      }
      
      if (i!=imax) {	/* do BOTTOM pixel */
	nextptr[0] += (r1*5) / 16;
	nextptr[1] += (g1*5) / 16;
	nextptr[2] += (b1*5) / 16;

	if (j>0) {  /* do BOTTOM LEFT pixel */
	  nextptr[-3] += (r1*3) / 16;
	  nextptr[-2] += (g1*3) / 16;
	  nextptr[-1] += (b1*3) / 16;
	}

	if (j!=jmax) {  /* do BOTTOM RIGHT pixel */
	  nextptr[3] += (r1)/16;
	  nextptr[4] += (g1)/16;
	  nextptr[5] += (b1)/16;
	}
	nextptr += 3;
      }
    }
  }
  
  free(thisline);
  free(nextline);
  return 0;


#undef RMASK
#undef RSHIFT
#undef GMASK
#undef GSHIFT
#undef BMASK
#undef BSHIFT
}
