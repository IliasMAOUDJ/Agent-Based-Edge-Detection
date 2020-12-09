#include <stdio.h>
#include <string.h>
#include "Image.h"

#define absT(x) (((x)>0)?(x):(-(x)))

ostream& operator<<(ostream& os, const Image& anImage)
{
 anImage.display(os);
 return os;
}

Image::Image(size_t nbRow, size_t nbCol)
{
 _nbRow = _nbCol = 0;
 _begin = _end = NULL; _zone = NULL;

  setImageSize(nbRow,nbCol); // Init de _nbRow, _nbCol, _ptrZone, _zone
}

Image::Image(const char* fileName)
{
 _nbRow = _nbCol = 0;
 _begin = _end = NULL; _zone = NULL;

 loadImage(fileName);          // En interne un setImageSize
}

Image::Image(const Image& anImage)
{
 _nbRow = _nbCol = 0;
 _begin = _end = NULL; _zone = NULL;

 _copy(anImage);               // En interne un setImageSize
}

Image::Image(const ImageRVB& anImageRVB)
{
 _nbRow = _nbCol = 0;
 _begin = _end = NULL; _zone = NULL;

 _ImageRVB2Image(anImageRVB);   // En interne un setImageSize
}

Image& Image::operator=(const Image& anImage)
{
 if (this != &anImage)
 {
//_destroy() 
  _copy(anImage);              // En interne un setImageSize
 }

 return *this;
}

Image& Image::operator=(const ImageRVB& anImageRVB)
{
 _ImageRVB2Image(anImageRVB); // En interne un setImageSize

 return *this;
}

Image::~Image(void)
{
 _destroy();
}

void Image::setImageSize(size_t nbRow, size_t nbCol)
{
 _mallocImage(nbRow,nbCol);
}

void Image::loadImage(const char* fileName)
{
 loadImageFile(fileName,*this);             // En interne un setImageSize
}

void Image::saveImage(const char* fileName)
{
 saveImageFile(fileName,*this);
}

//-- operateur +=, +, -= et -
Image& Image::operator+=(const Image& anImage)
{
 if (_nbRow!=anImage._nbRow || _nbCol!=anImage._nbCol)
 {
  cerr << "Warning : Image::operator+=, "
       << "size of *this != size of anImage !" << endl;
  cerr << "... do nothing ... " << endl;
 }
 else
 {
  octet* ptrImThis; 
  octet* ptrAnImage; 
  for(ptrImThis = _begin, ptrAnImage = anImage._begin;
      ptrImThis < _end; ptrImThis++, ptrAnImage++)
  {
   int val = *ptrImThis + *ptrAnImage;
   if (val>255) val=255;
   *ptrImThis = (octet)val;
  }
 }

 return *this;
}
//--
Image operator+(const Image& anImage1,const Image& anImage2) // friend
{
 Image im(anImage1);
 im += anImage2;
 return im;
}
//--
Image& Image::operator-=(const Image& anImage)
{
 if (_nbRow!=anImage._nbRow || _nbCol!=anImage._nbCol)
 {
  cerr << "Warning : Image::operator-=, "
       << "size of *this != size of anImage !" << endl;
  cerr << "... do nothing ... " << endl;
 }
 else
 {
  octet* ptrImThis;
  octet* ptrAnImage;
  for(ptrImThis = _begin, ptrAnImage = anImage._begin;
      ptrImThis < _end; ptrImThis++, ptrAnImage++)
  {
   int val = *ptrImThis - *ptrAnImage;
   if (val<0) val=0;
   *ptrImThis = (octet)val;
  }
 }

 return *this;
}
//--
Image operator-(const Image& anImage1,const Image& anImage2) // friend
{
 Image im(anImage1);
 im -= anImage2;
 return im;
}
//--

bool operator==(const Image& anImage1, const Image& anImage2)
{
 return anImage1.isEqualTo(anImage2);
}

bool operator!=(const Image& anImage1, const Image& anImage2)
{
 return !(anImage1==anImage2);
}

size_t Image::getNbRow(void) const
{
 return _nbRow;
}

size_t Image::getNbCol(void) const
{
 return _nbCol;
}

const octet* Image::begin(void) const
{
 return _begin;
}

octet* Image::begin(void)
{
 return _begin;
}

const octet* Image::end(void) const
{
 return _end;
}

octet* Image::end(void)
{
 return _end;
}

void Image::writePix(size_t row, size_t col, octet val)
{
 if (row >= _nbRow || col >= _nbCol) {
//cerr << "Warning, Image::writePix wrong parameter value (row or col)\n";
  return;
 }

 _zone[row][col] = val;
}

void Image::readPix(size_t row, size_t col, octet& val) const
{
 if (row >= _nbRow || col >= _nbCol) {
//cerr << "Warning, Image::readPix wrong parameter value (row or col)\n";
  return;
 }

 val = _zone[row][col];
}

octet Image::readPix(size_t row, size_t col) const
{
 if (row >= _nbRow || col >= _nbCol) {
//cerr << "Warning, Image::readPix wrong parameter value\n";
  return 0;
 }
 
 return _zone[row][col];
}

const octet* Image::operator[] (size_t row) const
{
 return _zone[row];
}

octet* Image::operator[] (size_t row)
{
 return _zone[row];
}

octet  Image::operator() (size_t row, size_t col) const
{
 if (row >= _nbRow || col >= _nbCol) {
//cerr << "Warning, Image::operator() wrong parameter value\n";
  return 0;
 }

 return _zone[row][col];
}

octet&  Image::operator() (size_t row, size_t col)
{
 static octet Zero = 0;

 if (row >= _nbRow || col >= _nbCol) {
//cerr << "Warning, Image::operator() wrong parameter value\n";
  return Zero;
 }

 return _zone[row][col];
}

void  Image::setImage(octet val)
{
 for(octet* ptrIm = _begin; ptrIm < _end; ptrIm++)
 {
  *ptrIm = val;
 }
}

void  Image::lineImage(int X1,int Y1,int X2,int Y2,octet val)
{
 int dx,dy,sx,sy;
 size_t x,y;
 double a,b;

 size_t x1, y1, x2 , y2;

 if (X1 < 0) x1 = 0;
 else {
       x1 = X1;
       if (x1 >= _nbRow) x1 = _nbRow - 1;
      }

 if (X2 < 0) x2 = 0;
 else {
       x2 = X2;
       if (x2 >= _nbRow) x2 = _nbRow - 1;
      }

 if (Y1 < 0) y1 = 0;
 else {
       y1 = Y1;
       if (y1 >= _nbCol) y1 = _nbCol - 1;
      }

 if (Y2 < 0) y2 = 0;
 else {
       y2 = Y2;
       if (y2 >= _nbCol) y2 = _nbCol - 1;
      }

 dx=x2-x1; dy=y2-y1;
 if (dx > 0) sx = 1; else sx = -1;
 if (dy > 0) sy = 1; else sy = -1;

 if (dx == 0)
    if (y1 > y2) for(y=y2;y<=y1;y++) _zone[x1][y]=val;
            else for(y=y1;y<=y2;y++) _zone[x1][y]=val;
 else
 if (dy == 0)
    if (x1 > x2) for(x=x2;x<=x1;x++) _zone[x][y1]=val;
            else for(x=x1;x<=x2;x++) _zone[x][y1]=val;
 else
 if (absT(dy) < absT(dx))
     {
      a=(double)dy / (double) dx;
      b=(double)y1 - a*(double)x1;
      x=x1;
      while (x != x2)
          {
           _zone[x][(int)(a*(double)x+b)]=val;
           if (sx > 0) x++; else x--;
          }
      _zone[x2][y2]=val;
     }
     else
     {
      a=(double)dx / (double) dy;
      b=(double)x1 - a*(double)y1;
      y=y1;
      while (y != y2)
          {
           _zone[(int)(a*(double)y+b)][y]=val;
           if (sy > 0) y++; else y--;
          }
      _zone[x2][y2]=val;
     }
}

void Image::display(ostream& os) const
{
 os << "Image size : " << _nbRow << " X " << _nbCol << endl;
}

bool Image::isEqualTo(const Image& anImage) const
{
 if (_nbRow!=anImage._nbRow) return false;
 if (_nbCol!=anImage._nbCol) return false;

 // Parcours des 2 images et des que l'on trouve
 // un pixel different, on retourne false

 octet* ptrAnImage = anImage._begin;

 for(octet* ptrIm = _begin; ptrIm < _end; ptrIm++, ptrAnImage++)
 {
  if (*ptrIm != *ptrAnImage) return false;
 }

 return true;
}

void Image::_mallocImage(size_t nbRow, size_t nbCol)
{
 if (_nbRow==nbRow && _nbCol==nbCol) return;

 _freeImage();

 _begin = (octet*)malloc(sizeof(octet)*nbRow*nbCol);
 if (_begin==NULL) { fprintf(stderr,"Pb, Image::_mallocImage (1)\n");
                     exit(1);
 }

 _end = _begin + sizeof(octet)*nbRow*nbCol;

 _zone    = (octet**)malloc(sizeof(octet*)*nbRow);
 if (_zone==NULL)    { _freeImage();
                       fprintf(stderr,"Pb, Image::_mallocImage::new (2)\n");
                       exit(1);
 }

 for(size_t i=0;i<nbRow;i++)
 {
  _zone[i]=_begin+i*nbCol;
 }

 _nbRow = nbRow;
 _nbCol = nbCol;
}

void Image::_freeImage(void)
{
 if (_zone!=NULL)  free(_zone);
 if (_begin!=NULL) free(_begin);

 _nbRow = _nbCol = 0;
 _begin = _end = NULL; _zone = NULL;
}

void Image::_ImageRVB2Image(const ImageRVB& anImageRVB)
{
 setImageSize(anImageRVB.getNbRow(),anImageRVB.getNbCol()); 

 for(size_t l=0; l<_nbRow; l++)      // On doit pouvoir faire plus rapide...
  for(size_t c=0; c<_nbCol; c++)     // un jour, si j'ai le temps !!!
   {
    octet r,v,b;
    anImageRVB.readPix(l,c,r,v,b);
    int red   = r;
    int green = v;
    int blue  = b;
    float meanVal=(red+green+blue)/3.0;   // Voir la fonction _RVB_To_MONO
    _zone[l][c]=(octet)(meanVal+0.5);     // du fichier ESImages.cpp
   }
}

void Image::_copy(const Image& anImage)
{
 setImageSize(anImage._nbRow, anImage._nbCol);

 memcpy(_begin,anImage._begin,_nbRow*_nbCol);
}

void Image::_destroy(void)
{
 _freeImage();
}
