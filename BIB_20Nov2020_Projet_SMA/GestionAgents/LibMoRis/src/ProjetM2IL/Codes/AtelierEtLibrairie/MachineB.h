#ifndef _MACHINEB_H
#define _MACHINEB_H

#include <iostream>

#include "Machine.h"

class MachineB: public Machine {
 private:
  static TypeTreeNode * machinebtreenode;
  
 public:
  // Accessing
  static set<Agent *> getAgents();
  static TypeTreeNode * getTypetreenode();
  virtual string getType();
  
  // Constructing
  MachineB();
  static void create(int);
  static void create(Atelier * atelier);
  
  // Destructing
  ~MachineB();
  
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
