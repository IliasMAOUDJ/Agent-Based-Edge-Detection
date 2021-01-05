#include <iostream>

#include <vector>

#define NBAGENTS 1000

using namespace std;

#include "LibImages.h"
#include "MAS.h"
#include "Edge_detection.h"
#include "EdgeFollowingAgent.h"
#include "NodeAgent.h"
#include "system.h"
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
  Image &explored = sys.explored;
  ImageRVB &superposed = sys.superposed;

  sys.pixelExplored = 0;
  int &pixelExplored = sys.pixelExplored;

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
  if (argc >= 3)
  {
    nbagents = atoi(argv[2]);
    if (nbagents == 0)
    {
      cout << "\nNBAGENTS isn't int" << endl;
      cout << "Using default value: 1000" << endl;
    }
  }

  im = originale;
  preprocessed.setImageSize(im.getNbRow(), im.getNbCol());

  resultat.setImageSize(im.getNbRow(), im.getNbCol());
  resultat.setImage(128);

  explored.setImageSize(im.getNbRow(), im.getNbCol());

  superposed.setImageSize(im.getNbRow(), im.getNbCol());

  //--

  //XAffichage *Fim = new XAffichage(im.getNbRow(), im.getNbCol());
  XAffichage *Fpreprocessed = new XAffichage(preprocessed.getNbRow(),
                                             preprocessed.getNbCol());
  XAffichage *Fresultat = new XAffichage(resultat.getNbRow(),
                                         resultat.getNbCol());

  XAffichage *Fsuperposed = new XAffichage(superposed.getNbRow(), superposed.getNbCol());

  Fpreprocessed->Afficher(preprocessed);
  Fpreprocessed->XEvenement(preprocessed);
  Fpreprocessed->setLabel("ENVIRONMENT");

  Fresultat->Afficher(resultat);
  Fresultat->XEvenement(resultat);
  Fresultat->setLabel("RESULTAT");

  for (int i = 0; i < nbagents; i++)
  {
    new KirschAgent(&sys);
  }

  //--
  octet v, newValPix;
  bool preprocess = false;
  int k = 0;
  if (argc == 5 && string(argv[3]) == "-f")
  {
    preprocess = true;
    int kernel = atoi(argv[4]);
    k = (kernel - 1) / 2;
  }

  for (int r = 0; r < (int)im.getNbRow(); ++r)
  {
    for (int c = 0; c < (int)im.getNbCol(); ++c)
    {
      if (r < k || c < k || r > (int)im.getNbRow() - 1 - k || c > (int)im.getNbCol() - 1 - k) // if on border of the image just copy the original pixel
      {
        preprocessed[r][c] = originale[r][c];
      }
      else
      {
        if (preprocess) // "-f" as filter, allowing preprocessing of the image with a median filter.
        {
          vector<octet> valPixs;
          for (int i = (int)r - k; i <= (int)r + k; i++)
          {
            for (int j = (int)c - k; j <= (int)c + k; j++) // le filtre median a un noyau de 5x5
            {
              v = im[i][j];
              valPixs.push_back(v);
            }
          }
          sort(valPixs.begin(), valPixs.end());
          newValPix = valPixs.at(((valPixs.size()) - 1) / 2);
          preprocessed[r][c] = newValPix;
        }
        else
        {
          preprocessed[r][c] = originale[r][c];
        }
      }
    }
  }

  superposed = preprocessed;
  Fsuperposed->Afficher(superposed);
  Fsuperposed->setLabel("SUPERPOSED");
  //--

  int indSauvegardeIm = 1;
  int indSauvegardeImResultat = 1;
  int indSauvegardeImSuperposed = 1;
  char nomSauvegarde[2048];

  cout << "s: sauvegarder" << endl;
  cout << "q: quitter" << endl;

  while (1)
  {
    char cimr, cimp, cims;

    Fpreprocessed->Afficher(im);
    cimp = Fpreprocessed->XEvenement(im);
    cimp = tolower(cimp);

    Fresultat->Afficher(resultat);
    cimr = Fresultat->XEvenement(resultat);
    cimr = tolower(cimr);

    Fsuperposed->Afficher(superposed);
    cims = Fsuperposed->XEvenement(superposed);
    cims = tolower(cims);

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

    vector<Agent *> n;
    getAllAgents("NodeAgent", n);
    size_t NbNodeAgents = n.size();

    for (size_t indV = 0; indV < NbNodeAgents; indV++)
    {
      NodeAgent *imAgent = (NodeAgent *)n[indV];
      imAgent->draw(im);
    }

    if (cimp == 's' || pixelExplored == (int)(im.getNbCol() * im.getNbRow()))
    {
      snprintf(nomSauvegarde, sizeof(nomSauvegarde),
               "Resultats/%s_Environment_%d.ppm",
               file, indSauvegardeIm);
      preprocessed.saveImage(nomSauvegarde);
      indSauvegardeIm++;
    }

    if (cimr == 's' || pixelExplored == (int)(im.getNbCol() * im.getNbRow()))
    {
      snprintf(nomSauvegarde, sizeof(nomSauvegarde),
               "Resultats/%s_Resultat_%d.ppm",
               file, indSauvegardeImResultat);
      resultat.saveImage(nomSauvegarde);
      indSauvegardeImResultat++;
    }

    if (cims == 's' || pixelExplored == (int)(im.getNbCol() * im.getNbRow()))
    {
      snprintf(nomSauvegarde, sizeof(nomSauvegarde),
               "Resultats/%s_Superposed_%d.ppm",
               file, indSauvegardeImSuperposed);
      superposed.saveImage(nomSauvegarde);
      indSauvegardeImSuperposed++;
    }

    if (cimr == 'p' || cimp == 'p' || cims == 'p')
    {
      cout << float(pixelExplored * 100 / (im.getNbCol() * im.getNbRow())) << "% of the image explored" << endl;
    }
    if (cimr == 'q' || cimp == 'q' || cims == 'q' || pixelExplored == (int)(im.getNbCol() * im.getNbRow()))
    {
      cout << float(pixelExplored * 100 / (im.getNbCol() * im.getNbRow())) << "% of the image explored" << endl;
      break;
    }
  }

  delete (Fpreprocessed);
  delete (Fresultat);
  delete (Fsuperposed);

  return (0);
}
