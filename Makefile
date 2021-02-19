CC=gcc
CFLAGS= -O3 -march=native


all: main

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $^

main: Stud.o jim_corrected.c
	$(CC) $(CFLAGS) -o $@ $^

clean: #make clean to run
	rm *.o *.h.gch main


