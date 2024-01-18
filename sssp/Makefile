# INC="./inc"
# FLAGS=-I$(INC)
P1= sssp_omp
P2= sssp
P3 = sssp_mpi
OMPFLAGS=-fopenmp
CFLAGS = -g -Wall
CC=gcc
MPICC=mpicc
CPP=g++
	
#SOURCES=$(wildcard *.c)
# OBJECTS=$(patsubst %.c, %, $(wildcard *.c))


all: sssp_omp sssp  $(P1) $(P2)
# 	@echo 'objects are "$(P3)"'

$(P1): $(P1).c
	$(CC) $(CFLAGS) $(OMPFLAGS) $(P1).c -o $(P1)
$(P2): $(P2).c
	$(CC) $(CFLAGS) $(P2).c -o $(P2)
# $(P3): $(P3).c
# 	$(MPICC) $(CFLAGS) $(P3) -o $(P3)


clean:
	rm -vf $(P1) $(P2) 
