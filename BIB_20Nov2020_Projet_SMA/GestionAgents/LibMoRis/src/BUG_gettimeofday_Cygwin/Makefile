###########################################################################

#----- lib a utiliser
DIRAGENTS=../../../../GestionAgents
INCAGENTS=${DIRAGENTS}/LibMoRis/include
LIBAGENTS=${DIRAGENTS}/LibMoRis/lib
AGENTSLIBNAME=Agents

#-------------  Appli  --------------

TARGET=ex

all: ${TARGET}

#---- les fichiers sources ----

FILESex=ex.cpp

###########################################################################

#---- les variables ----

CC=g++
INC=-I${INCAGENTS}
CCFLAGS=-W -Wall -Werror -pedantic ${INC}
LIBFLAGS=

LDFLAGS=\
-L${LIBAGENTS} -l${AGENTSLIBNAME} \
${LIBFLAGS}

###########################################################################

#---- les fichiers objets ----

OBJECTSex=${FILESex:.cpp=.o}

#---- librairies utilisees ----

DEPLIBS=${LIBAGENTS}/lib${AGENTSLIBNAME}.a

#--------- realisation de l'executable (edition de liens) ----

ex: ${OBJECTSex} ${DEPLIBS}
	${CC} ${OBJECTSex} ${LDFLAGS} -o ex

#--------- regle -------------------

ex.o: ex.cpp ${INCAGENTS}/MAS.h
	${CC} ${CCFLAGS} -c ex.cpp

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
