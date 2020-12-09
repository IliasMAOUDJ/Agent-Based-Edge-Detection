#include "Agent.h"
#include "Scheduler.h"

// Cycling
void Scheduler::cycle(set<Agent *> agents, int nb_cycles) {
	for (int i=0; i<nb_cycles; i++) {
		this->live(this->schedule(agents));
	}
}

// Living
void Scheduler::live(set<Agent *, classcomp> agents) {
	for (set<Agent *>::iterator it = agents.begin(); it != agents.end(); it++) {
		(*it)->live();
	}
}
