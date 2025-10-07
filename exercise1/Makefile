CC = gcc
MPICC = mpicc
CFLAGS = -O3 -Wall -std=c99
CFLAGS_OMP = -O3 -Wall -std=c99 -fopenmp
LDFLAGS = -lm
LDFLAGS_OMP = -lm -fopenmp

TARGET_SERIAL = gol_serial
TARGET_OMP = gol_omp
TARGET_MPI = gol_mpi
VALIDATE = validate
HEADERS = game_of_life.h

all: $(TARGET_SERIAL) $(TARGET_OMP) $(TARGET_MPI) $(VALIDATE)

# Serial version (no OpenMP)
$(TARGET_SERIAL): main.o game_of_life_serial.o
	$(CC) $(CFLAGS) -o $@ main.o game_of_life_serial.o $(LDFLAGS)

# OpenMP version
$(TARGET_OMP): main_omp.o game_of_life_omp.o
	$(CC) $(CFLAGS_OMP) -o $@ main_omp.o game_of_life_omp.o $(LDFLAGS_OMP)

# MPI+OpenMP version
$(TARGET_MPI): main_mpi.o game_of_life_mpi.o
	$(MPICC) $(CFLAGS_OMP) -o $@ main_mpi.o game_of_life_mpi.o $(LDFLAGS_OMP)

# Validation tool
$(VALIDATE): validate.o game_of_life_serial.o
	$(CC) $(CFLAGS) -o $@ validate.o game_of_life_serial.o $(LDFLAGS)

# Object files - serial
main.o: main.c $(HEADERS)
	$(CC) $(CFLAGS) -c main.c -o main.o

game_of_life_serial.o: game_of_life_serial.c $(HEADERS)
	$(CC) $(CFLAGS) -c game_of_life_serial.c -o game_of_life_serial.o

validate.o: validate.c $(HEADERS)
	$(CC) $(CFLAGS) -c validate.c -o validate.o

# Object files - OpenMP
main_omp.o: main_omp.c $(HEADERS)
	$(CC) $(CFLAGS_OMP) -c main_omp.c -o main_omp.o

game_of_life_omp.o: game_of_life_serial.c $(HEADERS)
	$(CC) $(CFLAGS_OMP) -c game_of_life_serial.c -o game_of_life_omp.o

# Object files - MPI
main_mpi.o: main_mpi.c $(HEADERS)
	$(MPICC) $(CFLAGS_OMP) -c main_mpi.c -o main_mpi.o

game_of_life_mpi.o: game_of_life_mpi.c $(HEADERS)
	$(MPICC) $(CFLAGS_OMP) -c game_of_life_mpi.c -o game_of_life_mpi.o

clean:
	rm -f *.o $(TARGET_SERIAL) $(TARGET_OMP) $(TARGET_MPI) $(VALIDATE) *.pgm

test_serial: $(TARGET_SERIAL)
	./$(TARGET_SERIAL) -i -k 1000 -f test.pgm
	./$(TARGET_SERIAL) -r -f test.pgm -e 1 -n 100

test_omp: $(TARGET_OMP)
	export OMP_NUM_THREADS=4 && ./$(TARGET_OMP) -r -f test.pgm -e 1 -n 100

.PHONY: all clean test_serial test_omp