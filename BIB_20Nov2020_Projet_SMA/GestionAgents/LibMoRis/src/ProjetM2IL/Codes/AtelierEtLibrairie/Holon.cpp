#include "Atelier.h"
#include "Holon.h"
#include "Message.h"
#include "TypeTreeNode.h"
#include "Utilities.h"

#include <sstream>

using namespace std;

TypeTreeNode * Holon::holontreenode = NULL;

// Accessing
set<Agent *> Holon::getAgents() {
  set<Agent *> agents;
  Holon::getTypetreenode()->getAgents(agents);
  return agents;
}
TypeTreeNode * Holon::getTypetreenode() {
  if (!holontreenode) {
    holontreenode = new TypeTreeNode();
    holontreenode->initialize(Agent::getTypetreenode(), "Agent");
  }
  return holontreenode;
}
string Holon::getType() {
  return Holon::getTypetreenode()->getType();
}

// Constructing
Holon::Holon():Agent() {
  Agent::getTypetreenode()->addChildNode(Holon::getTypetreenode());
}
void Holon::create(int number) {
  for (int i=0; i<number; i++) {
    Holon * holon = new Holon();
    string s;
    stringstream ss;
    
    ss << "Holon" << Holon::getTypetreenode()->getNumber();
    ss >> s;
    holon->setName(s);
    Holon::getTypetreenode()->addAgent(holon);
  }
}
void Holon::create(Atelier* atelier, double x, double y){
  Holon * holon = new Holon();
  string s;
  stringstream ss;
  
  ss << "Holon" << Holon::getTypetreenode()->getNumber();
  ss >> s;
  holon->setName(s);
  Holon::getTypetreenode()->addAgent(holon);
  
  holon->setLocation(x,y,0);
  holon->_atelier = atelier;
  holon->attachTo(*holon->_atelier);
}

// Destructing
Holon::~Holon() {}

// Erasing
void Holon::clearMailBox() {
  while(this->getNbMessage()) getNextMessage();
}
void Holon::eraseAgents() {
  Holon::getTypetreenode()->eraseAgents();
}

// Living
void Holon::live() {
  this->_Kinematic(getTime());
}

// Moving
double Holon::distancePosition(double xP, double yP){
  double x, y;
  this->getXY(x, y);
  
  double  dx = xP - x;
  double  dy = yP - y;
  
  return sqrt(dx*dx + dy*dy);
}
double Holon::distance(Holon* holon){
  double r = -1.0;
  if (exist(holon)){
    double xh, yh;
    holon->getXY(xh, yh);
    r = distancePosition(xh,yh);
  }
  return r;
}
double Holon::towardsPosition(double xP,double yP){
  double speed = 10.0;
  
  double x, y;
  this->getXY(x, y);
  
  double dx = xP - x;
  double dy = yP - y;
  
  double r  = distancePosition(xP,yP);
  
  if(r != 0) {
    this->setTheta( cartToAngle(dx,dy) );
  
    double absdx = abs(dx);
    double absdy = abs(dy);
    if (absdx>absdy) {
      this->setLinearVelocity( speed*dx/absdx , speed*dy/absdx);
    }
    else {
      this->setLinearVelocity( speed*dx/absdy , speed*dy/absdy);
    }
  }
  return r;
}
double Holon::towards(Holon* holon){
  double r = -1.0;
  if (exist(holon)){ 
    double xh, yh;
    holon->getXY(xh, yh);
    r = towardsPosition(xh,yh);
  }
  return r;
}

// Testing
bool Holon::isA(string type) {
  return (Holon::getTypetreenode()->isA(type));
}
