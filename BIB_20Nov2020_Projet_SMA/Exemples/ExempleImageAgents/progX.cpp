#include <iostream>

#include <vector>

#define NBAGENTS 2000

using namespace std;

#include "LibImages.h"
#include "MAS.h"
#include "Edge_detection.h"
#include "FilterAgent.h"
#include "ExplorationAgent.h"
#include "system.h"
#include "ImAgent.h"
#include <cstring>
#include <stdio.h>

int main(int argc, char *argv[])
{
  Scheduler sched;

  System sys;

  //--
  if (argc ==1)
  {
    cout << "Please enter a file name"<< endl;
    return 0;
  }
  Image &im = sys.im;
  Image &originale = sys.originale;
  Image &preprocessed = sys.preprocessed;
  Image &resultat = sys.resultat;

  //--
  string str = "Images/";
  char *file = argv[1];
  char *path = &str[0];
  char *filename = strcat(path, file);
  originale.loadImage(filename);

  im = originale;
  preprocessed.setImageSize(im.getNbRow(), im.getNbCol());
  resultat.setImageSize(im.getNbRow(), im.getNbCol());
  /**/
  //--
  XAffichage *Fim = new XAffichage(im.getNbRow(), im.getNbCol());
  XAffichage *Fpreprocessed = new XAffichage(preprocessed.getNbRow(),
                                             preprocessed.getNbCol());
  XAffichage *Fresultat = new XAffichage(resultat.getNbRow(),
                                         resultat.getNbCol());

  Fim->Afficher(im);
  Fim->XEvenement(im);
  Fpreprocessed->Afficher(preprocessed);
  Fpreprocessed->XEvenement(preprocessed);
  Fresultat->Afficher(resultat);
  Fresultat->XEvenement(resultat);

  //bool exploredPixelsMatrix [im.getNbRow()][im.getNbCol()] = {{0}};
  //--

  for (int i = 0; i < NBAGENTS; i++)
  {
    //new ImAgent(&sys);
    new KirschAgent(&sys);
    new FilterAgent(&sys);
    //new ExplorationAgent(&sys);
  }

  //--

  int indSauvegardeIm = 1;
  int indSauvegardeImResultat = 1;
  char nomSauvegarde[2048];

  cout << "s: sauvegarder" << endl;
  cout << "q: quitter" << endl;

  while (1)
  {
    char cim, cimr, cimp;

    Fim->Afficher(im);
    cim = Fim->XEvenement(im);
    cim = tolower(cim);

    Fpreprocessed->Afficher(preprocessed);
    cimp = Fpreprocessed->XEvenement(preprocessed);
    cimp = tolower(cimp);
    Fresultat->Afficher(resultat);
    cimr = Fresultat->XEvenement(resultat);
    cimr = tolower(cimr);

    im = originale;

    sched.cycle();

    // "Affichage" dans im de tous les "ImAgent"

    vector<Agent *> v;
    getAllAgents("Agents", v);
    /*
     size_t nbImAgents = v.size();
     
     for(size_t indV=0; indV < nbImAgents ; indV++)
     {
      FilterAgent *imAgent = (FilterAgent*)v[indV];
      imAgent->draw(im);
     }
    */
    if (cim == 's')
    {
      snprintf(nomSauvegarde, sizeof(nomSauvegarde),
               "Resultats/ImageIm_%d.ppm", indSauvegardeIm);
      im.saveImage(nomSauvegarde);
      indSauvegardeIm++;
    }

    if (cimr == 's')
    {
      snprintf(nomSauvegarde, sizeof(nomSauvegarde),
               "Resultats/ImageImResultat_%d.ppm",
               indSauvegardeImResultat);
      resultat.saveImage(nomSauvegarde);
      indSauvegardeImResultat++;
    }

    if (cim == 'q' || cimr == 'q')
      break;
  }

  delete (Fim);
  delete (Fresultat);

  return (0);
}
