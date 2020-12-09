#ifndef _MACHINEC_H
#define _MACHINEC_H

#include <iostream>

#include "Machine.h"

class MachineC: public Machine {
 private:
  static TypeTreeNode * machinectreenode;
  
 public:
  // Accessing
  static set<Agent *> getAgents();
  static TypeTreeNode * getTypetreenode();
  virtual string getType();
  
  // Constructing
  MachineC();
  static void create(int);
  static void create(Atelier* atelier);
  
  // Destructing
  ~MachineC();
  
  // Erasing
  static void eraseAgents();
  
  // Handling Messages
  void setSensible();
  void unsetSensible();
  
  // Living
  int choice();
  virtual void live();

  // Testing
  virtual bool isA(string);
};

#endif
