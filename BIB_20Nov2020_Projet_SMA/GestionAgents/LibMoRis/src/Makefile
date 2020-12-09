##############################################################################

#----- lib a obtenir
DIRAGENTS=../../../GestionAgents
INCAGENTS=${DIRAGENTS}/LibMoRis/include
LIBAGENTS=${DIRAGENTS}/LibMoRis/lib
AGENTSLIBNAME=Agents
#-----

CC=g++
INC=-I${INCAGENTS}
CCFLAGS=-W -Wall -Werror -pedantic ${INC}

#-----

#---- la bibliotheque a obtenir

LIB=${LIBAGENTS}/lib${AGENTSLIBNAME}.a

TARGET=${LIB}

all: ${TARGET}

#------------------------------------

FILESlibAgents=UtilAgent.cpp \
	NameService.cpp \
	Scheduler.cpp   \
	Agent.cpp       \
	Message.cpp

OBJECTSlibAgents=${FILESlibAgents:.cpp=.o}

#------------------------------------

${LIB} : ${OBJECTSlibAgents}
	ar -rs ${LIB} ${OBJECTSlibAgents}

#------------------------------------

UtilAgent.o : ${INCAGENTS}/MAS.h UtilAgent.cpp
	${CC} ${CCFLAGS} -c UtilAgent.cpp

NameService.o : ${INCAGENTS}/MAS.h NameService.cpp
	${CC} ${CCFLAGS} -c NameService.cpp

Scheduler.o : ${INCAGENTS}/MAS.h ${INCAGENTS}/Scheduler.h Scheduler.cpp
	${CC} ${CCFLAGS} -c Scheduler.cpp

Agent.o : ${INCAGENTS}/MAS.h Agent.cpp
	${CC} ${CCFLAGS} -c Agent.cpp

Message.o : ${INCAGENTS}/MAS.h Message.cpp
	${CC} ${CCFLAGS} -c Message.cpp

#---- la regle par defaut (.c/cpp --> .o) ----

.c.o :
	@echo
	${CC} -o $*.o -c ${CCFLAGS} $<
	@echo

.cpp.o :
	@echo
	${CC} -o $*.o -c ${CCFLAGS} $<
	@echo

#------------- clean - clear --------

CLEANING=rm -f *.o ${LIB}

clear :
	${CLEANING}
clean :
	${CLEANING}
