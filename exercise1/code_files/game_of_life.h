#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef enum {
    ORDERED,  
    STATIC    
} EvolutionMode;


typedef struct {
    int *data;        
    int rows;
    int cols;
} Grid;

Grid* create_grid(int rows, int cols);
void free_grid(Grid *grid);
void initialize_random(Grid *grid, double density);
int count_neighbors(Grid *grid, int row, int col);
void evolve_ordered(Grid *grid, int steps);
void evolve_static(Grid *grid, int steps);
void evolve_static_omp(Grid *grid, int steps);
void evolve_static_mpi(Grid *local_grid, int steps, int rank, int size, int global_rows);
void print_grid(Grid *grid);
void write_pgm(Grid *grid, const char *filename);
Grid* read_pgm(const char *filename);
void write_csv_result(const char *filename, const char *study, int rows, int cols, 
                      int steps, int mode, int mpi_procs, int omp_threads, double time);


static inline int get_cell(Grid *grid, int row, int col) {
    return grid->data[row * grid->cols + col];
}

static inline void set_cell(Grid *grid, int row, int col, int value) {
    grid->data[row * grid->cols + col] = value;
}

#endif