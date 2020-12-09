#ifndef _ESImage_H_
#define _ESImage_H_

#include "LibImages.h"

void loadImageFile(const char* fileName, Image& anImage);
void saveImageFile(const char* fileName, const Image& anImage);

void loadImageFileRVB(const char* fileName, ImageRVB& anImageRVB);
void saveImageFileRVB(const char* fileName, const ImageRVB& anImageRVB);

                                  // Affichage avec un outil : xv par defaut
void displayImage(const char *nomFichier, const char *visualiseur = "xv");

// Formats pris en charge : .ras (Sun), .ppm, .pgm
//
// Lors du chargement indiquer l'extension : .ras, .ppm ou .pgm
//
// Lors de la sauvegarde indiquer l'extension : .ras, .ppm ou .pgm
//              par defaut sauvegarde en binaire pour .ppm et .pgm 
//     ==> pour une sauvegarde en ascii : .ascii.ppm et .ascii.pgm

#endif // _ESImage_H_
