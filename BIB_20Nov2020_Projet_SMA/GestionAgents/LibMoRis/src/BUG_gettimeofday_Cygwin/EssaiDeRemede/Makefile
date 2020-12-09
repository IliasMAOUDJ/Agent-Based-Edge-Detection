###########################################################################

#----- lib a utiliser
DIRAGENTS=../../../../../GestionAgents
INCAGENTS=${DIRAGENTS}/LibMoRis/include
LIBAGENTS=${DIRAGENTS}/LibMoRis/lib
AGENTSLIBNAME=Agents

#-------------  Appli  --------------

TARGET=remede

all: ${TARGET}

#---- les fichiers sources ----

FILESremede=remede.cpp

###########################################################################

#---- les variables ----

CC=g++
INC=-I${INCAGENTS}
CCFLAGS=-W -Wall -Werror ${INC} # -pedantic
LIBFLAGS=

LDFLAGS=\
-L${LIBAGENTS} -l${AGENTSLIBNAME} \
${LIBFLAGS}

###########################################################################

#---- les fichiers objets ----

OBJECTSremede=${FILESremede:.cpp=.o}

#---- librairies utilisees ----

DEPLIBS=${LIBAGENTS}/lib${AGENTSLIBNAME}.a

#------------------------------------

remede: ${OBJECTSremede} $(DEPLIB)
	${CC} ${OBJECTSremede} ${LDFLAGS} -o remede

#------------------------------------

remede.o: remede.cpp ${INCAGENTS}/MAS.h
	${CC} ${CCFLAGS} -c remede.cpp

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

CLEANING=rm -f *.o core a.out *.exe *.exe.stackdump ${TARGET}

clear:
	${CLEANING}
clean:
	${CLEANING}
