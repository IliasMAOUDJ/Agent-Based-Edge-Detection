##############################################################################

#----- lib a obtenir
DIRIMAGES=../../../GestionImages
INCIMAGES=${DIRIMAGES}/LibImages/include
LIBIMAGES=${DIRIMAGES}/LibImages/lib
IMAGESLIBNAME=Images
#-----

CC=g++

INCFLAGS_STD=-I${INCIMAGES}
INCFLAGS_APPLE=-I/usr/X11/include
INCFLAGS=$(INCFLAGS_STD) $(INCFLAGS_APPLE)

WOFF=-W -Wall -Werror # Avant APPLE, -pedantic en plus..!

CCFLAGS=${WOFF} ${INCFLAGS}

#-----

#---- la bibliotheque a obtenir

LIB=${LIBIMAGES}/lib${IMAGESLIBNAME}.a

TARGET=${LIB}

all: ${TARGET}

#------------------------------------

FILESlibImages=Image.cpp \
	ImageRVB.cpp     \
	ESImages.cpp     \
	xAffichage.cpp

OBJECTSlibImages=${FILESlibImages:.cpp=.o}

#------------------------------------

${LIB} : ${OBJECTSlibImages}
	ar -rs ${LIB} ${OBJECTSlibImages}

#------------------------------------

Image.o : ${INCIMAGES}/LibImages.h Image.cpp
	${CC} ${CCFLAGS} -c Image.cpp

ImageRVB.o : ${INCIMAGES}/LibImages.h ImageRVB.cpp
	${CC} ${CCFLAGS} -c ImageRVB.cpp

ESImages.o : ${INCIMAGES}/LibImages.h ESImages.cpp
	${CC} ${CCFLAGS} -c ESImages.cpp

xAffichage.o : ${INCIMAGES}/LibImages.h xAffichage.cpp
	${CC} ${CCFLAGS} -c xAffichage.cpp

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
