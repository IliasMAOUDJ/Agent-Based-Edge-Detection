#ifndef _MACHINEA_H
#define _MACHINEA_H

#include <iostream>

#include "Machine.h"

class MachineA: public Machine {
 private:
  static TypeTreeNode * machineatreenode;
  
 public:
  // Accessing
  static set<Agent *> getAgents();
  static TypeTreeNode * getTypetreenode();
  virtual string getType();
  
  // Constructing
  MachineA();
  static void create(int);
  static void create(Atelier* atelier);
  
  // Destructing
  ~MachineA();
  
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
