# Copyright (c) John Allen Whitley, 2022, BSD 3-Clause

IDIR=includes
CC=gcc
CCFLAGS=-I$(IDIR) -ggdb -Wall -pedantic -fdiagnostics-color=always
DEPS=monster.h
OBJ=main.o monster.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CCFLAGS)

battle: $(OBJ)
	$(CC) -o $@ $^ $(CCFLAGS) 

clean:
	rm *.o battle
