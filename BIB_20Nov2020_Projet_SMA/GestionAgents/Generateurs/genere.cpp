#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

//--
void petitNom(const string& className, string& varPetitNom)
{
 if (className.length()==0)
 {
  cerr << "petitNom: Nom de classe vide!"<< endl;
 }

 char   firstLetter = toupper(className.at(0));

 if (firstLetter=='A'||firstLetter=='E'||firstLetter=='I' ||
     firstLetter=='O'||firstLetter=='U'||firstLetter=='Y')   varPetitNom="an";
 else varPetitNom="a";

 varPetitNom += firstLetter;
}

//--
void transformeMajuscules(const string& src, string& dest)
{
 dest = src;
 for(size_t i=0; i<dest.length();i++)
 {
  char c = dest.at(i);
  dest.at(i)=toupper(c);
 }
}

//--
void saisieString(const string& mess, string& str)
{
 str="";

 while (str=="") { cout << mess;
                   cin  >> str;
 }
}

//--
void saisieSize_t(const string& mess, size_t &val)
{
 int ok = 0;
 
 while (ok!=1) {
                 string saisie;
                 unsigned int entier;
                 saisieString(mess,saisie);
                 ok = sscanf(saisie.c_str(),"%ud",&entier);
                 val = entier;
 }
}

//--
void usage(int argc,char **argv)
{
  (void)argc;
  cerr << "Usage : " << argv[0] << " Agent , OU BIEN "
                     << argv[0] << " Message" << endl;
}

//--
int main(int argc, char **argv)
{
 if (argc!=2) { usage(argc,argv); return 1; }

 string argv1 = argv[1];
 if (argv1 != "Agent" && argv1 != "Message") { usage(argc,argv); return 1; }


 string className;
 string CLASSNAME;
 string varName;
 string decName;
 string decName1;
 string decName2;
 string decNames12;

 size_t       nbBaseClasses;
 vector<string> baseClasses;

 saisieString("Nom de la classe ? ",className);
 transformeMajuscules(className,CLASSNAME);
 petitNom(className,varName);

 decName    = "const "+className+"& "+varName;
 decName1   =  decName+"1";
 decName2   =  decName+"2";
 decNames12 =  decName1+", "+decName2;
 
 saisieSize_t("Nombre de classes dont la classe "+className+" herite ? ",
              nbBaseClasses);

 for(size_t i=0; i<nbBaseClasses; i++)
 {
  string aBaseClasse;
  string ieme = " eme ";
  if (i==0) ieme = " ere ";

  ostringstream iPlus1ToString; iPlus1ToString << (i+1);

  saisieString("Nom de la " +
               iPlus1ToString.str() + ieme +
               "classe dont la classe " +
               className +
               " herite ? ",
               aBaseClasse);

  baseClasses.push_back(aBaseClasse);
 }

/*
 size_t       nbAttributPrivate;
 vector<string> attributTypesPrivate;
 vector<string> attributNamesPrivate;

 saisieSize_t("Nombre d'attributs \"private\" de la classe "+className+" ? ",
              nbAttributPrivate);

 for(size_t i=0; i<nbAttributPrivate; i++)
 {
  string attributTprivate;
  string attributNprivate;
  string ieme = " eme ";
  if (i==0) ieme = " er ";

  ostringstream iPlus1ToString; iPlus1ToString << (i+1);

  saisieString("Type du " +
                iPlus1ToString.str() + ieme +
               "attribut \"private\" ? ",
               attributTprivate);
  saisieString("Nom du " +
                iPlus1ToString.str() + ieme +
               "attribut \"private\" ? ",
               attributNprivate);

  attributTypesPrivate.push_back(attributTprivate);
  attributNamesPrivate.push_back(attributNprivate);
 }

 cout << "Attributs private:" << endl;
 for(size_t i=0;i<attributTypesPrivate.size();i++)
 {
  cout << attributTypesPrivate[i] <<" "<< attributNamesPrivate[i] << endl;
 }

 size_t       nbAttributProtected;
 vector<string> attributTypesProtected;
 vector<string> attributNamesProtected;

 saisieSize_t("Nombre d'attributs \"protected\" de la classe "+className+" ? ",
              nbAttributProtected);

 for(size_t i=0; i<nbAttributProtected; i++)
 {
  string attributTprotected;
  string attributNprotected;
  string ieme = " eme ";
  if (i==0) ieme = " er ";

  ostringstream iPlus1ToString; iPlus1ToString << (i+1);

  saisieString("Type du " +
                iPlus1ToString.str() + ieme +
               "attribut \"protected\" ? ",
               attributTprotected);
  saisieString("Nom du " +
                iPlus1ToString.str() + ieme +
               "attribut \"protected\" ? ",
               attributNprotected);

  attributTypesProtected.push_back(attributTprotected);
  attributNamesProtected.push_back(attributNprotected);
 }

 cout << "Attributs protected:" << endl;
 for(size_t i=0;i<attributTypesProtected.size();i++)
 {
  cout << attributTypesProtected[i] <<" " <<attributNamesProtected[i] << endl;
 }
*/

 // Ecriture du .h
 ///////////////////

 ofstream oh((className+".h").c_str());
 if (oh.fail()) { cerr << "Erreur ouverture fichier "
                       << className+".h" << endl;
                  return 1;
 }

 oh << "#ifndef _"+CLASSNAME+"_H_" << endl;
 oh << "#define _"+CLASSNAME+"_H_" << endl;
 oh << endl;
 oh << "#include <iostream>" << endl;
 oh << endl;
 oh << "#include \"MAS.h\"" << endl;
 if (baseClasses.size()!=0) oh << endl;
 for(size_t i=0;i<baseClasses.size();i++)
 {
  oh << "#include \""+baseClasses[i]+".h\"" << endl;
 }
 oh << endl;
 oh << "using namespace std;" << endl;
 oh << endl;

/*
 // #define NEW_className(...) new className(__VA_ARGS__)

 oh << "#define NEW_"+className+"(...) new "+className+"(__VA_ARGS__)" << endl;
 oh << endl;
*/

 oh << "class " << className;
 if (baseClasses.size()!=0) oh << " : ";
 for(size_t i=0;i<baseClasses.size();i++)
 {
  if (i!=0)
  {
   size_t nbBlancs = ("class "+className+" : ").length(); 
   for(size_t i=0;i<nbBlancs;i++) oh << ' ';
  }
  oh << "public "+baseClasses[i];
  if (i!=baseClasses.size()-1) oh << "," << endl;
 }

 oh << endl;
 oh << "{" << endl;
 oh << "  DEFCLASS("+className+")"                                  << endl;
 oh << endl;
 oh << "   friend ostream& operator<<(ostream& os, "+decName+");"   << endl;
 oh << endl;
 oh << " public :" << endl;
 oh << endl;
 oh << "   // Allocateurs/Desallocateurs" << endl;
 oh << endl;
 oh << "            "+className+"( /* ### */ );"                 << endl;
 oh << "            "+className+"("+decName+");"                 << endl;
 oh << "            "+className+"& operator=("+decName+");"      << endl;
 oh << "   virtual ~"+className+"(void);"                        << endl;
 oh << endl;

 if (argv1=="Agent")
 {
  oh << "   virtual  void live(double dt);" << endl;
  oh << endl;
 }

 oh << "   // Comparaisons" << endl;
 oh << endl;
 oh << "   friend  bool operator==("+decNames12+");"            << endl;
 oh << "   friend  bool operator!=("+decNames12+");"            << endl;
 oh << endl;
 oh << "   // Inspecteurs/modificateurs" << endl;
 oh << endl;

 oh << " protected :" << endl;
 oh << endl;
 oh << "   // Methodes a appeler par une classe derivee" << endl;
 oh << endl;
 oh << "   // display: a appeler dans une classe derivee"
    << "      // display est une"                        << endl; 
 oh << "   virtual void display(ostream& os) const;"
    << "           // methode appelee"                   << endl;
 oh << "                                           "
    << "           // dans operator<<"                   << endl;
 oh << endl;
 oh << "   // isEqualTo: a appeler dans une classe derivee (dans operator==)";
 oh << endl;
 oh << "           bool isEqualTo("+decName+") const;"   << endl;

 oh << endl;
 oh << " protected :" << endl;
 oh << endl;
 oh << "             // ###" << endl;
 oh << endl;
 oh << " private :" << endl;
 oh << endl;
 oh << "             // ###" << endl;
 oh << endl;
 oh << " private :" << endl;
 oh << endl;
 oh << "   // Methodes privees d'allocation/desallocation" << endl;
 oh << endl;
 oh << "   void _copy("+decName+");" << endl;
 oh << "   void _destroy(void);"     << endl; 
 oh << endl;
 oh << "};" << endl;
 oh << endl;
 oh << "#endif // _"+CLASSNAME+"_H_" << endl;

 oh.close();

 // Ecriture du .cpp
 ///////////////////

 ofstream oc((className+".cpp").c_str());
 if (oc.fail()) { cerr << "Erreur ouverture fichier "
                       << className+".cpp" << endl;
                  return 1;
 }

 // L' include
 /////////////

 oc << "#include \""+className+".h\"" << endl;
 oc << endl;

 // Le constructeur
 //////////////////
 oc << "//--" << endl;
 oc << className+"::"+className+"( /* ### */ )";
 if (baseClasses.size()!=0) oc << " : ";
 for(size_t i=0;i<baseClasses.size();i++)
 {
  if (i!=0)
  {
   size_t nbBlancs = (className+"::"+className+
                      "( /* ### */ )"+" : ").length(); 
   for(size_t i=0;i<nbBlancs;i++) oc << ' ';
  }
  if (baseClasses[i]=="Agent") oc << "Agent()";
  else if (baseClasses[i]=="Message") oc << "Message()";
       else oc << baseClasses[i]+"( /* ### */ )";
  if (i!=baseClasses.size()-1) oc << "," << endl;
 }
 oc << endl;
 oc << "{" << endl;
 if (argv1=="Agent") oc << " newAgent();" << endl;
 else                oc << " newMessage();" << endl;
 oc << "}" << endl;
 oc << endl;

 // Le constructeur par recopie
 //////////////////////////////

 oc << "//--" << endl;
 oc << className << "::" << className << "("+decName+")";
 if (baseClasses.size()!=0) oc << " : ";
 for(size_t i=0;i<baseClasses.size();i++)
 {
  if (i!=0)
  {
   size_t nbBlancs = (className+"::"+className+"("+decName+")"+" : ").length(); 
   for(size_t i=0;i<nbBlancs;i++) oc << ' ';
  }
  if (baseClasses[i]=="Agent") oc << "Agent("+varName+")";
  else if (baseClasses[i]=="Message") oc << "Message("+varName+")";
       else oc << baseClasses[i] << "("+varName+")";
  if (i!=baseClasses.size()-1) oc << "," << endl;
 }
 oc << endl;
 oc << "{" << endl;
 if (argv1=="Agent") oc << " newAgent();" << endl;
 else                oc << " newMessage();" << endl;
 oc << " _copy("+varName+");" << endl;
 oc << "}" << endl;
 oc << endl;

 // L'operator=
 //////////////

 oc << "//--" << endl;
 oc << className << "& " << className << "::operator=("+decName+")" << endl;
 oc << "{" << endl;
 oc << " if (this != &"+varName+")" << endl;
 oc << " {" << endl;
 for(size_t i=0;i<baseClasses.size();i++)
 {
 oc << "  "+baseClasses[i]+"::operator=("+varName+");" << endl;
 }
 oc << "  _destroy();" << endl;
 oc << "  _copy("+varName+");" << endl;
 oc << " }" << endl;
 oc << " return *this;" << endl;
 oc << "}" << endl;
 oc << endl;
 

 // Le destructeur
 //////////////////

 oc << "//--" << endl;
 oc << className+"::~"+className+"(void)" << endl;
 oc << "{" << endl;
 oc << " _destroy();" << endl;
 oc << "}" << endl;
 oc << endl;

 // live si Agent
 ////////////////

 if (argv1=="Agent")
 {
  oc << "//--" << endl;
  oc << "void " << className << "::live(double dt)" << endl;
  oc << "{" << endl;
  oc << " (void)dt; // Pour eviter un warning si pas utilise" << endl;
  oc << endl;
  oc << " // ### : \"Comportement\" d'un Agent de la classe "+className<< endl; 
  oc << "}" << endl;
  oc << endl;
 }

 // operator==
 /////////////

 oc << "//--" << endl;
 oc << "bool operator==("+decNames12+")" <<endl;
 oc << "{" << endl;
 oc << " return "+varName+"1.isEqualTo("+varName+"2);" << endl;
 oc << "}" << endl;
 oc << endl;

 // operator!=
 /////////////

 oc << "//--" << endl;
 oc << "bool operator!=("+decNames12+")" <<endl;
 oc << "{" << endl;
 oc << " return !("+varName+"1=="+varName+"2);" << endl;
 oc << "}" << endl;
 oc << endl;

 // operator<<
 /////////////

 oc << "//--" << endl;
 oc << "ostream& operator<<(ostream& os, "+decName+")"<<endl;
 oc << "{" << endl;
 oc << " "+varName+".display(os);" << endl;
 oc << " return os;" << endl;
 oc << "}" << endl;
 oc << endl;

 // display
 ///////////

 oc << "//--" << endl;
 oc << "void "+className+"::display(ostream& os) const" << endl;
 oc << "{" << endl;
 oc << " (void)os; // Pour eviter un warning si pas utilise" << endl << endl;
 for(size_t i=0;i<baseClasses.size();i++)
 {
 oc << " "+baseClasses[i]+"::display(os);" << endl;
 }
 if (baseClasses.size()!=0) oc << endl;
 oc << " // ### : Affichage des attributs de la classe " <<
                className << endl;
 oc << " // Exemple : os << _at;" << endl;
 oc << "}" << endl;
 oc << endl;

 // isEqualTo
 ////////////

 oc << "//--" << endl;
 oc << "bool "+className+"::isEqualTo("+decName+") const"<<endl;
 oc << "{" << endl;
 oc << " (void)"+varName <<"; // Pour eviter un warning si pas utilise"
                         << endl
                         << endl;
 oc << " // ### : Test des attributs de la classe " <<
                className << endl;
 oc << " // Exemple : if (_at != "+varName+"._at) return false;" << endl;
 if (baseClasses.size()!=0) oc << endl;
 for(size_t i=0;i<baseClasses.size();i++)
 {
 oc << " if (!("<<baseClasses[i]+"::isEqualTo("+varName+"))) return false;"
    << endl;
 }
 oc << " return true;" << endl;
 oc << "}" << endl;
 oc << endl;

 // _copy
 /////////

 oc << "//--" << endl;
 oc << "void "+className+"::_copy("+decName+")"<< endl;
 oc << "{" << endl;
 oc << " (void)" << varName <<"; // Pour eviter un warning si pas utilise"
                            << endl
                            << endl;
 oc << " // ### : Affectation des attributs de la classe "+className << endl;
 oc << " // Exemple : _at = "+varName+"._at;" << endl;
 oc << "}" << endl;
 oc << endl;

 // _destroy
 ////////////

 oc << "//--" << endl;
 oc << "void "+className+"::_destroy(void)" << endl;
 oc << "{" << endl;
 oc << " // ### : Destruction des attributs de la classe "+className << endl;
 oc << " // Exemple : delete _at;" << endl;
 oc << "}" << endl;
 oc << endl;

 oc.close();

 return 0;
}
