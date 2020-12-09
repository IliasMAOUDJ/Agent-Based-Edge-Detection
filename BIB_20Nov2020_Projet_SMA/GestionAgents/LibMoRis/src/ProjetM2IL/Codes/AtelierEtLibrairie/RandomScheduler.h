#ifndef _RANDOM_SCHEDULER_H
#define _RANDOM_SCHEDULER_H

#include <iostream>
#include <set>

#include "Scheduler.h"

class Agent;

class RandomScheduler:public Scheduler {
  
 public:
	// Constructing
	RandomScheduler();

	// Scheduling
	set<Agent *, Scheduler::classcomp> schedule(set<Agent *> agents);
};

#endif
