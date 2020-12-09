#include <iostream>

#include <vector>

#define NBAGENTS 1000

using namespace std;

#include "LibImages.h"
#include "MAS.h"
#include "Edge_detection.h"
#include "system.h"
#include "ImAgent.h"

#include <stdio.h>

int main(void)
{
  Scheduler sched;

  System sys;

//--

  Image& im        = sys.im;
  Image& originale = sys.originale;
  Image& resultat  = sys.resultat;

//--

  originale.loadImage("Images/lena.ras");
  im = originale;
  resultat.setImageSize(originale.getNbRow(),originale.getNbCol());
/**/
//--

  XAffichage *Fim       = new XAffichage(im.getNbRow(),im.getNbCol());
  XAffichage *Fresultat = new XAffichage(resultat.getNbRow(),
                                         resultat.getNbCol());
  Fim->Afficher(im);
  Fim->XEvenement(im);

  Fresultat->Afficher(resultat);
  Fresultat->XEvenement(resultat);

//--

  for (int i = 0; i < NBAGENTS; i++) { new ImAgent(&sys); new Edge_detection_Agent(&sys); }

//--

  int  indSauvegardeIm=1;
  int  indSauvegardeImResultat=1;
  char nomSauvegarde[2048];

  cout << "s: sauvegarder" << endl;
  cout << "q: quitter" << endl;

  while (1)
    {
     char cim, cimr;

     Fim->Afficher(im);
     cim = Fim->XEvenement(im);
     cim = tolower(cim);

     Fresultat->Afficher(resultat);
     cimr = Fresultat->XEvenement(resultat);
     cimr = tolower(cimr);

     im = originale;

     sched.cycle();

     // "Affichage" dans im de tous les "ImAgent"

     vector<Agent*> v;
     getAllAgents("Edge_detection_Agent",v);

     size_t nbImAgents = v.size();
     for(size_t indV=0; indV < nbImAgents ; indV++)
     {
      //ImAgent *imAgent = (ImAgent*)v[indV];
      Edge_detection_Agent *imAgent = (Edge_detection_Agent*)v[indV];
      imAgent->draw(im);
     }

     if (cim=='s') {
                    snprintf(nomSauvegarde,sizeof(nomSauvegarde),
                        "Resultats/ImageIm_%d.ppm",indSauvegardeIm);
                    im.saveImage(nomSauvegarde);
                    indSauvegardeIm++;
     }

     if (cimr=='s') {
                    snprintf(nomSauvegarde,sizeof(nomSauvegarde),
                        "Resultats/ImageImResultat_%d.ppm",
                        indSauvegardeImResultat);
                    resultat.saveImage(nomSauvegarde);
                    indSauvegardeImResultat++;
     }

     if (cim=='q' || cimr=='q') break;
    }

 delete(Fim);
 delete(Fresultat);

 return(0);
}
