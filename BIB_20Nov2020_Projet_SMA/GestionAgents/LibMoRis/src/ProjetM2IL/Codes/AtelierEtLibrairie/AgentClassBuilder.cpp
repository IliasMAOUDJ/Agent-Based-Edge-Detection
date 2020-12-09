#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct my_tolower { 
  char operator()(char c) const {
    return tolower(static_cast<unsigned char>(c));
  }
};

struct my_toupper { 
  char operator()(char c) const {
    return toupper(static_cast<unsigned char>(c));
  } 
};

int main(int argc, char** argv) {
  if (argc != 3) {
    cout << "Utilisation : \"AgentClassBuilder className parentClassName\"" << endl;
  }
  else {
    ofstream writing;
    string classLowerName = argv[1];
    transform(classLowerName.begin(), classLowerName.end(), classLowerName.begin(), my_tolower());
    
    string className = classLowerName;
    string classUpperName = classLowerName;
    string parentName = argv[2];
    className[0] = toupper(className[0]);
    transform(classUpperName.begin(), classUpperName.end(), classUpperName.begin(), my_toupper());
    transform(parentName.begin(), parentName.end(), parentName.begin(), my_tolower());
    parentName[0] = toupper(parentName[0]);
    
    
    /*********** Ecriture du .h ***********/
    writing.open((className + ".h").c_str());
    
    writing << "#ifndef _" << classUpperName << "_H" << endl
	    << "#define _" << classUpperName << "_H" << endl << endl;
    
    writing << "#include <iostream>" << endl << endl;
    
    writing << "#include \"" << parentName << ".h\"" << endl << endl;
    
    writing << "class " << className << ": public " << parentName << " {" << endl
	    << "private:" << endl
	    << "static TypeTreeNode * " << classLowerName << "treenode;" << endl << endl;
    
    writing << " public:" << endl
	    << "// Accessing" << endl
	    << "static set<Agent *> getAgents();" << endl
	    << "static TypeTreeNode * getTypetreenode();" << endl
	    << "virtual string getType();" << endl << endl;
    
    writing << "// Constructing" << endl
	    << className << "();" << endl
	    << "static void create(int);" << endl << endl;
    
    writing << "// Destructing" << endl
	    << "~" << className << "();" << endl << endl;
    
    writing << "// Erasing" << endl
	    << "static void eraseAgents();" << endl << endl;
    
    writing << "// Living" << endl
	    << "virtual void live();" << endl << endl;	  
    
    writing << "// Testing" << endl
	    << "virtual bool isA(string);" << endl << endl;
    
    writing << "};" << endl << endl;
    
    writing << "#endif" << endl << endl;
    
    writing.close();
    
    /********** Ecriture du .cpp **********/
    writing.open((className  + ".cpp").c_str());
    
    writing << "#include \"" << className << ".h\"" << endl
	    << "#include \"TypeTreeNode.h\"" << endl
	    << "#include <sstream>" << endl << endl;
    
    writing << "using namespace std;" << endl << endl;
    
    writing << "TypeTreeNode * " << className << "::" << classLowerName << "treenode = NULL;" << endl << endl;
    
    writing << "// Accessing" << endl;
    
    writing << "set<Agent *> " << className << "::getAgents() {" << endl
	    << "set<Agent *> agents;" << endl
	    << className << "::getTypetreenode()->getAgents(agents);" << endl
	    << "return agents;" << endl
	    << "}" << endl << endl;
    
    writing << "TypeTreeNode * " << className << "::getTypetreenode() {" << endl
	    << "if (!" << classLowerName << "treenode) {" << endl
	    << className << "::" << classLowerName << "treenode = new TypeTreeNode();" << endl
	    << className << "::getTypetreenode()->initialize(" << parentName << "::getTypetreenode(), \"" << className << "\");" << endl
	    << "}" << endl
	    << "return " << className << "::" << classLowerName << "treenode;" << endl
	    << "}" << endl << endl;
    
    writing << "string " << className << "::getType() {" << endl
	    << "return " << className << "::getTypetreenode()->getType();" << endl
	    << "}" << endl << endl;
    
    writing << "// Constructing" << endl;

    writing << className << "::" << className << "():" << parentName << "() {" << endl
	    << parentName << "::getTypetreenode()->addChildNode(" << className << "::getTypetreenode());" << endl
	    << "}" << endl << endl;
    
    writing << "void " << className << "::create(int number) {" << endl
	    << "for (int i=0; i<number; i++) {" << endl
	    << className << " * " << classLowerName << " = new " << className << "();" << endl
	    << "string s;" << endl
	    << "stringstream ss;" << endl
	    << "ss << " << "\"" << className << "\" << " << className << "::getTypetreenode()->getNumber();" << endl
	    << "ss >> s;" << endl
	    << classLowerName << "->setName(s);" << endl
	    << className << "::getTypetreenode()->addAgent(" << classLowerName << ");" << endl
	    << "}" << endl
	    << "}" << endl << endl;
    
    writing << "// Destructing" << endl;
    
    writing << className << "::~" << className << "() {}" << endl << endl;
    
    writing << "// Erasing" << endl;
  
    writing << "void " << className << "::eraseAgents() {" << endl
	    << className << "::getTypetreenode()->eraseAgents();" << endl
	    << "}" << endl << endl;
    
    writing << "// Living" << endl;
    
    writing << "void " << className << "::live() {" << endl
	    << "}" << endl << endl;
    
    writing << "// Testing" << endl;
    
    writing << "bool " << className << "::isA(string type) {" << endl
	    << "return (" << className << "::getTypetreenode()->isA(type));" << endl
	    << "}" << endl << endl;
    
    writing.close(); 
  }
  
  return (0);
}
