#ifndef _PIECE_H
#define _PIECE_H

#include <iostream>

#include "Holon.h"

class Atelier;
class TypeTreeNode;

class Piece: public Holon {
 private:
	static TypeTreeNode * piecetreenode;

 protected:
	queue<string> _gamme;
	int _currentState;
	int _timeout;

	Holon * _currentMachine;
	string _currentMachineName;
	
 public:
	// Accessing
	static set<Agent *> getAgents();
	static TypeTreeNode * getTypetreenode();
	virtual string getType();
	
	// Constructing
	Piece();
	static void create(int);
	static void create(Atelier* atelier);
	
	// Destructing
	~Piece();
	
	// Erasing
	static void eraseAgents();

	// Initializing
	void initGamme();

	// Living
	void changeShape(int i);
	virtual void live();
		
	int  init();
	int  choice();
	int  confirm();
	int  transit();
	int  ready();
	int  busy();
	int  end();
	void exit();

	// Testing
	virtual bool isA(string);
};

#endif
