#include <iostream>

using namespace std;

#include "LibImages.h"

int main(void)
{
 Image    im1(256,256);
 im1.setImage(100);
 Image    im2(im1.getNbRow(),im1.getNbCol());
 im2.setImage(255);
 Image    im3(im1.getNbRow(),im1.getNbCol());
 Image    im4(im1.getNbRow(),im1.getNbCol());

 for(int l=20; l<100; l++)
 {
   for(int c=30;c<200;c++)
   {
    im2(l,c)=0;
   }
 }

 im3 = im1 + im2;

 im4 = im3;

 im3 -= im2;

 XAffichage *Fim1  = new XAffichage(im1.getNbRow(),im1.getNbCol());
 Fim1->setLabel("im1");
 XAffichage *Fim2  = new XAffichage(im2.getNbRow(),im2.getNbCol());
 Fim2->setLabel("im2");
 XAffichage *Fim3  = new XAffichage(im3.getNbRow(),im3.getNbCol());
 Fim3->setLabel("im3");
 XAffichage *Fim4  = new XAffichage(im4.getNbRow(),im4.getNbCol());
 Fim4->setLabel("im4");

 while (1)
 {
  Fim1->Afficher(im1);
  Fim1->XEvenement(im1);

  Fim2->Afficher(im2);
  Fim2->XEvenement(im2);

  Fim3->Afficher(im3);
  Fim3->Afficher(im3);

  Fim4->XEvenement(im4);
  Fim4->XEvenement(im4);
 }

 return(0);
}
