all: genere

CCFLAGS=-g -pedantic -W -Wall -Werror

clear:
	rm -f a.out *.o core *.exe *.exe.stackdump genere
clean:
	rm -f a.out *.o core *.exe *.exe.stackdump genere

genere: genere.o
	g++ genere.o -o genere

.cpp.o :
	g++ $(CCFLAGS) -c $<
