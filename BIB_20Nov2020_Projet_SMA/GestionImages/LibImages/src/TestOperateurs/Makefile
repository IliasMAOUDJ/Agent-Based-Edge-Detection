##############################################################################

#----- lib a utiliser
DIRIMAGES=../../../../GestionImages
INCIMAGES=${DIRIMAGES}/LibImages/include
LIBIMAGES=${DIRIMAGES}/LibImages/lib
IMAGESLIBNAME=Images
#-----

#---- les programmes a obtenir (cibles principales) ----

TARGET=progX

all : ${TARGET}

#---- les fichiers sources ----

FILESprogX=progX.cpp

##############################################################################

#---- les variables ----

CC=g++
INC=-I${INCIMAGES}
CCFLAGS=-W -Wall -pedantic -Werror ${INC}
LIBFLAGS=-lm -L/usr/X11R6/lib -lX11 -lXt

LDFLAGS=\
-L${LIBIMAGES} -l${IMAGESLIBNAME} \
${LIBFLAGS}

##############################################################################

#---- les fichiers objets ----

OBJECTSprogX=${FILESprogX:.cpp=.o}

#---- librairies utilisees ----

DEPLIBS=${LIBIMAGES}/lib${IMAGESLIBNAME}.a

#---- realisation des l'executables (edition de liens) ----

progX: ${OBJECTSprogX} ${DEPLIBS} ${INCIMAGES}/LibImages.h
	${CC} ${OBJECTSprogX} ${LDFLAGS} -o progX

#------ regles -----------------------

#---- la regle par defaut (.c/cpp --> .o) ----

.c.o :
	@echo
	${CC} -o $*.o -c ${CCFLAGS} $<
	@echo

.cpp.o :
	@echo
	${CC} -o $*.o -c ${CCFLAGS} $<
	@echo

#---- nettoyage ----

CLEANING=rm -f *.o a.out *.exe core *.exe.stackdump ${TARGET}

clean :
	${CLEANING}

clear :
	${CLEANING}

##############################################################################
