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
    cout << "Utilisation : \"MessageClassBuilder className parentClassName\"" << endl;
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
      
    writing << "#include \"" << parentName << ".h\"" << endl << endl;
    
    writing << "class Agent;" << endl << endl;

    writing << "class " << className << ": public " << parentName << " {" << endl
	    << "private:" << endl
	    << "static TypeTreeNode * " << classLowerName << "treenode;" << endl << endl;
    
    writing << " public:" << endl
	    << "// Accessing" << endl
	    << "static set<Agent *> getSubscribers();" << endl
	    << "static TypeTreeNode * getTypetreenode();" << endl
	    << "virtual string getType();" << endl << endl;
    
    writing << "// Broadcasting" << endl
	    << "static void broadcast(Message *);" << endl << endl;

    writing << "// Constructing" << endl
	    << className << "();" << endl << endl;
    
    writing << "// Destructing" << endl
	    << "~" << className << "();" << endl << endl;
    
    writing << "// Sending" << endl
	    << "virtual void sendTo(Agent *);" << endl << endl;	  
    
    writing << "// Testing" << endl
	    << "virtual bool isA(string);" << endl << endl;
    
    writing << "};" << endl << endl;
    
    writing << "#endif" << endl << endl;
    
    writing.close();
    
    /********** Ecriture du .cpp **********/
    writing.open((className  + ".cpp").c_str());
    
    writing << "#include \"" << className << ".h\"" << endl
	    << "#include \"TypeTreeNode.h\"" << endl
	    << "#include \"Agent.h\"" << endl << endl;
    
    writing << "TypeTreeNode * " << className << "::" << classLowerName << "treenode = NULL;" << endl << endl;
    
    writing << "// Accessing" << endl;
    
    writing << "set<Agent *> " << className << "::getSubscribers() {" << endl
	    << "set<Agent *> subscribers;" << endl
	    << className << "::getTypetreenode()->getAgents(subscribers);" << endl
	    << "return subscribers;" << endl
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

    writing << "// Broadcasting" << endl;
    
    writing << "void " << className << "::broadcast(Message * message) {" << endl
	    << "set<Agent *> subscribers = " << className << "::getSubscribers();" << endl
	    << "for (set<Agent *>::iterator it = subscribers.begin(); it != subscribers.end(); it++) {" << endl
	    << "if (Agent::exist(*it)) {" << endl
	    << "message->sendTo(*it);" << endl
	    << "}" << endl
	    << "}" << endl
	    << "delete message;" << endl
	    << "}" << endl << endl;

    writing << "// Constructing" << endl;

    writing << className << "::" << className << "():" << parentName << "() {" << endl
	    << parentName << "::getTypetreenode()->addChildNode(" << className << "::getTypetreenode());" << endl
	    << "}" << endl << endl;
    
    writing << "// Destructing" << endl;
    
    writing << className << "::~" << className << "() {}" << endl << endl;
    
    writing << "// Sending" << endl;
    
    writing << "void " << className << "::sendTo(Agent * receiver) {" << endl
	    << className << " * newMessage = new " << className << "(*this);" << endl
	    << "receiver->saveMessage(newMessage);" << endl
	    << "}" << endl << endl;
    
    writing << "// Testing" << endl;
    
    writing << "bool " << className << "::isA(string type) {" << endl
	    << "return (" << className << "::getTypetreenode()->isA(type));" << endl
	    << "}" << endl << endl;
    
    writing.close(); 
  }
  
  return (0);
}
