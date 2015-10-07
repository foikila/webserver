CC=gcc
BASE=webserver
#OBJECTDIR=object
#ALL_SRCS:=$(BASE)/src/$(wildcard *\.c$)
SRC=$(BASE)/src/
INCLUDE=$(BASE)/include/
#ASM=hello.s
#OBJ=hello.o
PROG=webserver_main

all:
	@echo "make [preprocess|compile|assemble|link]"


compile:
	$(CC) -Wall -o $(PROG) -I$(INCLUDE) $(SRC)*.c




#	$(CC) -o $(PROG) $(SRC)hello.c
#	$(CC) -o process $(SRC)processes.c
#	gcc -o $(PROG) -I webserver/include/ $(ALL_SRCS)

#gcc -o $(PROG) $(BASE)/src/$(SRC)

# preprocess: $(SRC)
#	$(CC) -E $(BASE)/src/$(SRC) -o $(BASE)/$(OBJDIR)/hello.i
# compile: $(ASM)
# $(ASM): $(SRC)
# $(CC) -S $(SRC) -o $(ASM)
# assemble: $(OBJ)
# $(OBJ):	$(ASM)
# 	$(CC) -c $(ASM)
# link: $(OBJ)
#	$(CC) $(OBJ) -o $(PROG)

clean:
	rm $(PROG) $(SRC)all.c
