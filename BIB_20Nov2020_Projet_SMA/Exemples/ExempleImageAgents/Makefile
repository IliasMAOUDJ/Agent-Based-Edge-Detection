###############################################################

#----- lib a utiliser
DIRIMAGES=../../GestionImages
INCIMAGES=${DIRIMAGES}/LibImages/include
LIBIMAGES=${DIRIMAGES}/LibImages/lib
IMAGESLIBNAME=Images
#----- lib a utiliser
DIRAGENTS=../../GestionAgents
INCAGENTS=${DIRAGENTS}/LibMoRis/include
LIBAGENTS=${DIRAGENTS}/LibMoRis/lib
AGENTSLIBNAME=Agents

#-------------  Appli  --------------

TARGET=progX

all: $(TARGET)

#------------ Les fichiers sources --

FILESprogX=progX.cpp \
	Edge_detection.cpp \
	EdgeFollowingAgent.cpp

###############################################################

#---- les variables ----

CC=g++

INCFLAGS_STD=-I${INCIMAGES} -I${INCAGENTS}
INCFLAGS_APPLE=-I/usr/X11/include
INCFLAGS=$(INCFLAGS_STD) $(INCFLAGS_APPLE)

LIBX11_STD=-L/usr/X11R6/lib
LIBX11_APPLE=-L/usr/X11/lib

LIBX11=$(LIBX11_STD)
LIBX11=$(LIBX11_APPLE)
LIBFLAGS=-lm $(LIBX11) -lX11 -lXt

WOFF=-W -Wall -Werror # Avant APPLE, -pedantic en plus.!
CCOPTS=-DX_GUI
OPTIMIZER=-O3 -fno-exceptions

CCFLAGS=$(INCFLAGS) $(WOFF) $(OPTIMIZER) $(CCOPTS)

LDFLAGS=\
-L${LIBIMAGES} -l${IMAGESLIBNAME} \
-L${LIBAGENTS} -l${AGENTSLIBNAME} \
${LIBFLAGS}

###############################################################

#---- les fichiers objets ----

OBJECTSprogX=${FILESprogX:.cpp=.o}

#---- librairies utilisees ----

DEPLIBS=${LIBIMAGES}/lib${IMAGESLIBNAME}.a \
${LIBAGENTS}/lib${AGENTSLIBNAME}.a

#---- realisation de l'executable (edition de liens) ----

progX: ${OBJECTSprogX} ${DEPLIBS}
	${CC} ${OBJECTSprogX} ${LDFLAGS} -o progX

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
