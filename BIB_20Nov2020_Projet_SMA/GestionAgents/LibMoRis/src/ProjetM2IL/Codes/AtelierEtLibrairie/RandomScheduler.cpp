#include "Agent.h"
#include "RandomScheduler.h"

// Constructing
RandomScheduler::RandomScheduler() {
	srand(time(NULL));
}

// Scheduling
set<Agent *, Scheduler::classcomp> RandomScheduler::schedule(set<Agent *> agents) {
	int r;
	set<Agent *, Scheduler::classcomp> scheduledAgents;
	set<Agent *>::iterator itToSchedule;
	
	while (agents.size() > 0) {
		r = rand()%(agents.size());
		itToSchedule = agents.begin();
		for (int i=0; i<r; i++) {
			itToSchedule++;
		}
		
		scheduledAgents.insert(scheduledAgents.begin(), (* itToSchedule));
		agents.erase(itToSchedule);
	} 
	return (scheduledAgents);
}
