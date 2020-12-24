#include <iostream>

#include <vector>

#define NBAGENTS 10000

using namespace std;

#include "LibImages.h"
#include "MAS.h"
#include "Edge_detection.h"
#include "FilterAgent.h"
#include "EdgeFollowingAgent.h"
#include "system.h"
#include "ImAgent.h"
#include <cstring>
#include <stdio.h>

int main(int argc, char *argv[])
{
  Scheduler sched;

  System sys;

  //--
  Image &im = sys.im;
  Image &originale = sys.originale;
  //Image &postprocessed = sys.postprocessed;
  Image &resultat = sys.resultat;

  //--

  if (argc < 2)
  {
    cout << "\nSyntax error: Must be ./progX <filename>" << endl;
    cout << "optional arguments: <NBAGENTS>\n"
         << endl;
    return -1;
  }
  string str = "Images/";
  char *file = argv[1];
  char *path = &str[0];
  char *filename = strcat(path, file);
  originale.loadImage(filename);
  int nbagents = NBAGENTS;
  if (argc == 3)
  {
    nbagents = atoi(argv[2]);
    if (nbagents == 0)
    {
      cout << "\nSyntax error: NBAGENTS isn't int" << endl;
      return -1;
    }
  }

  im = originale;
  //postprocessed.setImageSize(im.getNbRow(), im.getNbCol());
  resultat.setImageSize(im.getNbRow(), im.getNbCol());
  //resultat.setImage(255);
  /**/
  //--
  XAffichage *Fim = new XAffichage(im.getNbRow(), im.getNbCol());
  //XAffichage *Fpostprocessed = new XAffichage(postprocessed.getNbRow(),
  //                                            postprocessed.getNbCol());
  XAffichage *Fresultat = new XAffichage(resultat.getNbRow(),
                                         resultat.getNbCol());

  //Fim->Afficher(im);
  //Fim->XEvenement(im);
  //Fpostprocessed->Afficher(postprocessed);
  //Fpostprocessed->XEvenement(postprocessed);
  Fresultat->Afficher(resultat);
  Fresultat->XEvenement(resultat);

  //bool exploredPixelsMatrix [im.getNbRow()][im.getNbCol()] = {{0}};
  //--

  for (int i = 0; i < nbagents; i++)
  {
    new KirschAgent(&sys);
    //new FilterAgent(&sys);
  }

  //--

  //int indSauvegardeIm = 1;
  int indSauvegardeImResultat = 1;
  char nomSauvegarde[2048];

  cout << "s: sauvegarder" << endl;
  cout << "q: quitter" << endl;

  while (1)
  {
    //char cim;
    char cimr, cimp;

    //Fim->Afficher(im);
    //cim = Fim->XEvenement(im);
    //cim = tolower(cim);

    //Fpostprocessed->Afficher(postprocessed);
    //cimp = Fpostprocessed->XEvenement(postprocessed);
    //cimp = tolower(cimp);
    Fresultat->Afficher(resultat);
    cimr = Fresultat->XEvenement(resultat);
    cimr = tolower(cimr);

    im = originale;

    sched.cycle();

    // "Affichage" dans im de tous les "ImAgent"

    /*  
    if (cim == 's')
    {
      snprintf(nomSauvegarde, sizeof(nomSauvegarde),
               "Resultats/ImageIm_%d.ppm", indSauvegardeIm);
      im.saveImage(nomSauvegarde);
      indSauvegardeIm++;
    }
*/
    if (cimr == 's')
    {
      snprintf(nomSauvegarde, sizeof(nomSauvegarde),
               "Resultats/ImageImResultat_%d.ppm",
               indSauvegardeImResultat);
      resultat.saveImage(nomSauvegarde);
      indSauvegardeImResultat++;
    }

    if (cimp == 'p')
    {     
      /*
      for (int i = 0; i < nbagents; i++)
      {
        new FilterAgent(&sys);
      }
      cout << "Done" << endl;
      vector<Agent *> v;
      getAllAgents("FilterAgent", v);

      size_t nbImAgents = v.size();

      for (size_t indV = 0; indV < nbImAgents; indV++)
      {
        FilterAgent *imAgent = (FilterAgent *)v[indV];
        imAgent->draw(postprocessed);
      }
      */
    }

    if (cimr == 'q')
      break;
  }

  delete (Fim);
  delete (Fresultat);

  return (0);
}
