  /* Version avant le probleme de lecture de fichiers ppm issus de Mathlab */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#include "ESImages.h"

void displayImage(const char *fileName, const char *visualiseur)
{
 struct stat fileStat;
 pid_t pid;

 if ((stat(fileName,&fileStat)==-1) ||
     (!S_ISREG(fileStat.st_mode))     ||
     (access(fileName,R_OK)==-1))      /* Pb pour lire le fichier */
 {
  fprintf(stderr,"displayImage: Can't read %s file\n",fileName);
  return;
 }

 do {
       pid = fork();
 } while (pid==-1 && errno==EAGAIN);

 if (pid==-1)
 {
   fprintf(stderr,"displayImage: Can't creat a new process (fork)\n");
   return;
 }

 if (pid==0)  /* Le fils seulement */
 {

  execlp(visualiseur,visualiseur,fileName,NULL);

  if (!strcmp(visualiseur,"xv"))
  {
   fprintf(stderr,"\nImpossible de trouver xv dans le PATH\n");
   fprintf(stderr,"=>Il faut executer la commande Shell suvante :\n");
   fprintf(stderr,"export PATH=...\n");
  }
  else {
   fprintf(stderr,
           "\nImpossible de trouver le visualiseur d'images %s "
           "dans le PATH\n",visualiseur);
  }

  exit(1);
 }
}

// Declaration des fonctions privees

static void _getSizeImageFile(const char* fileName,
                              size_t& nbRow, size_t& nbCol);

static void _getSizeRAS(const char *fileName, size_t& nbRow, size_t& nbCol);
static void _getSizePPM(const char *fileName, size_t& nbRow, size_t& nbCol);
static void _getSizePGM(const char *fileName, size_t& nbRow, size_t& nbCol);

static void _loadImageRAS(const char *fileName, Image& anImage);
static void _loadImagePPM(const char *fileName, Image& anImage);
static void _loadImagePGM(const char *fileName, Image& anImage);

static void _saveImageRAS(const char *fileName, const Image& anImage);
static void _saveImagePPM(const char *fileName, const Image& anImage);
static void _saveImagePGM(const char *fileName, const Image& anImage);

static void _loadImageRASRVB(const char *fileName, ImageRVB& anImageRVB);
static void _loadImagePPMRVB(const char *fileName, ImageRVB& anImageRVB);
static void _loadImagePGMRVB(const char *fileName, ImageRVB& anImageRVB);

static void _saveImageRASRVB(const char *fileName, const ImageRVB& anImageRVB);
static void _saveImagePPMRVB(const char *fileName, const ImageRVB& anImageRVB);
static void _saveImagePGMRVB(const char *fileName, const ImageRVB& anImageRVB);

// Fin declaration des fonctions privees

static octet _RVB_To_MONO(octet o1, octet o2, octet o3)
{
 float meanVal=((int)o1+(int)o2+(int)o3)/3.0;
 return ((octet)(meanVal+0.5));
}

static void _getSizeImageFile(const char* fileName,
                              size_t& nbRow, size_t& nbCol)
{
 char *extension = strrchr(fileName,'.');

 nbRow=0;     // Valeurs
 nbCol=0;     // par defaut

 if (extension==NULL) {
   fprintf(stderr,
           "Error while getSize : No extension to file %s\n",fileName);
 }
 else
 if (!strcmp(extension,".ras")) _getSizeRAS(fileName,nbRow,nbCol);
 else
 if (!strcmp(extension,".ppm")) _getSizePPM(fileName,nbRow,nbCol);
 else
 if (!strcmp(extension,".pgm")) _getSizePGM(fileName,nbRow,nbCol);
 else
 {
   fprintf(stderr,
           "Error while getSize : unknown extension %s\n",extension);
 }
}

void loadImageFile(const char* fileName, Image& anImage)
{
 size_t nbRow,nbCol;
 char *extension = strrchr(fileName,'.');

 if (extension==NULL) {
   fprintf(stderr,
           "Error while loading Image : No extension to file %s\n",fileName);
   exit(1);
 }
 else {

  _getSizeImageFile(fileName,nbRow,nbCol);
  if (nbRow==0 || nbCol==0) exit(1);

  anImage.setImageSize(nbRow,nbCol);

  if (!strcmp(extension,".ras")) _loadImageRAS(fileName,anImage);
  else
  if (!strcmp(extension,".ppm")) _loadImagePPM(fileName,anImage);
  else
  if (!strcmp(extension,".pgm")) _loadImagePGM(fileName,anImage);
  else
  {
   fprintf(stderr,
           "Error while loading Image : unknown extension %s\n",extension);
   exit(1);
  }
 }
}

void saveImageFile(const char* fileName, const Image& anImage)
{
 char *extension = strrchr(fileName,'.');

 if (extension==NULL) {
   fprintf(stderr,
           "Error while saving Image : No extension to file %s\n",fileName);
 }
 else
 if (!strcmp(extension,".ras")) _saveImageRAS(fileName,anImage);
 else
 if (!strcmp(extension,".ppm")) _saveImagePPM(fileName,anImage);
 else
 if (!strcmp(extension,".pgm")) _saveImagePGM(fileName,anImage);
 else
 {
   fprintf(stderr,
           "Error while saving Image : unknown extension %s\n",extension);
 }
}

void loadImageFileRVB(const char* fileName, ImageRVB& anImageRVB)
{
 size_t nbRow,nbCol;
 char *extension = strrchr(fileName,'.');

 if (extension==NULL) {
   fprintf(stderr,
           "Error while loading RGB Image : No extension to file %s\n",
           fileName);
   exit(1);
 }
 else {

  _getSizeImageFile(fileName,nbRow,nbCol);
  if (nbRow==0 || nbCol==0) exit(1);

  anImageRVB.setImageSize(nbRow,nbCol);

  if (!strcmp(extension,".ras")) _loadImageRASRVB(fileName,anImageRVB);
  else
  if (!strcmp(extension,".ppm")) _loadImagePPMRVB(fileName,anImageRVB);
  else
  if (!strcmp(extension,".pgm")) _loadImagePGMRVB(fileName,anImageRVB);
  else
  {
   fprintf(stderr,
           "Error while loading RGB Image : unknown extension %s\n",extension);
   exit(1);
  }
 }
}

void saveImageFileRVB(const char *fileName, const ImageRVB& anImageRVB)
{
 char *extension = strrchr(fileName,'.');

 if (extension==NULL) {
   fprintf(stderr,
           "Error while saving Image RGB : No extension to file %s\n",
           fileName);
 }
 else
 if (!strcmp(extension,".ras")) _saveImageRASRVB(fileName,anImageRVB);
 else
 if (!strcmp(extension,".ppm")) _saveImagePPMRVB(fileName,anImageRVB);
 else
 if (!strcmp(extension,".pgm")) _saveImagePGMRVB(fileName,anImageRVB);
 else
 {
   fprintf(stderr,
           "Error while saving RGB Image : unknown extension %s\n",extension);
 }
}

///////////////////////////////
// Code des fonctions privees /
///////////////////////////////

/**************************************************************************/
/**************************  RASTERFILE ***********************************/
/**************************************************************************/
/* ------- Code des fonctions "privees" rasterfile : ----------------------- */
/* _getSizeRAS,_loadImageRAS,_loadImageRASRVB,_saveImageRAS,_saveImageRASRVB */ 
/*---------------------------------------------------------------------------*/

                          /* Des define pour les fonctions ... */
#define PIC24        24
#define PIC8          8

#define F_FULLCOLOR   0
#define F_GREYSCALE   1
#define F_COLORLUT    2
#define F_GREYNOLUT   3    /* A moi... */
#define F_BWDITHER    4


/* rasterfile.h : Sun Raster FIle */

/*
 * Description of header for files containing raster images
 */
struct rasterfile {
        long    ras_magic;      /* magic number */
        long    ras_width;      /* width (pixels) of image */
        long    ras_height;     /* height (pixels) of image */
        long    ras_depth;      /* depth (1, 8, or 24 bits) of pixel */
        long    ras_length;     /* length (bytes) of image */
        long    ras_type;       /* type of file; see RT_* below */
        long    ras_maptype;    /* type of colormap; see RMT_* below */
        long    ras_maplength;  /* length (bytes) of following map */
        /* color map follows for ras_maplength bytes, followed by image */
};
#define RAS_MAGIC       0x59a66a95

        /* Sun supported ras_type's */
#define RT_OLD          0       /* Raw pixrect image in 68000 byte order */
#define RT_STANDARD     1       /* Raw pixrect image in 68000 byte order */
#define RT_BYTE_ENCODED 2       /* Run-length compression of bytes */
#define RT_FORMAT_RGB   3       /* XRGB or RGB instead of XBGR or BGR */
#define RT_EXPERIMENTAL 0xffff  /* Reserved for testing */

        /* Sun registered ras_maptype's */
#define RMT_RAW         2
        /* Sun supported ras_maptype's */
#define RMT_NONE        0       /* ras_maplength is expected to be 0 */
#define RMT_EQUAL_RGB   1       /* red[ras_maplength/3],green[],blue[] */

#define RAS_RLE         0x80

/*
 * NOTES:
 *      Each line of the image is rounded out to a multiple of 16 bits.
 *   This corresponds to the rounding convention used by the memory pixrect
 *   package (/usr/include/pixrect/memvar.h) of the SunWindows system.
 *      The ras_encoding field (always set to 0 by Sun's supported software)
 *   was renamed to ras_length in release 2.0.  As a result, rasterfiles
 *   of type 0 generated by the old software claim to have 0 length; for
 *   compatibility, code reading rasterfiles must be prepared to compute the
 *   true length from the width, height, and depth fields.
 */

/*-----------------*/
/** Routines d'XV **/
/*-----------------*/
/* 
   MONO => sunRas_MONO
*/
static octet sunRas_MONO(octet o1, octet o2, octet o3)
{
// float meanVal=((int)o1+(int)o2+(int)o3)/3.0;
// return ((octet)(meanVal+0.5));
 return _RVB_To_MONO(o1,o2,o3);
}
/* 
   write_sun_long => sunRas_write_sun_long
*/
/* write a long word in sun byte-order
   returns 0 for success, EOF for failure
 */
static int sunRas_write_sun_long(long l,FILE *fp)
{
 char c;
 
    c = ((l >> 24) & 0xff);
    if (putc (c, fp) == EOF) return (EOF);
    c = ((l >> 16) & 0xff);
    if (putc (c, fp) == EOF) return (EOF);
    c = ((l >> 8) & 0xff);
    if (putc (c, fp) == EOF) return (EOF);
    c = (l & 0xff);
    if (putc (c, fp) == EOF) return (EOF);
    return (0);
}
/* 
   read_sun_long => sunRas_read_sun_long
*/
/*
 read_sun_long => sunRas_read_sun_long
 reads a 4-byte int in Sun byteorder
 returns 0 for success, EOF for failure
*/
static int sunRas_read_sun_long (long *l, FILE *fp)
{
  int c0, c1, c2, c3;
 
  c0 = fgetc(fp);
  c1 = fgetc(fp);
  c2 = fgetc(fp);
  c3 = fgetc(fp);
 
  *l = (long)((((u_long) c0 & 0xff) << 24) |
              (((u_long) c1 & 0xff) << 16) |
              (((u_long) c2 & 0xff) <<  8) |
              (((u_long) c3 & 0xff)));
 
  if (ferror(fp) || feof(fp)) return EOF;
 
  return 0;
}
/* 
   rle_read => sunRas_readLine_ENCODED
*/
static int sunRas_readLine_ENCODED(octet *ptr, long size, long nitems,
                                   FILE* fp, int init)
{
  static int count, ch;
  int  c;
  long readbytes, read;

  if (init) { count = ch = 0; }

  readbytes = size * nitems;
  for (read = 0; read < readbytes; read++) {
    if (count) {
      *ptr++ = (octet) ch;
      count--;
    }

    else {
      c = getc(fp);
      if (c == EOF) break;

      if (c == RAS_RLE) {   /* 0x80 */
        count = getc(fp);
        if (count == EOF) break;

        if (count < 0) count &= 0xff;
        if (count == 0) *ptr++ = c;
        else {
          if ((ch = getc(fp)) == EOF) break;
          *ptr++ = ch;
        }
      }
      else *ptr++ = c;
    }
  }

  return (read/size);
}
/* 
   sunRas1to8 => sunRas_1to8
*/
static void sunRas_1to8 (octet *dest, octet *src, long len)
{
  int i, b;
  int c = 0;
 
  for (i = 0, b = -1; i < len; i++) {
    if (b < 0) {
      b = 7;
      c = ~*src++;
    }
    *dest++ = ((c >> (b--)) & 1);
  }
}
/* 
   sunRas8to1 => sunRas_8to1
*/
static void sunRas_8to1(octet *dest, octet *src, int len, int flip)
{
  int i, b;
  int c;
 
  for (c = b = i = 0; i < len; i++) {
    c <<= 1;
    c |= (*src++ ? 1 : 0);
    if (b++ == 7) {
      if (flip) c = ~c;
      *dest++ = (octet) (c & 0xff);
      b = c = 0;
    }
  }
  if (b) {
    if (flip) c = ~c;
    *dest = (octet) ((c<<(8-b)) & 0xff);
  }
}
/* 
   fixBGR => sunRas_fixBGR
*/
static void sunRas_fixBGR(octet *img, long w, long h)
{
  long  i,npixels;
  octet tmp;
 
  npixels = w*h;
  for (i=0; i<npixels; i++) {
    tmp = img[0];                   /* swap red and blue channels */
    img[0] = img[2];
    img[2] = tmp;
    img += 3;                       /* bump to next pixel */
  }
}
/* 
   xvbcopy => sunRas_xvbcopy
   static void sunRas_xvbcopy(char *src,char *dst,size_t len); 
           => memmove
   Mais attention, src <-> dst entre xvbcopy et memmove !
static void xvbcopy(char *src,char *dst,size_t len)
{ 
  //
  // Modern OS's (Solaris, etc.) frown upon the use of bcopy(),
  // and only want you to use memcpy().  However, memcpy() is broken,
  // in the sense that it doesn't properly handle overlapped regions
  // of memory.  This routine does, and also has its arguments in
  // the same order as bcopy() did, without using bcopy().
  //
 
  // determine if the regions overlap
  // 
  // 3 cases:  src=dst, src<dst, src>dst
  //
  // if src=dst, they overlap completely, but nothing needs to be moved
  // if src<dst and src+len>dst then they overlap
  // if src>dst and src<dst+len then they overlap
  //
 
  if (src==dst || len<=0) return;    // nothin' to do
 
  if (src<dst && src+len>dst) {  // do a backward copy
    src = src + len - 1;
    dst = dst + len - 1;
    for ( ; len>0; len--, src--, dst--) *dst = *src;
  }
 
  else {  // they either overlap (src>dst) or they don't overlap
    // do a forward copy
    for ( ; len>0; len--, src++, dst++) *dst = *src;
  }                                                                             
}
*/
static int sunRas_WriteSunRas(FILE *fp,octet *pic,
                              int ptype,int w,int h,
                              octet *rmap,octet *gmap,octet *bmap,
                              int numcols,int colorstyle,int userle)
{
/*
  ptype + colorstyle :

  PIC24 + F_GREYSCALE =>  8 bits (PIC24 -> PIC8 (avec LUT NOIR ET BLANC))
  PIC24 (+F_FULLCOLOR)=> 24 bits (sans LUT)
  PIC8  + F_BWDITHER  =>  1 bit  (sans LUT)
  PIC8  + F_GREYSCALE =>  8 bits (avec modif: LUT COULEUR => LUT NOIR ET BLANC)
  PIC8  + F_GREYNOLUT =>  8 bits (sans LUT)
  PIC8  (+F_COLORLUT) =>  8 bits (avec LUT d'origine)
*/
  /* writes a sun rasterfile to the already open stream
     writes either 24-bit, 8-bit or 1-bit
     currently will not write rle files

     if PIC24 and F_GREYSCALE, writes an 8-bit grayscale image

     biggest problem w/ rle file: should we compute
     image size first (nicer) or go back and write it
     in when we are done (kludgy)?
   */

  struct rasterfile sunheader;
  int   linesize, i, color, d, y, flipbw;
  octet *line, *graypic, graymap[256], *sp, *dp;

  (void)userle;
  graypic = NULL;
  flipbw  = 0;

  /* special case: if PIC24 and writing GREYSCALE, write 8-bit file */
  if (ptype == PIC24  && colorstyle == F_GREYSCALE) {
    graypic = (octet*) malloc((size_t) w*h);
    if (!graypic) {
                   fprintf(stderr,"unable to malloc in sunRas_WriteSunRas\n");
                   return (1);
    }
    
    for (i=0,sp=pic,dp=graypic; i<w*h; i++,sp+=3,dp++) {
      *dp = sunRas_MONO(sp[0],sp[1],sp[2]);
    }

    for (i=0; i<256; i++) graymap[i] = i;
    rmap = gmap = bmap = graymap;
    numcols = 256;
    ptype = PIC8;
    pic = graypic;
  }


  if      (ptype==PIC24)    { d = 24;  linesize = w * 3; }
  else if (colorstyle != F_BWDITHER) { d = 8;   linesize = w;     }
  else { 
    d = 1;
    linesize = w;
    if (linesize % 8) linesize += (8 - linesize % 8);
    linesize /= 8;
  }

  if (linesize % 2) linesize++;
  line = (octet *) malloc((size_t) linesize);
  if (!line) {
    fprintf(stderr,"unable to malloc in sunRas_WriteSunRas\n");
    if (graypic) free(graypic);
    return (1);
  }

  if (d==1) {
    /* set flipbw if color#0 is black */
    flipbw = (
              sunRas_MONO(rmap[0],gmap[0],bmap[0])
                <
              sunRas_MONO(rmap[1],gmap[1],bmap[1])
             );
  }

  /* set up the header */
  sunheader.ras_magic	  = RAS_MAGIC;
  sunheader.ras_width	  = w;
  sunheader.ras_height	  = h;
  sunheader.ras_depth	  = d;
  sunheader.ras_length	  = linesize * h;
  sunheader.ras_type	  = RT_STANDARD;
  sunheader.ras_maptype   = (d==1 || d==24 || colorstyle==F_GREYNOLUT) ?
                                RMT_NONE : RMT_EQUAL_RGB;
  sunheader.ras_maplength = (d==1 || d==24 || colorstyle==F_GREYNOLUT) ?
                                0 : 3 * numcols;

  sunRas_write_sun_long (sunheader.ras_magic , fp);
  sunRas_write_sun_long (sunheader.ras_width , fp);
  sunRas_write_sun_long (sunheader.ras_height, fp);
  sunRas_write_sun_long (sunheader.ras_depth , fp);
  sunRas_write_sun_long (sunheader.ras_length   , fp);
  sunRas_write_sun_long (sunheader.ras_type     , fp);
  sunRas_write_sun_long (sunheader.ras_maptype  , fp);
  sunRas_write_sun_long (sunheader.ras_maplength, fp);

  /* write the colormap */
  if (d == 8 && colorstyle!= F_GREYNOLUT)
  {
    if (colorstyle == F_GREYSCALE)  /* grayscale */
    {
      for (color=0; color<3; color++)
      {
	for (i=0; i<numcols; i++)
        {
	  putc (sunRas_MONO(rmap[i],gmap[i],bmap[i]), fp);
        }
      }
    }
    else {
      fwrite (rmap, sizeof(octet), (size_t) numcols, fp);
      fwrite (gmap, sizeof(octet), (size_t) numcols, fp);
      fwrite (bmap, sizeof(octet), (size_t) numcols, fp);
    }
  }

  /* write the image */
  line[linesize-1] = 0;
  for (y = 0; y < h; y++) {

    if (d == 24) {
      octet *lptr, *pix;

      for (i=0,lptr=line,pix=pic+y*w*3; i<w; i++,pix+=3) {
	*lptr++ = pix[2];          /* write data out in BGR order */
	*lptr++ = pix[1];
	*lptr++ = pix[0];
      }
    }

    else if (d == 8)
      memmove(line, pic + y * w, (size_t) w);

    else /* d == 1 */
      sunRas_8to1 (line, pic + y * w, w, flipbw);

    if (fwrite (line, (size_t) 1, (size_t) linesize, fp) != (size_t)linesize) {
      fprintf(stderr,"Write failed during save!\n");
      if (graypic) free(graypic);
      free(line);
      return (2);
    }
  }

  free (line);
  if (graypic) free(graypic);
  return (0);
}

static void _getSizeRAS(const char *fileName, size_t& nbRow, size_t& nbCol)
{
  FILE  *fp = NULL;

  struct rasterfile sunheader;                 /* Comme XV */

  if ((fp=fopen(fileName,"rb"))==NULL) {
    fprintf(stderr,"Can't Read Rasterfile while getSizeImage: %s\n",
                   fileName);
    return;
  }

 /* Lecture de l'en-tete */

  sunRas_read_sun_long (&sunheader.ras_magic    , fp);
  sunRas_read_sun_long (&sunheader.ras_width    , fp);
  sunRas_read_sun_long (&sunheader.ras_height   , fp);
  sunRas_read_sun_long (&sunheader.ras_depth    , fp);
  sunRas_read_sun_long (&sunheader.ras_length   , fp);
  sunRas_read_sun_long (&sunheader.ras_type     , fp);
  sunRas_read_sun_long (&sunheader.ras_maptype  , fp);
  sunRas_read_sun_long (&sunheader.ras_maplength, fp);

 fclose(fp);

 if (sunheader.ras_magic!=RAS_MAGIC) {
    fprintf(stderr,"Rasterfile Error: %s, Bad Magic Number!\n",fileName);
    return;
 }

 nbRow = sunheader.ras_height;
 nbCol = sunheader.ras_width;
}

/*---*/

static void _loadImageRAS(const char *fileName, Image& anImage)
{
  FILE  *fp = NULL;

  struct rasterfile sunheader;                 /* Comme XV */

  size_t nbRow = 0, nbCol = 0;
  octet *LUT = NULL , *Red = NULL, *Green = NULL, *Blue = NULL;

  long linesize,lsize,csize,isize,i,w,h,d;     /* Comme XV */

  octet *line = NULL, *image = NULL;           /* Comme XV */

  if ((fp=fopen(fileName,"rb"))==NULL) {
      fprintf(stderr,"Can't Read Rasterfile: %s\n",fileName);
      exit(1);
  }

 fprintf(stderr,"Loading Rasterfile: %s...\n",fileName);

 /* Lecture de l'en-tete */

  sunRas_read_sun_long (&sunheader.ras_magic    , fp);
  sunRas_read_sun_long (&sunheader.ras_width    , fp);
  sunRas_read_sun_long (&sunheader.ras_height   , fp);
  sunRas_read_sun_long (&sunheader.ras_depth    , fp);
  sunRas_read_sun_long (&sunheader.ras_length   , fp);
  sunRas_read_sun_long (&sunheader.ras_type     , fp);
  sunRas_read_sun_long (&sunheader.ras_maptype  , fp);
  sunRas_read_sun_long (&sunheader.ras_maplength, fp);

 if (sunheader.ras_magic!=RAS_MAGIC) {
      fprintf(stderr,"Rasterfile Error: %s, Bad Magic Number!\n",fileName);
      fclose(fp);
      exit(1);
 }

 nbRow = sunheader.ras_height;
 nbCol = sunheader.ras_width;
 /*#####################################################################*/
 if (nbRow!=anImage.getNbRow() || nbCol!=anImage.getNbCol())
 {
  fprintf(stderr,"Rasterfile Error : Bad Image Size!\n");
  fprintf(stderr,"Must Be : %dX%d\n",anImage.getNbRow(),anImage.getNbCol());
  fprintf(stderr,"===> %s is not %dX%d\n",fileName,anImage.getNbRow(),
                                                   anImage.getNbCol());
  fclose(fp);
  exit(1);
 }
 /*#####################################################################*/

 /* make sure that the input picture can be dealt with */
  if (sunheader.ras_depth != 1 &&
      sunheader.ras_depth != 8 &&
      sunheader.ras_depth != 24 &&
      sunheader.ras_depth != 32)
  {
    fprintf (stderr, "Rasterfile Error: %s image has depth %ld\n",
             fileName,sunheader.ras_depth);
    fprintf (stderr, "Depths supported are 1, 8, 24, and 32\n");
    fclose(fp);
    exit(1);
  }

  if (sunheader.ras_type != RT_OLD &&
      sunheader.ras_type != RT_STANDARD &&
      sunheader.ras_type != RT_BYTE_ENCODED &&
      sunheader.ras_type != RT_FORMAT_RGB) {
    fprintf (stderr, "Rasterfile Error: %s of unsupported type %ld\n",
             fileName,sunheader.ras_type);
    fclose(fp);
    exit(1);
  }

  w = sunheader.ras_width;        /* w <=> nbCol */
  h = sunheader.ras_height;       /* h <=> nbRow */
  d = sunheader.ras_depth;
  isize = sunheader.ras_length ?
          sunheader.ras_length :
          (w * h * d) / 8;
  csize = (sunheader.ras_maptype == RMT_NONE) ? 0 : sunheader.ras_maplength;
 
 
  /* compute length of the output (xv-format) image */
  lsize = w * h;
  if (d == 24 || d == 32) lsize = lsize * 3;
 
  linesize = w * d;
  if (linesize % 16) linesize += (16 - (linesize % 16));
  linesize /= 8;

  /* read in the colormap, if any */
  if (sunheader.ras_maptype == RMT_EQUAL_RGB && csize) {
     int lutSize=sunheader.ras_maplength;
     LUT=(octet*)malloc(lutSize);
     fread(LUT,lutSize,1,fp);
     Red=LUT;
     Green=Red+lutSize/3;
     Blue=Green+lutSize/3;
  }
  else if (sunheader.ras_maptype == RMT_RAW && csize) {
    /* we don't know how to handle raw colormap, ignore */
    fseek (fp, (long) csize, 1);
  }
  else {  /* no colormap, make one up */
    if (d == 1) {
      int lutSize=2*3;
      LUT=(octet*)malloc(lutSize);
      Red=LUT;
      Green=Red+lutSize/3;
      Blue=Green+lutSize/3;
      Red[0] = Green[0] = Blue[0] = 0;
      Red[1] = Green[1] = Blue[1] = 255;
    }
    else if (d == 8) {
      int lutSize=256*3;
      LUT=(octet*)malloc(lutSize);
      Red=LUT;
      Green=Red+lutSize/3;
      Blue=Green+lutSize/3;
      for (i = 0; i < lutSize/3; i++)
        Red[i] = Green[i] = Blue[i] = i;
    }
  }


  /* allocate memory for picture and read it in */
  /* note we may slightly overallocate here (if image is padded) */
  image = (octet*)malloc ((size_t) lsize);
  line  = (octet*)malloc ((size_t) linesize);

  if (image == NULL || line == NULL) {
    fprintf(stderr,
            "Rasterfile Error: %s, can't allocate memory\n",fileName);
    fclose(fp);
    free(LUT);
    free(image);
    free(line);
    exit(1);
  }
  
  for (i = 0; i < h ; i++)        /* h <=> nbRow */
  {
    if (sunheader.ras_type == RT_BYTE_ENCODED) {
      if (sunRas_readLine_ENCODED(line, 1, linesize, fp, (i==0)) != linesize)
      {
        break;
      }
    }
    else {
      if (fread (line, (size_t) 1, (size_t) linesize, fp) != (size_t)linesize)
      {
        fclose(fp);
        free(LUT);
        free(image);
        free(line);
        exit(1);
      }
    }

    switch (d) {

    case 1:  sunRas_1to8 (image + w * i, line, w);
             break;
    case 8:  memmove(image + w * i, line, (size_t) w);
             break;
    case 24: memmove(image + w * i * 3, line, (size_t) w*3);
             break;
    case 32:
      {
        int k;
        octet *ip, *op;
        ip = line;
        op = (octet *) (image + w * i * 3);
        for (k = 0; k<w; k++) {
           ip++;           /* skip 'alpha' */
          *op++ = *ip++;   /* red   */
          *op++ = *ip++;   /* green */
          *op++ = *ip++;   /* blue  */
        }
      }
    }
  }
                    /* Fix up BGR order SUN 24-bit rasters to be RGB order */
  if (d == 24 || d == 32) {  /* Il faut inverser le Bleu et le Rouge ! */
    if (sunheader.ras_type != RT_FORMAT_RGB) sunRas_fixBGR(image,w,h);
  }

  /* Mise a jour de la structure "Image" de XV :

    if (d == 24 || d == 32) pinfo->type = PIC24;
    if (d ==  1 || d ==  8) pinfo->type = PIC8;
    else                    pinfo->type = PIC8
 
    pinfo->pic = image;  // zone image
    pinfo->w = w;        // nbCol (largeur)
    pinfo->h = h;        // nbRow (hauteur)
    pinfo->normw = pinfo->w; pinfo->normh = pinfo->h;
    pinfo->frmType = F_SUNRAS;
    pinfo->colType = (d==1) ? F_BWDITHER : F_FULLCOLOR;
    sprintf(pinfo->fullInfo, "Sun %s rasterfile.  (%d plane%s)  (%ld bytes)",
          sunheader.ras_type == RT_BYTE_ENCODED ? "rle" : "standard",
          d, d == 1 ? "" : "s",
          (long) (sizeof(struct rasterfile) + csize + isize));
 
    sprintf(pinfo->shrtInfo, "%dx%d Sun Rasterfile.",w,h);
    pinfo->comment = (char *) NULL;

  */

 /* Il faut transferer le contenu de l'image dans notre image anImage */

 if (d==1 || d==8) {

#if 0

   for (i = 0 ; i < anImage.getNbRow() ; i++)
   {
    int j;
    for (j = 0 ; j < anImage.getNbCol() ; j++)
    {
     octet val   = *(image + w*i+j);
     octet red   = Red[val];
     octet green = Green[val];
     octet blue  = Blue[val];
     anImage[i][j] = sunRas_MONO(red,green,blue);
    }
   }

#else

  /* Et pour aller plus vite ... */

  octet *ptrZone    = anImage.begin();
  octet *ptrEndZone = ptrZone+anImage.getNbRow()*anImage.getNbCol();
  octet *ptrImage   = image;

  octet *ptrZ = NULL, *ptrI = NULL;

  for(ptrZ=ptrZone,ptrI=ptrImage; ptrZ<ptrEndZone ; )
  {
   octet val   = *ptrI++;
   octet red   = Red[val];
   octet green = Green[val];
   octet blue  = Blue[val];
   *ptrZ++ = sunRas_MONO(red,green,blue);
  }

#endif

 }
 else { /* d==24 || d==32 */

#if 0

   for (i = 0 ; i < anImage.getNbRow() ; i++)
   {
    int j;
    for (j = 0 ; j < anImage.getNbCol() ; j++)
    {
     octet red   = *(image + (w*i+j)*3);
     octet green = *(image + (w*i+j)*3 + 1);
     octet blue  = *(image + (w*i+j)*3 + 2);
     anImage[i][j] = sunRas_MONO(red,green,blue);
    }
   }

#else

  /* Et pour aller plus vite ... */
  octet *ptrZone    = anImage.begin();
  octet *ptrEndZone = ptrZone+anImage.getNbRow()*anImage.getNbCol();
  octet *ptrImage   = image;

  octet *ptrZ = NULL, *ptrI = NULL;

  for(ptrZ=ptrZone,ptrI=ptrImage; ptrZ<ptrEndZone ; )
  {
   octet red   = *ptrI++;
   octet green = *ptrI++;
   octet blue  = *ptrI++;
   *ptrZ++ = sunRas_MONO(red,green,blue);
  }

#endif

 }

 fclose(fp);
 free(LUT);
 free(image);
 free(line);
}



static void _saveImageRAS(const char *fileName, const Image& anImage)
{
  int    nbRow = anImage.getNbRow(), nbCol = anImage.getNbCol();

  const octet *ptrZone = anImage.begin();
  const octet *ptrEndZone = ptrZone+nbRow*nbCol;

  FILE  *fp = NULL;

  int   ok;

  int   i = 0, numcols = 0;
  octet graymap[256], *rmap = NULL, *gmap = NULL, *bmap = NULL;

  const octet *pic = ptrZone;
  octet *allocatedPic = NULL;

/* 
  PIC24 + F_GREYSCALE =>  8 bits (PIC24 -> PIC8 (avec LUT NOIR ET BLANC))
  PIC24 (+F_FULLCOLOR)=> 24 bits (sans LUT)
  PIC8  + F_BWDITHER  =>  1 bit  (sans LUT)
  PIC8  + F_GREYSCALE =>  8 bits (avec modif: LUT COULEUR => LUT NOIR ET BLANC)
  PIC8  + F_GREYNOLUT =>  8 bits (sans LUT)
  PIC8  (+F_COLORLUT) =>  8 bits (avec LUT d'origine)
*/

  int ptype      = PIC8;              /* PIC24 ou PIC8 */
  int colorstyle = F_GREYSCALE;

  /* Preparation LUT pour XV */

  numcols = 256;
  for (i=0; i<numcols; i++) graymap[i] = i;
  rmap = gmap = bmap = graymap;

  /* Preparation image (pic) pour XV */

  if (ptype == PIC24) {

     const octet *ptrZ = NULL; octet *ptrAP = NULL;

     allocatedPic = (octet*)malloc((size_t) nbRow*nbCol*3);
     if (allocatedPic==NULL) {
       fprintf(stderr,"Can't allocate a 24 bits image in saveImageRAS\n");
       exit(1);
     }

     pic = allocatedPic;
 
     for(ptrZ=ptrZone,ptrAP=allocatedPic; ptrZ<ptrEndZone ; )
     {
      *ptrAP++ = *ptrZ;
      *ptrAP++ = *ptrZ;
      *ptrAP++ = *ptrZ++;
     }
  }

  if (ptype == PIC8 && colorstyle == F_BWDITHER) {

     octet seuil = 100;
     const octet *ptrZ = NULL; octet *ptrAP = NULL;

     allocatedPic = (octet*)malloc((size_t) nbRow*nbCol);
     if (allocatedPic==NULL) {
       fprintf(stderr,"Can't allocate a 8 bits image in saveImageRAS\n");
       exit(1);
     }

     pic = allocatedPic;

     /* En fait, il y a plus "réaliste" qu'un seuillage .... */
     /* Voir dans le fichier xvimage.c la fonction FSDither  */

     for(ptrZ=ptrZone,ptrAP=allocatedPic; ptrZ<ptrEndZone;)
     {
      *ptrAP++ = (*ptrZ++ <= seuil) ? 0 : 1;
     }

    /* Modif LUT pour XV dans le cas F_BWDITHER */
     numcols = 2;
     graymap[0]=0; graymap[1]=255;
     rmap = gmap = bmap = graymap;
  }

  /* Ouverture du fichier pour XV */

  if ((fp=fopen(fileName,"wb"))==NULL) {
      fprintf(stderr,"Can't Save Rasterfile: %s\n",fileName);
      exit(1);
  }

  fprintf(stderr,"Saving image Rasterfile: %s\n",fileName);

  /* Sauvegarde avec la routine d'XV */

  ok = sunRas_WriteSunRas(fp,(octet*)pic,ptype,nbCol,nbRow,
                          rmap,gmap,bmap,numcols,colorstyle,0);
 
  if (ok!=0) { fprintf(stderr,
                       "Error when saving image Rasterfile: %s\n", fileName);
               if (allocatedPic!=NULL) free(allocatedPic);
               fclose(fp);
               exit(1);
  }

  if (allocatedPic!=NULL) free(allocatedPic);

  fclose(fp);
}

static void _loadImageRASRVB(const char *fileName, ImageRVB& anImageRVB)
{
  FILE  *fp = NULL;

  struct rasterfile sunheader;                 /* Comme XV */

  size_t  nbRow = 0, nbCol = 0;
  octet *LUT = NULL , *Red = NULL, *Green = NULL, *Blue = NULL;

  long linesize,lsize,csize,isize,i,w,h,d;     /* Comme XV */

  octet *line = NULL, *image = NULL;           /* Comme XV */

  if ((fp=fopen(fileName,"rb"))==NULL) {
      fprintf(stderr,"Can't Read Rasterfile: %s\n",fileName);
      exit(1);
  }

 fprintf(stderr,"Loading Rasterfile: %s...\n",fileName);

 /* Lecture de l'en-tete */

  sunRas_read_sun_long (&sunheader.ras_magic    , fp);
  sunRas_read_sun_long (&sunheader.ras_width    , fp);
  sunRas_read_sun_long (&sunheader.ras_height   , fp);
  sunRas_read_sun_long (&sunheader.ras_depth    , fp);
  sunRas_read_sun_long (&sunheader.ras_length   , fp);
  sunRas_read_sun_long (&sunheader.ras_type     , fp);
  sunRas_read_sun_long (&sunheader.ras_maptype  , fp);
  sunRas_read_sun_long (&sunheader.ras_maplength, fp);

 if (sunheader.ras_magic!=RAS_MAGIC) {
      fprintf(stderr,"Rasterfile Error: %s, Bad Magic Number!\n",fileName);
      fclose(fp);
      exit(1);
 }

 nbRow = sunheader.ras_height;
 nbCol = sunheader.ras_width;
 /*#####################################################################*/
 if (nbRow!=anImageRVB.getNbRow() || nbCol!=anImageRVB.getNbCol())
 {
  fprintf(stderr,"Rasterfile Error : Bad Image Size!\n");
  fprintf(stderr,"Must Be : %dX%d\n",anImageRVB.getNbRow(),
                                     anImageRVB.getNbCol());
  fprintf(stderr,"===> %s is not %dX%d\n",fileName,anImageRVB.getNbRow(),
                                                   anImageRVB.getNbCol());
  fclose(fp);
  exit(1);
 }
 /*#####################################################################*/

 /* make sure that the input picture can be dealt with */
  if (sunheader.ras_depth != 1 &&
      sunheader.ras_depth != 8 &&
      sunheader.ras_depth != 24 &&
      sunheader.ras_depth != 32)
  {
    fprintf (stderr, "Rasterfile Error: %s image has depth %ld\n",
             fileName,sunheader.ras_depth);
    fprintf (stderr, "Depths supported are 1, 8, 24, and 32\n");
    fclose(fp);
    exit(1);
  }

  if (sunheader.ras_type != RT_OLD &&
      sunheader.ras_type != RT_STANDARD &&
      sunheader.ras_type != RT_BYTE_ENCODED &&
      sunheader.ras_type != RT_FORMAT_RGB) {
    fprintf (stderr, "Rasterfile Error: %s of unsupported type %ld\n",
             fileName,sunheader.ras_type);
    fclose(fp);
    exit(1);
  }

  w = sunheader.ras_width;        /* w <=> nbCol */
  h = sunheader.ras_height;       /* h <=> nbRow */
  d = sunheader.ras_depth;
  isize = sunheader.ras_length ?
          sunheader.ras_length :
          (w * h * d) / 8;
  csize = (sunheader.ras_maptype == RMT_NONE) ? 0 : sunheader.ras_maplength;
 
 
  /* compute length of the output (xv-format) image */
  lsize = w * h;
  if (d == 24 || d == 32) lsize = lsize * 3;
 
  linesize = w * d;
  if (linesize % 16) linesize += (16 - (linesize % 16));
  linesize /= 8;

  /* read in the colormap, if any */
  if (sunheader.ras_maptype == RMT_EQUAL_RGB && csize) {
     int lutSize=sunheader.ras_maplength;
     LUT=(octet*)malloc(lutSize);
     fread(LUT,lutSize,1,fp);
     Red=LUT;
     Green=Red+lutSize/3;
     Blue=Green+lutSize/3;
  }
  else if (sunheader.ras_maptype == RMT_RAW && csize) {
    /* we don't know how to handle raw colormap, ignore */
    fseek (fp, (long) csize, 1);
  }
  else {  /* no colormap, make one up */
    if (d == 1) {
      int lutSize=2*3;
      LUT=(octet*)malloc(lutSize);
      Red=LUT;
      Green=Red+lutSize/3;
      Blue=Green+lutSize/3;
      Red[0] = Green[0] = Blue[0] = 0;
      Red[1] = Green[1] = Blue[1] = 255;
    }
    else if (d == 8) {
      int lutSize=256*3;
      LUT=(octet*)malloc(lutSize);
      Red=LUT;
      Green=Red+lutSize/3;
      Blue=Green+lutSize/3;
      for (i = 0; i < lutSize/3; i++)
        Red[i] = Green[i] = Blue[i] = i;
    }
  }


  /* allocate memory for picture and read it in */
  /* note we may slightly overallocate here (if image is padded) */
  image = (octet*)malloc ((size_t) lsize);
  line  = (octet*)malloc ((size_t) linesize);

  if (image == NULL || line == NULL) {
    fprintf(stderr,
            "Rasterfile Error: %s, can't allocate memory\n",fileName);
    fclose(fp);
    free(LUT);
    free(image);
    free(line);
    exit(1);
  }
  
  for (i = 0; i < h ; i++)        /* h <=> nbRow */
  {
    if (sunheader.ras_type == RT_BYTE_ENCODED) {
      if (sunRas_readLine_ENCODED(line, 1, linesize, fp, (i==0)) != linesize)
      {
        break;
      }
    }
    else {
      if (fread (line, (size_t) 1, (size_t) linesize, fp) != (size_t)linesize)
      {
        fclose(fp);
        free(LUT);
        free(image);
        free(line);
        exit(1);
      }
    }

    switch (d) {

    case 1:  sunRas_1to8 (image + w * i, line, w);
             break;
    case 8:  memmove(image + w * i, line, (size_t) w);
             break;
    case 24: memmove(image + w * i * 3, line, (size_t) w*3);
             break;
    case 32:
      {
        int k;
        octet *ip, *op;
        ip = line;
        op = (octet *) (image + w * i * 3);
        for (k = 0; k<w; k++) {
           ip++;           /* skip 'alpha' */
          *op++ = *ip++;   /* red   */
          *op++ = *ip++;   /* green */
          *op++ = *ip++;   /* blue  */
        }
      }
    }
  }
                    /* Fix up BGR order SUN 24-bit rasters to be RGB order */
  if (d == 24 || d == 32) {  /* Il faut inverser le Bleu et le Rouge ! */
    if (sunheader.ras_type != RT_FORMAT_RGB) sunRas_fixBGR(image,w,h);
  }

  /* Mise a jour de la structure "Image" de XV :

    if (d == 24 || d == 32) pinfo->type = PIC24;
    if (d ==  1 || d ==  8) pinfo->type = PIC8;
    else                    pinfo->type = PIC8
 
    pinfo->pic = image;  // zone image
    pinfo->w = w;        // nbCol (largeur)
    pinfo->h = h;        // nbRow (hauteur)
    pinfo->normw = pinfo->w; pinfo->normh = pinfo->h;
    pinfo->frmType = F_SUNRAS;
    pinfo->colType = (d==1) ? F_BWDITHER : F_FULLCOLOR;
    sprintf(pinfo->fullInfo, "Sun %s rasterfile.  (%d plane%s)  (%ld bytes)",
          sunheader.ras_type == RT_BYTE_ENCODED ? "rle" : "standard",
          d, d == 1 ? "" : "s",
          (long) (sizeof(struct rasterfile) + csize + isize));
 
    sprintf(pinfo->shrtInfo, "%dx%d Sun Rasterfile.",w,h);
    pinfo->comment = (char *) NULL;

  */

 /* Il faut transferer le contenu de l'image dans notre image out */

 if (d==1 || d==8) {

#if 0

   for (i = 0 ; i < anImageRVB.getNbRow() ; i++)
   {
    int j;
    for (j = 0 ; j < anImageRVB.getNbCol() ; j++)
    {
     octet val   = *(image + w*i+j);
     anImageRVB.writePix(i,j,Red[val],Green[val],Blue[val]);
    }
   }

#else

  /* Et pour aller plus vite ... */

  Image& R = anImageRVB.getR();
  Image& V = anImageRVB.getV();
  Image& B = anImageRVB.getB();

  octet *ptrZoneR    = R.begin();
  octet *ptrZoneV    = V.begin();
  octet *ptrZoneB    = B.begin();

  octet *ptrEndZoneR = ptrZoneR+anImageRVB.getNbRow()*anImageRVB.getNbCol();
  octet *ptrImage    = image;

  octet *ptrZ_R = NULL, *ptrZ_V = NULL, *ptrZ_B = NULL, *ptrI = NULL;

  for(ptrZ_R=ptrZoneR,ptrZ_V=ptrZoneV,ptrZ_B=ptrZoneB,ptrI=ptrImage;
      ptrZ_R<ptrEndZoneR ; )
  {
   octet val   = *ptrI++;
  
   *ptrZ_R++ = Red[val];
   *ptrZ_V++ = Green[val];
   *ptrZ_B++ = Blue[val];
  }

#endif

 }
 else { /* d==24 || d==32 */

#if 0

  Image& R = anImageRVB.getR();
  Image& V = anImageRVB.getV();
  Image& B = anImageRVB.getB();

   for (i = 0 ; i < anImageRVB.getNbRow() ; i++)
   {
    int j;
    for (j = 0 ; j < anImageRVB.getNbCol() ; j++)
    {
     octet r,v,b;
     r = *(image + (w*i+j)*3);
     v = *(image + (w*i+j)*3 + 1);
     b = *(image + (w*i+j)*3 + 2);
     anImageRVB.writePix(i,j, r,v,b);
    }
   }

#else

  /* Et pour aller plus vite ... */

  Image& R = anImageRVB.getR();
  Image& V = anImageRVB.getV();
  Image& B = anImageRVB.getB();

  octet *ptrZoneR    = R.begin();
  octet *ptrZoneV    = V.begin();
  octet *ptrZoneB    = B.begin();

  octet *ptrEndZoneR = ptrZoneR+anImageRVB.getNbRow()*anImageRVB.getNbCol();
  octet *ptrImage    = image;

  octet *ptrZ_R = NULL, *ptrZ_V = NULL, *ptrZ_B = NULL, *ptrI = NULL;

  for(ptrZ_R=ptrZoneR,ptrZ_V=ptrZoneV,ptrZ_B=ptrZoneB,ptrI=ptrImage;
      ptrZ_R<ptrEndZoneR ; )
  {
   *ptrZ_R++ = *ptrI++;
   *ptrZ_V++ = *ptrI++;
   *ptrZ_B++ = *ptrI++;
  }

#endif

 }

 fclose(fp);
 free(LUT);
 free(image);
 free(line);
}

/*---*/

static void _saveImageRASRVB(const char *fileName, const ImageRVB& anImageRVB)
{
  int    nbRow = anImageRVB.getNbRow(), nbCol = anImageRVB.getNbCol();

  const octet *ptrZoneR = anImageRVB.getR().begin();
  const octet *ptrZoneV = anImageRVB.getV().begin();
  const octet *ptrZoneB = anImageRVB.getB().begin();

  const octet *ptrEndZoneR = ptrZoneR+nbRow*nbCol;

  FILE  *fp = NULL;

  int   ok;

  int   i = 0, numcols = 0;
  octet graymap[256], *rmap = NULL, *gmap = NULL, *bmap = NULL;

  octet *pic = NULL, *allocatedPic = NULL;

/* 
  PIC24 + F_GREYSCALE =>  8 bits (PIC24 -> PIC8 (avec LUT NOIR ET BLANC))
  PIC24 (+F_FULLCOLOR)=> 24 bits (sans LUT)
  PIC8  + F_BWDITHER  =>  1 bit  (sans LUT)
  PIC8  + F_GREYSCALE =>  8 bits (avec modif: LUT COULEUR => LUT NOIR ET BLANC)
  PIC8  + F_GREYNOLUT =>  8 bits (sans LUT)
  PIC8  (+F_COLORLUT) =>  8 bits (avec LUT d'origine)
*/

  int ptype      = PIC24;              /* PIC24 ou PIC8 */
  int colorstyle = F_FULLCOLOR;

  /* Preparation LUT pour XV */

  numcols = 256;
  for (i=0; i<numcols; i++) graymap[i] = i;
  rmap = gmap = bmap = graymap;

  /* Preparation image (pic) pour XV */

  if (ptype == PIC24) {

     const octet *ptrZ_R = NULL;
     const octet *ptrZ_V = NULL;
     const octet *ptrZ_B = NULL;
     octet *ptrAP = NULL;

     allocatedPic = (octet*)malloc((size_t) nbRow*nbCol*3);
     if (allocatedPic==NULL) {
       fprintf(stderr,"Can't allocate a 24 bits image in saveImageRAS\n");
       exit(1);
     }

     pic = allocatedPic;
 
     for(ptrZ_R=ptrZoneR,ptrZ_V=ptrZoneV,ptrZ_B=ptrZoneB,ptrAP=allocatedPic;
         ptrZ_R<ptrEndZoneR ; )
     {
      *ptrAP++ = *ptrZ_R++;
      *ptrAP++ = *ptrZ_V++;
      *ptrAP++ = *ptrZ_B++;
     }
  }

  if (ptype == PIC8) {

     const octet *ptrZ_R = NULL;
     const octet *ptrZ_V = NULL;
     const octet *ptrZ_B = NULL;
     octet *ptrAP = NULL;

     allocatedPic = (octet*)malloc((size_t) nbRow*nbCol);
     if (allocatedPic==NULL) {
       fprintf(stderr,"Can't allocate a 8 bits image in saveImageRAS\n");
       exit(1);
     }

     pic = allocatedPic;

     /* on va passer de R,V,B a NB (R+V+B)/3 */

     for(ptrZ_R=ptrZoneR,ptrZ_V=ptrZoneV,ptrZ_B=ptrZoneB,ptrAP=allocatedPic;
         ptrZ_R<ptrEndZoneR ; )
     {
       octet red   = *ptrZ_R++;
       octet green = *ptrZ_V++;
       octet blue  = *ptrZ_B++;
       *ptrAP++    = sunRas_MONO(red,green,blue);
     }
  }

  if (ptype == PIC8 && colorstyle == F_BWDITHER) {

    octet seuil = 100;
    octet *ptrAP = NULL, *ptrEndAP = NULL;

    /* En fait, il y a plus "réaliste" qu'un seuillage .... */
    /* Voir dans le fichier xvimage.c la fonction FSDither  */

    for(ptrAP=allocatedPic,ptrEndAP=ptrAP+nbRow*nbCol; ptrAP<ptrEndAP;ptrAP++)
    {
     octet val = (*ptrAP <= seuil) ? 0 : 1;
     *ptrAP = val;
    }

    /* Modif LUT pour XV dans le cas F_BWDITHER */
    numcols = 2;
    graymap[0]=0; graymap[1]=255;
    rmap = gmap = bmap = graymap;
  }

  /* Ouverture du fichier pour XV */

  if ((fp=fopen(fileName,"wb"))==NULL) {
      fprintf(stderr,"Can't Save Rasterfile: %s\n",fileName);
      exit(1);
  }

  fprintf(stderr,"Saving image Rasterfile: %s\n",fileName);

  /* Sauvegarde avec la routine d'XV */

  ok = sunRas_WriteSunRas(fp,pic,ptype,nbCol,nbRow,
                          rmap,gmap,bmap,numcols,colorstyle,0);
 
  if (ok!=0) { fprintf(stderr,
                       "Error when saving image Rasterfile: %s\n", fileName);
               if (allocatedPic!=NULL) free(allocatedPic);
               fclose(fp);
               exit(1);
  }

  if (allocatedPic!=NULL) free(allocatedPic);

  fclose(fp);
}


/**************************************************************************/
/**************************  Fin RASTERFILE *******************************/
/**************************************************************************/

/**************************************************************************/
/***********************  Et sur des fichiers PPM *************************/
/**************************************************************************/
/* ------- Code des fonctions "privees" ppm : --------------------------- */
/*              _getSizePPM, _loadImagePPM, _saveImagePPM                 */
/*------------------------------------------------------------------------*/

static void _getSizePPM(const char *fileName, size_t& nbRow, size_t& nbCol)
{
 FILE *fd;
 char buff[1024];
 char Type[1024];
 int  _nbRow,_nbCol;

 if ((fd=fopen(fileName,"rb"))==NULL) {
   fprintf(stderr,"Can't Read PPM while getSize : %s\n",fileName);
   return;
 }

  /* Lecture en-tete */
 fgets(buff,255,fd);
 sscanf(buff,"%s",Type);

  /* Lecture taille */
 fgets(buff,255,fd);
 while (buff[0]=='#') fgets(buff,255,fd);

 sscanf(buff,"%d %d",&_nbCol,&_nbRow);

 nbRow=_nbRow;
 nbCol=_nbCol;

 fclose(fd);
}

static
void PPM_loadAsciiP3(FILE * fd, const char *fileName, Image& anImage)
{
 int  i = 0, j = 0;
 int  nbRow = anImage.getNbRow(), nbCol = anImage.getNbCol();
 char buff [1024];

 fprintf(stderr,"Loading image from %s (PPM Ascii File (P3))\n",fileName);

 for (i = 0 ; i < nbRow ; i++)
 {
  for (j = 0 ; j < nbCol ; j++)
  {
   octet red, green, blue;
   fscanf(fd,"%s",buff);
   red = atoi (buff);
   fscanf(fd,"%s",buff);
   green = atoi (buff);
   fscanf(fd,"%s",buff);
   blue = atoi (buff);
   anImage[i][j] = _RVB_To_MONO(red,green,blue);
  }
 }
}

/*---*/

static
void PPM_saveAsciiP3(FILE * fd, const char *fileName,const Image& anImage)
{
 int  i = 0, j = 0;
 int  nbRow = anImage.getNbRow(), nbCol = anImage.getNbCol();
 char buff [255];
 char car;
 int  inc;

 fprintf(stderr,"Saving image PPM Ascii (P3) File: %s\n",fileName);

 strcpy(buff,"P3\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);
 sprintf(buff,"# saveImagePPM V0.0\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);
 sprintf(buff,"%d %d\n",nbCol,nbRow);
 fwrite(buff,sizeof(char),strlen(buff),fd);
 strcpy(buff,"255\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);

 for(i=0,inc=0;i<nbRow;i++)
  {
    for(j=0;j<nbCol;j++)
     {
       octet val = anImage[i][j];
       sprintf(buff,"%3d %3d %3d",val,val,val);
       fwrite(buff,sizeof(char),strlen(buff),fd);
       inc ++;
       if (inc==5) {
                      inc = 0;
                      car = '\n';
       }
       else {
                      car = ' ';
       }

       fwrite(&car,sizeof(car),1,fd);
     }
  }

  car = '\n';
  fwrite(&car,sizeof(car),1,fd);
}

/*---*/

static
void PPM_loadBinP6(FILE * fd, const char *fileName, Image& anImage)
{
 int i = 0, j = 0;
 int nbRow = anImage.getNbRow(), nbCol = anImage.getNbCol();

 fprintf(stderr,"Loading image from %s (PPM Bin File (P6))\n",fileName);

 fgetc(fd);  /* To skip a byte */

 for (i = 0 ; i < nbRow ; i++)
 {
  for (j = 0 ; j < nbCol ; j++)
  {
   octet red, green, blue;
   red   = fgetc(fd);
   green = fgetc(fd);
   blue  = fgetc(fd);
   anImage[i][j] = _RVB_To_MONO(red,green,blue);
  }
 }
}

/*---*/

static
void PPM_saveBinP6(FILE * fd, const char *fileName,const Image& anImage)
{
 int i = 0, j = 0;
 int nbRow = anImage.getNbRow(), nbCol = anImage.getNbCol();

 char buff [255];

 fprintf(stderr,"Saving image PPM Bin (P6) File: %s\n",fileName);

 strcpy(buff,"P6\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);
 sprintf(buff,"# saveImagePPM V0.0\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);
 sprintf(buff,"%d %d\n",nbCol,nbRow);
 fwrite(buff,sizeof(char),strlen(buff),fd);
 strcpy(buff,"255\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);

 for (i = 0 ; i < nbRow ; i++)
 {
  for (j = 0 ; j < nbCol ; j++)
  {
   char val;
   val = (char)anImage[i][j];
   fprintf(fd,"%c%c%c",val,val,val);
  }
 }
}

static void _loadImagePPM(const char *fileName, Image& anImage)
{
 FILE * fd;
 char buff [1024];
 size_t nbRow = 0, nbCol = 0, depth = 0;
 char Type[1024];

 if ((fd=fopen(fileName,"rb"))==NULL) {
     fprintf(stderr,"Can't Read PPM file: %s\n",fileName);
     exit(1);
 }

  /* Lecture en-tete */
 fgets(buff,255,fd);
 sscanf(buff,"%s",Type);

  /* Lecture taille */
 fgets(buff,255,fd);
 while (buff[0]=='#') fgets(buff,255,fd);

 sscanf(buff,"%u %u",&nbCol,&nbRow);

  /* Lecture profondeur */
 fscanf(fd,"%s",buff);
 depth = atoi(buff);

 /*#####################################################################*/
 if (nbRow!=anImage.getNbRow() || nbCol!=anImage.getNbCol())
 {
  fprintf(stderr,"Error : Bad Image Size! Must Be : %dX%d\n",
                  anImage.getNbRow(),anImage.getNbCol());
  fprintf(stderr,"===> %s is not %dX%d\n",fileName,
                  anImage.getNbRow(),anImage.getNbCol());
  fclose(fd);
  exit(1);
 }
 /*#####################################################################*/
 if (depth != 255)
 {
  fprintf(stderr,"Error PPM File: %s, Bad Depth Value!\n",fileName);
  fclose(fd);
  exit(1);
 }
 /*#####################################################################*/

 if (!strcmp(Type,"P3")) PPM_loadAsciiP3(fd,fileName,anImage);
 else
 if (!strcmp(Type,"P6")) PPM_loadBinP6(fd,fileName,anImage);
 else
 {
   fprintf(stderr,
           "Error: %s Bad PPM Image File (Must be P3 or P6)\n",fileName);
   fclose(fd);
   exit(1);
 }

 fclose(fd);
}

static void _saveImagePPM(const char *fileName, const Image& anImage)
{
 FILE * fd;
 const char *extension = strstr(fileName,".ascii.ppm");

 if ((fd=fopen(fileName,"wb"))==NULL) {
      fprintf(stderr,"Can't Save PPM File: %s\n",fileName);
      exit(1);
 }

 if (extension==NULL) extension="";

 if (!strcmp(extension,".ascii.ppm"))
 {
  PPM_saveAsciiP3(fd,fileName,anImage);
 }
 else {
  PPM_saveBinP6(fd,fileName,anImage);
 }

 fclose(fd);
}

static
void PPM_loadAsciiP3RVB(FILE * fd, const char *fileName, ImageRVB& anImageRVB)
{
 int  i = 0, j = 0;
 int  nbRow = anImageRVB.getNbRow(), nbCol = anImageRVB.getNbCol();
 char buff [1024];

 fprintf(stderr,"Loading RVB image from %s (PPM Ascii File (P3))\n",
                fileName);

 for (i = 0 ; i < nbRow ; i++)
 {
  for (j = 0 ; j < nbCol ; j++)
  {
   octet red, green, blue;
   fscanf(fd,"%s",buff);
   red = atoi (buff);
   fscanf(fd,"%s",buff);
   green = atoi (buff);
   fscanf(fd,"%s",buff);
   blue = atoi (buff);
   anImageRVB.writePix(i,j,red,green,blue);
  }
 }
}

/*---*/

static
void PPM_saveAsciiP3RVB(FILE * fd, const char *fileName,
                        const ImageRVB& anImageRVB)
{
 int  i = 0, j = 0;
 int  nbRow = anImageRVB.getNbRow(), nbCol = anImageRVB.getNbCol();
 char buff [255];
 char car;
 int  inc;

 fprintf(stderr,"Saving RVB image PPM Ascii (P3) File: %s\n",fileName);

 strcpy(buff,"P3\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);
 sprintf(buff,"# saveImagePPM V0.0\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);
 sprintf(buff,"%d %d\n",nbCol,nbRow);
 fwrite(buff,sizeof(char),strlen(buff),fd);
 strcpy(buff,"255\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);


 for(i=0,inc=0;i<nbRow;i++)
  {
    for(j=0;j<nbCol;j++)
     {
       octet red, green, blue;
       anImageRVB.readPix(i,j,red,green,blue);
       sprintf(buff,"%3d %3d %3d",red,green,blue);
       fwrite(buff,sizeof(char),strlen(buff),fd);
       inc ++;
       if (inc==5) {
                      inc = 0;
                      car = '\n';
       }
       else {
                      car = ' ';
       }

       fwrite(&car,sizeof(car),1,fd);
     }
  }

  car = '\n';
  fwrite(&car,sizeof(car),1,fd);
}

/*---*/

static
void PPM_loadBinP6RVB(FILE * fd, const char *fileName, ImageRVB& anImageRVB)
{
 int i = 0, j = 0;
 int nbRow = anImageRVB.getNbRow(), nbCol = anImageRVB.getNbCol();

 fprintf(stderr,"Loading RVB image from %s (PPM Bin File (P6))\n",fileName);

 fgetc(fd);  /* To skip a byte */

 for (i = 0 ; i < nbRow ; i++)
 {
  for (j = 0 ; j < nbCol ; j++)
  {
   octet red, green, blue;
   red   = fgetc(fd);
   green = fgetc(fd);
   blue  = fgetc(fd);
   anImageRVB.writePix(i,j,red,green,blue);
  }
 }
}

/*---*/

static
void PPM_saveBinP6RVB(FILE * fd, const char *fileName,
                                 const ImageRVB& anImageRVB)
{
 int i = 0, j = 0;
 int nbRow = anImageRVB.getNbRow(), nbCol = anImageRVB.getNbCol();

 char buff [255];

 fprintf(stderr,"Saving RVB image PPM Bin (P6) File: %s\n",fileName);

 strcpy(buff,"P6\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);
 sprintf(buff,"# saveImagePPM V0.0\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);
 sprintf(buff,"%d %d\n",nbCol,nbRow);
 fwrite(buff,sizeof(char),strlen(buff),fd);
 strcpy(buff,"255\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);

 for (i = 0 ; i < nbRow ; i++)
 {
  for (j = 0 ; j < nbCol ; j++)
  {
   octet red, green, blue;
   anImageRVB.readPix(i,j,red,green,blue);
   fprintf(fd,"%c%c%c",red,green,blue);
  }
 }
}

/*---*/

void _loadImagePPMRVB(const char *fileName, ImageRVB& anImageRVB)
{
 FILE * fd;
 char buff [1024];
 size_t nbRow = 0, nbCol = 0, depth = 0;
 char Type[1024];

 if ((fd=fopen(fileName,"rb"))==NULL) {
     fprintf(stderr,"Can't Read PPM file: %s\n",fileName);
     exit(1);
 }

  /* Lecture en-tete */
 fgets(buff,255,fd);
 sscanf(buff,"%s",Type);

  /* Lecture taille */
 fgets(buff,255,fd);
 while (buff[0]=='#') fgets(buff,255,fd);

 sscanf(buff,"%u %u",&nbCol,&nbRow);

  /* Lecture profondeur */
 fscanf(fd,"%s",buff);
 depth = atoi(buff);

 /*#####################################################################*/
 if (nbRow!=anImageRVB.getNbRow() || nbCol!=anImageRVB.getNbCol())
 {
  fprintf(stderr,"Error : Bad Image Size! Must Be : %dX%d\n",
                  anImageRVB.getNbRow(),anImageRVB.getNbCol());
  fprintf(stderr,"===> %s is not %dX%d\n",fileName,
                  anImageRVB.getNbRow(),anImageRVB.getNbCol());
  fclose(fd);
  exit(1);
 }
 /*#####################################################################*/
 if (depth != 255)
 {
  fprintf(stderr,"Error PPM File: %s, Bad Depth Value!\n",fileName);
  fclose(fd);
  exit(1);
 }
 /*#####################################################################*/

 if (!strcmp(Type,"P3")) PPM_loadAsciiP3RVB(fd,fileName,anImageRVB);
 else
 if (!strcmp(Type,"P6")) PPM_loadBinP6RVB(fd,fileName,anImageRVB);
 else
 {
   fprintf(stderr,
           "Error: %s Bad PPM Image File (Must be P3 or P6)\n",fileName);
   fclose(fd);
   exit(1);
 }

 fclose(fd);
}

/*---*/

void _saveImagePPMRVB(const char *fileName, const ImageRVB& anImageRVB)
{
 FILE * fd;
 const char *extension = strstr(fileName,".ascii.ppm");
 
 if ((fd=fopen(fileName,"wb"))==NULL) {
      fprintf(stderr,"Can't Save PPM File: %s\n",fileName);
      exit(1);
 }

 if (extension==NULL) extension="";

 if (!strcmp(extension,".ascii.ppm"))
 {
  PPM_saveAsciiP3RVB(fd,fileName,anImageRVB);
 }
 else {
  PPM_saveBinP6RVB(fd,fileName,anImageRVB);
 }

 fclose(fd);
}

/***********************************************************************/
/********************  Fin sur des fichiers PPM ************************/
/***********************************************************************/

/***********************************************************************/
/********************  Et sur des fichiers PGM *************************/
/***********************************************************************/
/* ------- Code des fonctions "privees" pgm : --------------------------- */
/*               _getSizePM, _loadImagePGM, _saveImagePGM                 */
/*------------------------------------------------------------------------*/

static void _getSizePGM(const char *fileName, size_t& nbRow, size_t& nbCol)
{
 FILE * fd;
 char buff [1024];
 char Type[1024];
 int  _nbRow,_nbCol;

 if ((fd=fopen(fileName,"rb"))==NULL) {
   fprintf(stderr,"Can't Read PGM while getSize : %s\n",fileName);
   return;
 }

  /* Lecture en-tete */
 fgets(buff,255,fd);
 sscanf(buff,"%s",Type);

  /* Lecture taille */
 fgets(buff,255,fd);
 while (buff[0]=='#') fgets(buff,255,fd);

 sscanf(buff,"%d %d",&_nbCol,&_nbRow);

 nbRow=_nbRow;
 nbCol=_nbCol;

 fclose(fd);
}

static
void PGM_loadAsciiP2(FILE * fd, const char *fileName, Image& anImage)
{
 int  i = 0, j = 0;
 int  nbRow = anImage.getNbRow(), nbCol = anImage.getNbCol();
 char buff [255];

 fprintf(stderr,"Loading image from %s (PGM Ascii File (P2))\n",fileName);

 for (i = 0 ; i < nbRow ; i++)
 {
  for (j = 0 ; j < nbCol ; j++)
  {
   octet val;
   fscanf(fd,"%s",buff);
   val = atoi (buff);
   anImage[i][j] = val;
  }
 }
}

/*---*/

static
void PGM_saveAsciiP2(FILE * fd, const char *fileName,const Image& anImage)
{
 int  i = 0, j = 0;
 int  nbRow = anImage.getNbRow(), nbCol = anImage.getNbCol();

 int  inc;
 char car;
 char buff [1024];

 fprintf(stderr,"Saving image PGM Ascii (P2) File: %s\n",fileName);

 strcpy(buff,"P2\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);

 sprintf(buff,"# saveImagePGM V0.0\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);
 sprintf(buff,"%d %d\n",nbCol,nbRow);
 fwrite(buff,sizeof(char),strlen(buff),fd);
 strcpy(buff,"255\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);

 for(i=0,inc=0;i<nbRow;i++)
  {
    for(j=0;j<nbCol;j++)
     {
       sprintf(buff,"%3d",anImage[i][j]);
       fwrite(buff,sizeof(char),strlen(buff),fd);
       inc ++;
       if (inc==15) {
                      inc = 0;
                      car = '\n';
       }
       else {
                      car = ' ';
       }

       fwrite(&car,sizeof(car),1,fd);
     }
  }

  car = '\n';
  fwrite(&car,sizeof(car),1,fd);
}

/*---*/

static
void PGM_loadBinP5(FILE * fd, const char *fileName, Image& anImage)
{
 int  i = 0, j = 0;
 int  nbRow = anImage.getNbRow(), nbCol = anImage.getNbCol();

 fprintf(stderr,"Loading image from %s (PGM Bin File (P5))\n",fileName);

 fgetc(fd);  /* To skip a byte */

 /*
 for (i = 0 ; i < nbRow ; i++)
 {
  for (j = 0 ; j < nbCol ; j++)
  {
   char val;
   fscanf(fd,"%c",&val);
   anImage[i][j] = (octet)val;
  }
 }
 */

 /* Ou en une seule lecture ... */
     (void)i; (void)j;
     fread(anImage.begin(),sizeof(octet),nbRow*nbCol,fd);
}

/*---*/

static
void PGM_saveBinP5(FILE * fd, const char *fileName,const Image& anImage)
{
 int  i = 0, j = 0;
 int  nbRow = anImage.getNbRow(), nbCol = anImage.getNbCol();

 char buff [1024];

 fprintf(stderr,"Saving image PGM Bin (P5) File: %s\n",fileName);

 strcpy(buff,"P5\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);

 sprintf(buff,"# saveImagePGM V0.0\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);
 sprintf(buff,"%d %d\n",nbCol,nbRow);
 fwrite(buff,sizeof(char),strlen(buff),fd);
 strcpy(buff,"255\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);

 /*
 for (i = 0 ; i < nbRow ; i++)
 {
  for (j = 0 ; j < nbCol ; j++)
  {
   char val;
   val = (char)anImage[i][j];
   fprintf(fd,"%c",val);
  }
 }
 */
 /* Ou en une seule ecriture ... */
   (void)i; (void)j;

   fwrite(anImage.begin(),sizeof(octet),nbRow*nbCol,fd);
}

/*---*/

static void _loadImagePGM(const char *fileName, Image& anImage)
{
 FILE * fd;
 char buff [1024];
 size_t  nbRow = 0, nbCol = 0;
 int  depth = 0;
 char Type[1024];

 if ((fd=fopen(fileName,"rb"))==NULL) {
      fprintf(stderr,"Can't Read PGM file: %s\n",fileName);
      exit(1);
  }

  /* Lecture en-tete */
 fgets(buff,255,fd);
 sscanf(buff,"%s",Type);

  /* Lecture taille */
 fgets(buff,255,fd);
 while (buff[0]=='#') fgets(buff,255,fd);

 sscanf(buff,"%u %u",&nbCol,&nbRow);

 fscanf(fd,"%s",buff);
 depth = atoi(buff);

 /*#####################################################################*/
 if (nbRow!=anImage.getNbRow() || nbCol!=anImage.getNbCol())
 {
  fprintf(stderr,"Error : Bad Image Size! Must Be : %dX%d\n",
                  anImage.getNbRow(),anImage.getNbCol());
  fprintf(stderr,"===> %s is not %dX%d\n",fileName,
                  anImage.getNbRow(),anImage.getNbCol());
  fclose(fd);
  exit(1);
 }
 /*#####################################################################*/
 if (depth != 255)
 {
  fprintf(stderr,"Error PGM File: %s, Bad Depth Value!\n",fileName);
  fclose(fd);
  exit(1);
 }
 /*#####################################################################*/

 if (!strcmp(Type,"P2")) PGM_loadAsciiP2(fd,fileName,anImage);
 else
 if (!strcmp(Type,"P5")) PGM_loadBinP5(fd,fileName,anImage);
 else
 {
   fprintf(stderr,
           "Error: %s Bad PGM Image File (Must be P2 or P5)\n",fileName);
   fclose(fd);
   exit(1);
 }

 fclose(fd);
}

/*---*/

static void _saveImagePGM(const char *fileName, const Image& anImage)
{
 FILE * fd;
 const char *extension = strstr(fileName,".ascii.pgm");

 if ((fd=fopen(fileName,"wb"))==NULL) {
      fprintf(stderr,"Can't Save PGM File: %s\n",fileName);
      exit(1);
 }
 
 if (extension==NULL) extension="";

 if (!strcmp(extension,".ascii.pgm"))
 {
  PGM_saveAsciiP2(fd,fileName,anImage);
 }
 else {
  PGM_saveBinP5(fd,fileName,anImage);
 }

 fclose(fd);
}

/*---*/

static
void PGM_loadAsciiP2RVB(FILE * fd, const char *fileName, ImageRVB& anImageRVB)
{
 int  i = 0, j = 0;
 int  nbRow = anImageRVB.getNbRow(), nbCol = anImageRVB.getNbCol();
 char buff [255];

 fprintf(stderr,"Loading RVB image from %s (PGM Ascii File (P2))\n",
         fileName);

 for (i = 0 ; i < nbRow ; i++)
 {
  for (j = 0 ; j < nbCol ; j++)
  {
   octet val;
   fscanf(fd,"%s",buff);
   val = atoi (buff);
   anImageRVB.writePix(i,j,val,val,val);
  }
 }
}

/*---*/

static
void PGM_saveAsciiP2RVB(FILE * fd, const char *fileName,
                                   const ImageRVB& anImageRVB)
{
 int  i = 0, j = 0;
 int  nbRow = anImageRVB.getNbRow(), nbCol = anImageRVB.getNbCol();

 int  inc;
 char car;
 char buff [1024];

 fprintf(stderr,"Saving RVB image PGM Ascii (P2) File: %s\n",fileName);

 strcpy(buff,"P2\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);

 sprintf(buff,"# saveImagePGM V0.0\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);
 sprintf(buff,"%d %d\n",nbCol,nbRow);
 fwrite(buff,sizeof(char),strlen(buff),fd);
 strcpy(buff,"255\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);

 for(i=0,inc=0;i<nbRow;i++)
  {
    for(j=0;j<nbCol;j++)
     {
       octet red,green,blue; octet val;
       anImageRVB.readPix(i,j,red,green,blue);
       val = (char)_RVB_To_MONO(red,green,blue);
       sprintf(buff,"%3d",val);
       fwrite(buff,sizeof(char),strlen(buff),fd);
       inc ++;
       if (inc==15) {
                      inc = 0;
                      car = '\n';
       }
       else {
                      car = ' ';
       }

       fwrite(&car,sizeof(car),1,fd);
     }
  }

  car = '\n';
  fwrite(&car,sizeof(car),1,fd);
}

/*---*/

static
void PGM_loadBinP5RVB(FILE * fd, const char *fileName, ImageRVB& anImageRVB)
{
 int  i = 0, j = 0;
 int  nbRow = anImageRVB.getNbRow(), nbCol = anImageRVB.getNbCol();

 fprintf(stderr,"Loading RVB image from %s (PGM Bin File (P5))\n",fileName);

 fgetc(fd);  /* To skip a byte */

 /*
 for (i = 0 ; i < nbRow ; i++)
 {
  for (j = 0 ; j < nbCol ; j++)
  {
   char val;
   fscanf(fd,"%c",&val);
   anImageRVB.writePix(i,j,val,val,val);
  }
 }
 */

 /* Ou en une seule lecture ... */
     (void)i; (void)j;

     Image& R = anImageRVB.getR();
     Image& V = anImageRVB.getV();
     Image& B = anImageRVB.getB();

     fread(R.begin(),sizeof(octet),nbRow*nbCol,fd);

     V = R;   // copyImage(&out->R,&out->V);
     B = R;   // copyImage(&out->R,&out->B);
}

/*---*/

static
void PGM_saveBinP5RVB(FILE * fd, const char *fileName,
                                 const ImageRVB& anImageRVB)
{
 int  i = 0, j = 0;
 int  nbRow = anImageRVB.getNbRow(), nbCol = anImageRVB.getNbCol();

 char buff [1024];

 fprintf(stderr,"Saving RVB image PGM Bin (P5) File: %s\n",fileName);

 strcpy(buff,"P5\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);

 sprintf(buff,"# saveImagePGM V0.0\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);
 sprintf(buff,"%d %d\n",nbCol,nbRow);
 fwrite(buff,sizeof(char),strlen(buff),fd);
 strcpy(buff,"255\n");
 fwrite(buff,sizeof(char),strlen(buff),fd);

 for (i = 0 ; i < nbRow ; i++)
 {
  for (j = 0 ; j < nbCol ; j++)
  {
   octet red,green,blue; octet val;
   anImageRVB.readPix(i,j,red,green,blue);
   val = (char)_RVB_To_MONO(red,green,blue);
   fprintf(fd,"%c",val);
  }
 }
 /* Ou en une seule ecriture ... pas possible sans allocation */
}

/*---*/

static void _loadImagePGMRVB(const char *fileName, ImageRVB& anImageRVB)
{
 FILE * fd;
 char buff [1024];
 size_t nbRow = 0, nbCol = 0;
 int depth = 0;
 char Type[1024];

 if ((fd=fopen(fileName,"rb"))==NULL) {
      fprintf(stderr,"Can't Read PGM file: %s\n",fileName);
      exit(1);
  }

  /* Lecture en-tete */
 fgets(buff,255,fd);
 sscanf(buff,"%s",Type);

  /* Lecture taille */
 fgets(buff,255,fd);
 while (buff[0]=='#') fgets(buff,255,fd);

 sscanf(buff,"%u %u",&nbCol,&nbRow);

 fscanf(fd,"%s",buff);
 depth = atoi(buff);

 /*#####################################################################*/
 if (nbRow!=anImageRVB.getNbRow() || nbCol!=anImageRVB.getNbCol())
 {
  fprintf(stderr,"Error : Bad Image Size! Must Be : %dX%d\n",
                  anImageRVB.getNbRow(),anImageRVB.getNbCol());
  fprintf(stderr,"===> %s is not %dX%d\n",fileName,
                  anImageRVB.getNbRow(),anImageRVB.getNbCol());
  fclose(fd);
  exit(1);
 }
 /*#####################################################################*/
 if (depth != 255)
 {
  fprintf(stderr,"Error PGM File: %s, Bad Depth Value!\n",fileName);
  fclose(fd);
  exit(1);
 }
 /*#####################################################################*/

 if (!strcmp(Type,"P2")) PGM_loadAsciiP2RVB(fd,fileName,anImageRVB);
 else
 if (!strcmp(Type,"P5")) PGM_loadBinP5RVB(fd,fileName,anImageRVB);
 else
 {
   fprintf(stderr,
           "Error: %s Bad PGM Image File (Must be P2 or P5)\n",fileName);
   fclose(fd);
   exit(1);
 }

 fclose(fd);
}

/*---*/

static void _saveImagePGMRVB(const char *fileName, const ImageRVB& anImageRVB)
{
 FILE * fd;
 const char *extension = strstr(fileName,".ascii.pgm");

 if ((fd=fopen(fileName,"wb"))==NULL) {
      fprintf(stderr,"Can't Save PGM File: %s\n",fileName);
      exit(1);
 }
 
 if (extension==NULL) extension="";

 if (!strcmp(extension,".ascii.pgm"))
 {
  PGM_saveAsciiP2RVB(fd,fileName,anImageRVB);
 }
 else {
  PGM_saveBinP5RVB(fd,fileName,anImageRVB);
 }

 fclose(fd);
}


/***********************************************************************/
/********************  Fin sur des fichiers PGM ************************/
/************************************************************************/
