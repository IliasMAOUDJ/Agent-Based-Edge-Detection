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
#include <algorithm>

int main(int argc, char *argv[])
{
  Scheduler sched;

  System sys;

  //--
  Image &im = sys.im;
  Image &originale = sys.originale;
  Image &preprocessed = sys.preprocessed;
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
  preprocessed.setImageSize(im.getNbRow(), im.getNbCol());
  resultat.setImageSize(im.getNbRow(), im.getNbCol());
  resultat.setImage(128);
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

  for (int i = 0; i < nbagents; i++)
  {
    new KirschAgent(&sys);
    //new FilterAgent(&sys);
  }

  //--
  octet v, newValPix;
  for(size_t r=2; r<im.getNbRow()-2;++r)
  {
    for(size_t c=2; c<im.getNbCol()-2;++c)
    {
      vector<octet> valPixs;
      for (int i = (int)r - 2; i <= (int)r + 2; i++)
     {
         for (int j = (int)c - 2; j <= (int)c + 2; j++) // le filtre median a un noyau de 3x3
         {
             v = im[i][j];
             valPixs.push_back(v);
         }
     }   
     sort(valPixs.begin(), valPixs.end());
     newValPix = valPixs.at(12); // filtre 5x5 donc 25 valeurs -> le median est la 5e valeur donc � l'indice  12 
     preprocessed[r][c] = newValPix;
    }
  }

  //--

  int indSauvegardeIm = 1;
  int indSauvegardeImResultat = 1;
  char nomSauvegarde[2048];

  cout << "s: sauvegarder" << endl;
  cout << "q: quitter" << endl;

  while (1)
  {
    char cim;
    char cimr, cimp;

    Fim->Afficher(originale);
    cim = Fim->XEvenement(originale);
    cim = tolower(cim);

    Fpreprocessed->Afficher(im);
    cimp = Fpreprocessed->XEvenement(im);
    cimp = tolower(cimp);
    
    Fresultat->Afficher(resultat);
    cimr = Fresultat->XEvenement(resultat);
    cimr = tolower(cimr);

    im = preprocessed;

    sched.cycle();

    // "Affichage" dans im de tous les "ImAgent"
      vector<Agent *> v;
      getAllAgents("EdgeFollowingAgent", v);
      size_t nbImAgents = v.size();

      for (size_t indV = 0; indV < nbImAgents; indV++)
      {
        EdgeFollowingAgent *imAgent = (EdgeFollowingAgent *)v[indV];
        imAgent->draw(im);
      }


      vector<Agent *> e;
      getAllAgents("KirschAgent", e);
      size_t nbKAgents = e.size();

      for (size_t indV = 0; indV < nbKAgents; indV++)
      {
        KirschAgent *imAgent = (KirschAgent *)e[indV];
        imAgent->draw(im);
      }

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


    if (cimr == 'q')
      break;
  }

  delete (Fim);
  delete (Fpreprocessed);
  delete (Fresultat);

  return (0);
}
