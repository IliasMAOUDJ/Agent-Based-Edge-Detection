#include "ImageRVB.h"

ostream& operator<<(ostream& os, const ImageRVB& anImageRVB)
{
 anImageRVB.display(os);
 return os;
}

ImageRVB::ImageRVB(size_t nbRow, size_t nbCol)
{
 _nbRow = _nbCol = 0;
 _allocated = false;

 setImageSize(nbRow,nbCol);
}

ImageRVB::ImageRVB(const char* fileName)
{
 _nbRow = _nbCol = 0;
 _allocated = false;

 loadImage(fileName);         // En interne un setImageSize
}

ImageRVB::ImageRVB(const ImageRVB& anImageRVB)
{
 _nbRow = _nbCol = 0;
 _allocated = false;

 _copy(anImageRVB);           // En interne un setImageSize
}

ImageRVB::ImageRVB(const Image& anImage)
{
 _nbRow = _nbCol = 0;
 _allocated = false;

 _Image2ImageRVB(anImage);    // En interne un setImageSize
}

ImageRVB& ImageRVB::operator=(const ImageRVB& anImageRVB)
{
 if (this != &anImageRVB)
 {
//_destroy();
  _copy(anImageRVB);          // En interne un setImageSize
 }

 return *this;
}

ImageRVB& ImageRVB::operator=(const Image& anImage)
{
 _Image2ImageRVB(anImage);   // En interne un setImageSize

 return *this;
}

ImageRVB::~ImageRVB(void)
{
 _destroy();
}

void ImageRVB::setImageSize(size_t nbRow, size_t nbCol)
{
 if (!_allocated)
 {
  R = new Image(nbRow,nbCol);
  V = new Image(nbRow,nbCol);
  B = new Image(nbRow,nbCol);
  _allocated = true;
 }
 else
 {
  R->setImageSize(nbRow,nbCol);
  V->setImageSize(nbRow,nbCol);
  B->setImageSize(nbRow,nbCol);
 }

 _nbRow = nbRow;
 _nbCol = nbCol;
}

void ImageRVB::loadImage(const char* fileName)
{
 loadImageFileRVB(fileName,*this);             // En interne un setImageSize
}

void ImageRVB::saveImage(const char* fileName)
{
 saveImageFileRVB(fileName,*this);
}

//-- operateur +=, +, -= et -
ImageRVB& ImageRVB::operator+=(const ImageRVB& anImageRVB)
{
 if (_nbRow!=anImageRVB._nbRow || _nbCol!=anImageRVB._nbCol)
 {
  cerr << "Warning : ImageRVB::operator+=, "
       << "size of *this != size of anImage !" << endl;
  cerr << "... do nothing ... " << endl;
 }
 else
 {
  *R += *(anImageRVB.R);
  *V += *(anImageRVB.V);
  *B += *(anImageRVB.B);
 }

 return *this;
}
//--
ImageRVB operator+(const ImageRVB& anImageRVB1,    // friend
                   const ImageRVB& anImageRVB2)
{
 ImageRVB im(anImageRVB1);
 im += anImageRVB2;
 return im;
}
//--
ImageRVB& ImageRVB::operator-=(const ImageRVB& anImageRVB)
{
 if (_nbRow!=anImageRVB._nbRow || _nbCol!=anImageRVB._nbCol)
 {
  cerr << "Warning : ImageRVB::operator-=, "
       << "size of *this != size of anImage !" << endl;
  cerr << "... do nothing ... " << endl;
 }
 else
 {
  *R -= *(anImageRVB.R);
  *V -= *(anImageRVB.V);
  *B -= *(anImageRVB.B);
 }

 return *this;
}
//--
ImageRVB operator-(const ImageRVB& anImageRVB1,    // friend
                   const ImageRVB& anImageRVB2)
{
 ImageRVB im(anImageRVB1);
 im -= anImageRVB2;
 return im;
}
//--

bool operator==(const ImageRVB& anImageRVB1, const ImageRVB& anImageRVB2)
{
 return anImageRVB1.isEqualTo(anImageRVB2);
}

bool operator!=(const ImageRVB& anImageRVB1, const ImageRVB& anImageRVB2)
{
 return !(anImageRVB1.isEqualTo(anImageRVB2));
}


size_t  ImageRVB::getNbRow(void) const
{
 return _nbRow;
}


size_t  ImageRVB::getNbCol(void) const
{
 return _nbCol;
}

const Image&  ImageRVB::getR(void) const
{
 return *R;
}

Image&  ImageRVB::getR(void)
{
 return *R;
}

const Image&  ImageRVB::getV(void) const
{
 return *V;
}

Image&  ImageRVB::getV(void)
{
 return *V;
}

const Image&  ImageRVB::getB(void) const
{
 return *B;
}

Image&  ImageRVB::getB(void)
{
 return *B;
}

void  ImageRVB::writePix(size_t row, size_t col, octet valR,
                                                 octet valV,
                                                 octet valB)
{
 R->writePix(row,col,valR);
 V->writePix(row,col,valV);
 B->writePix(row,col,valB);
}

void  ImageRVB::readPix(size_t row, size_t col, octet& valR,
                                                octet& valV,
                                                octet& valB) const
{
 R->readPix(row,col,valR);
 V->readPix(row,col,valV);
 B->readPix(row,col,valB);
}

void  ImageRVB::setImage(octet valR,
                         octet valV,
                         octet valB)
{
 R->setImage(valR);
 V->setImage(valV);
 B->setImage(valB);
}

void  ImageRVB::lineImage(int X1,int Y1,int X2,int Y2, octet valR, 
                                                       octet valV,
                                                       octet valB)
{
 R->lineImage(X1,Y1,X2,Y2,valR);
 V->lineImage(X1,Y1,X2,Y2,valV);
 B->lineImage(X1,Y1,X2,Y2,valB);
}

void ImageRVB::display(ostream& os) const
{
 os << "RVB Image size : " << _nbRow << " X " << _nbCol << endl;
}

bool ImageRVB::isEqualTo(const ImageRVB& anImageRVB) const
{
 if (*R != *anImageRVB.R) return false;
 if (*V != *anImageRVB.V) return false;
 if (*B != *anImageRVB.B) return false;

 return true;
}

void  ImageRVB::_Image2ImageRVB(const Image& anImage)
{
 setImageSize(anImage.getNbRow(),anImage.getNbCol());

 *R = anImage;
 *V = anImage;
 *B = anImage;
}

void ImageRVB::_copy(const ImageRVB& anImageRVB)
{
  setImageSize(anImageRVB._nbRow, anImageRVB._nbCol);

  *R = *(anImageRVB.R);
  *V = *(anImageRVB.V);
  *B = *(anImageRVB.B);
}

void ImageRVB::_destroy(void)
{
 _nbRow = _nbCol = 0;

 if (_allocated)
 {
  delete R;
  delete V;
  delete B;
 }
}
