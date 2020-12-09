#include <iostream>

using namespace std;

#include "LibImages.h"

int main(void)
{
 ImageRVB im0("Images/CercleSinBruit.ppm");
 Image    im0NB("Images/CercleSinBruit.ppm");
 XAffichage *Fim0    = new XAffichage(im0.getNbRow(),im0.getNbCol());
 XAffichage *Fim0NB  = new XAffichage(im0NB.getNbRow(),im0NB.getNbCol());

#if 0   // Bin
 ImageRVB im1("Images/lenaColor.ras");
 Image    im1NB("Images/lenaColor.ras");
 cout << "************" << endl;
 ImageRVB im2("Images/lenaColorBin.ppm");
 Image    im2NB("Images/lenaColorBin.ppm");
 cout << "************" << endl;
 ImageRVB im3("Images/lenaNB.ras");
 Image    im3NB("Images/lenaNB.ras");
 cout << "************" << endl;
 ImageRVB im4("Images/lenaNBBin.pgm");
 Image    im4NB("Images/lenaNBBin.pgm");
 cout << "************" << endl;
#else  // Ascii
 ImageRVB im1("Images/lenaColor.ras");
 Image    im1NB("Images/lenaColor.ras");
 cout << "************" << endl;
 ImageRVB im2("Images/lenaColorAscii.ppm");
 Image    im2NB("Images/lenaColorAscii.ppm");
 cout << "************" << endl;
 ImageRVB im3("Images/lenaNB.ras");
 Image    im3NB("Images/lenaNB.ras");
 cout << "************" << endl;
 ImageRVB im4("Images/lenaNBAscii.pgm");
 Image    im4NB("Images/lenaNBAscii.pgm");
 cout << "************" << endl;
#endif

 cout << "************" << endl;
 if (im1!=im2) cout << "pb 1-2" << endl;
 else cout << "1=2" << endl;
 if (im1NB!=im2NB) cout << "pb 1NB-2NB" << endl;
 else cout << "1NB=2NB" << endl;

 cout << "************" << endl;

 if (im3!=im4) cout << "pb 3-4" << endl;
 else cout << "3=4" << endl;
 if (im3NB!=im4NB) cout << "pb 3NB-4NB" << endl;
 else cout << "3NB=4NB" << endl;

 cout << "************" << endl;

 XAffichage *Fim1    = new XAffichage(im1.getNbRow(),im1.getNbCol());
 XAffichage *Fim1NB  = new XAffichage(im1NB.getNbRow(),im1NB.getNbCol());
 XAffichage *Fim2    = new XAffichage(im2.getNbRow(),im2.getNbCol());
 XAffichage *Fim2NB  = new XAffichage(im2NB.getNbRow(),im2NB.getNbCol());
 XAffichage *Fim3    = new XAffichage(im3.getNbRow(),im3.getNbCol());
 XAffichage *Fim3NB  = new XAffichage(im3NB.getNbRow(),im3NB.getNbCol());
 XAffichage *Fim4    = new XAffichage(im4.getNbRow(),im4.getNbCol());
 XAffichage *Fim4NB  = new XAffichage(im4NB.getNbRow(),im4NB.getNbCol());

 while (1)
 {
  Fim0->Afficher(im0);
  Fim0->XEvenement(im0);
  Fim0NB->Afficher(im0NB);
  Fim0NB->XEvenement(im0NB);

  Fim1->Afficher(im1);
  Fim1->XEvenement(im1);
  Fim1NB->Afficher(im1NB);
  Fim1NB->XEvenement(im1NB);

  Fim2->Afficher(im2);
  Fim2->XEvenement(im2);
  Fim2NB->Afficher(im2NB);
  Fim2NB->XEvenement(im2NB);

  Fim3->Afficher(im3);
  Fim3->XEvenement(im3);
  Fim3NB->Afficher(im3NB);
  Fim3NB->XEvenement(im3NB);

  Fim4->Afficher(im4);
  Fim4->XEvenement(im4);
  Fim4NB->Afficher(im4NB);
  Fim4NB->XEvenement(im4NB);
 }

 return(0);
}
