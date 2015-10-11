CC=gcc
BASE=webserver
SRC=$(BASE)/src/
INCLUDE=$(BASE)/include/
PROG=webserver_main

all:
	@echo "make [compile|clean]"

compile:
	$(CC) -Wall -o $(PROG) -I$(INCLUDE) $(SRC)*.c


clean:
	rm $(PROG) $(SRC)all.c
