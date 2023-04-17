
.phony: all clean

all: bin/person.exe

bin:
	mkdir bin
obj:
	mkdir obj

obj/person.o: person.c
	gcc -Wall -c -O3 -DNDEBUG -o obj/person.o person.c

bin/person.exe: obj/person.o
	gcc -o bin/person.exe obj/person.o

