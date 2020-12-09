##############################################################################

WORKDIR=..
LIBDIR=${WORKDIR}/LibObject2D/lib
INCDIR=${WORKDIR}/LibObject2D/include

#---- le programme a obtenir (cible principale) ----
TARGET=run

all : ${TARGET}

#---- les fichiers sources ----

FILESgraphTest=run.cpp Agent.cpp TypeTreeNode.cpp Scheduler.cpp Message.cpp RandomScheduler.cpp Atelier.cpp Or2DEntity.cpp Door.cpp Input.cpp Output.cpp Holon.cpp HolonMessage.cpp Utilities.cpp Piece.cpp Machine.cpp MachineA.cpp MachineB.cpp MachineC.cpp ARequest.cpp BRequest.cpp CRequest.cpp YesMessage.cpp ConfirmMessage.cpp NoMessage.cpp NearMessage.cpp AckMessage.cpp ReadyMessage.cpp EndMessage.cpp

##############################################################################

#---- les variables ----

CC=g++
CCFLAGS=-W -Wall -pedantic -Werror -I${INCDIR} -DX_GUI
DEPFLAGS=-MM
LIBFLAGS=-lm -L/usr/X11R6/lib -lX11 -lXt
LDFLAGS=-L${LIBDIR} -lgraphics2D $(LIBFLAGS)

##############################################################################

#---- les fichiers objets ----

OBJECTSgraphTest=${FILESgraphTest:.c=.o}

#---- la regle par defaut (.cpp --> .o) ----

.cpp.o :
	@echo
	${CC} -o $*.o -c ${CCFLAGS} $<
	@echo

#---- realisation de l'executable (edition de liens) ----

run : ${OBJECTSgraphTest} ${LIBDIR}/libgraphics2D.a
	@echo
	${CC} -o run ${CCFLAGS} ${OBJECTSgraphTest} ${LDFLAGS}
	@echo

#---- nettoyage ----

clean :
	rm -f ${TARGET} *.o a.out *.exe core *.exe.stackdump *~

##############################################################################

