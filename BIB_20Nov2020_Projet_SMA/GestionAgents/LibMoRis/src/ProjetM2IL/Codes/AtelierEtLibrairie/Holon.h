#ifndef _HOLON_H
#define _HOLON_H

#include <iostream>

#include "Agent.h"
#include "Or2DEntity.h"

class Atelier;
class TypeTreeNode;

class Holon: public Agent, public Or2DEntity {

 private:
  static TypeTreeNode * holontreenode;

 protected:
  Atelier * _atelier;
  string  _holonColor;

 public:
  // Accessing
  static set<Agent *> getAgents();
  static TypeTreeNode * getTypetreenode();
  virtual string getType();

  // Constructing
  Holon();
  static void create(int);
  static void create(Atelier *, double, double);

  // Destructing
  ~Holon();
  
  // Erasing
  void clearMailBox();
  static void eraseAgents();

  // Living
  virtual void live();
  
  // Moving
  double distancePosition(double, double);
  double distance(Holon *);
  double towardsPosition(double, double);
  double towards(Holon *);

  // Testing
  virtual bool isA(string);
};

#endif
