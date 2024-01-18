#This is a comment
P1=scan_omp
P2=scan
# P3=hello_omp
#INC="./inc"
#FLAGS=-I$(INC)
OMPFLAG=-fopenmp
CC=gcc
CFLAGS = -g -Wall

all: scan_omp scan $(P1) $(P2) $(P3)

$(P1): $(P1).c
	$(CC) $(CFLAGS) $(OMPFLAG) $(P1).c -o $(P1)
$(P2): $(P2).c
	$(CC) $(CFLAGS) $(OMPFLAG) $(P2).c -o $(P2)
$(P3): $(P3).c
	$(CC) $(CFLAGS) $(OMPFLAG) $(P3).c -o $(P3)


clean:
	rm -vf $(P1) $(P2) $(P3)

# output: scan.o omp_scan.o
# 	gcc scan.o omp_scan.o -o output

# scan.o: scan.c
# 	gcc -c scan.c

# omp_scan.o: omp_scan.c
# 	gcc -c -fopenmp omp_scan.c 
