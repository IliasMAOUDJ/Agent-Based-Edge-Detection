#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include <iostream>
#include <set>

using namespace std;

class Agent;

class Scheduler {
	
 public:
	struct classcomp {
		bool operator() (const Agent *, const Agent *) const {
			return (true);
		}
	};
	
	// Cycling
	void cycle(set<Agent *>, int nb_cycles);
	
	// Living
	void live(set<Agent *, classcomp>);
	
	// Scheduling
	virtual set<Agent *, classcomp> schedule(set<Agent *>) = 0;
};

#endif
