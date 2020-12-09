#ifndef _MACHINE_H
#define _MACHINE_H

#include <iostream>

#include "Holon.h"

class Atelier;
class TypeTreeNode;

class Machine: public Holon {
 private:
  static TypeTreeNode * machinetreenode;
  
 protected:
  bool _hs;

  int _timeout;
  int _currentState;
  int _workingTime;

  Agent * _currentPiece;
  string _currentPieceName;

  bool _isSensible;

 public:
  // Accessing
  static set<Agent *> getAgents();
  static TypeTreeNode * getTypetreenode();
  virtual string getType();
  
  // Constructing
  Machine();
  static void create(int);
  static void create(Atelier* atelier);

  // Destructing
  ~Machine();
  
  // Erasing
  static void eraseAgents();

  // Events handling
  void keyPress(string key);
  void event_F1();       // machine state
  void event_F2();       // on/off
  void event_H();        // help

  // Handling Messages
  virtual void setSensible();
  virtual void unsetSensible();

  // Initializing
  void initLocation(Atelier* atelier, double& x, double& y);

  // Living
  void changeShape(int i);
  virtual void live();
  void on();
  void off();
  void workOn();

  int init();
  virtual int choice();
  int confirm();
  int transit();
  int ready();
  int busy();

  // Testing
  virtual bool isA(string);
};

#endif
