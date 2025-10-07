#include "game_of_life.h"

// Create and allocate grid
Grid* create_grid(int rows, int cols) {
    Grid *grid = malloc(sizeof(Grid));
    grid->rows = rows;
    grid->cols = cols;
    grid->data = calloc(rows * cols, sizeof(int));
    return grid;
}

// Free grid memory
void free_grid(Grid *grid) {
    free(grid->data);
    free(grid);
}

// Initialize grid with random cells (density = probability of alive cell)
void initialize_random(Grid *grid, double density) {
    srand(42);  // Fixed seed for reproducibility
    for (int i = 0; i < grid->rows * grid->cols; i++) {
        grid->data[i] = (rand() / (double)RAND_MAX) < density ? 1 : 0;
    }
}

// Count live neighbors with periodic boundary conditions
int count_neighbors(Grid *grid, int row, int col) {
    int count = 0;
    int rows = grid->rows;
    int cols = grid->cols;
    
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            
            // Periodic boundaries
            int r = (row + dr + rows) % rows;
            int c = (col + dc + cols) % cols;
            count += get_cell(grid, r, c);
        }
    }
    return count;
}

// ORDERED evolution: cells update sequentially, changes take effect immediately
void evolve_ordered(Grid *grid, int steps) {
    for (int step = 0; step < steps; step++) {
        // Row-major order: (0,0), (0,1), ..., (rows-1, cols-1)
        for (int i = 0; i < grid->rows; i++) {
            for (int j = 0; j < grid->cols; j++) {
                int neighbors = count_neighbors(grid, i, j);
                int cell = get_cell(grid, i, j);
                
                // Conway's rules
                if (cell == 1) {
                    set_cell(grid, i, j, (neighbors == 2 || neighbors == 3) ? 1 : 0);
                } else {
                    set_cell(grid, i, j, (neighbors == 3) ? 1 : 0);
                }
            }
        }
    }
}

// STATIC evolution: all cells update simultaneously based on frozen state
void evolve_static(Grid *grid, int steps) {
    Grid *temp = create_grid(grid->rows, grid->cols);
    
    for (int step = 0; step < steps; step++) {
        // Compute next state based on current frozen state
        for (int i = 0; i < grid->rows; i++) {
            for (int j = 0; j < grid->cols; j++) {
                int neighbors = count_neighbors(grid, i, j);
                int cell = get_cell(grid, i, j);
                
                // Conway's rules
                if (cell == 1) {
                    set_cell(temp, i, j, (neighbors == 2 || neighbors == 3) ? 1 : 0);
                } else {
                    set_cell(temp, i, j, (neighbors == 3) ? 1 : 0);
                }
            }
        }
        
        // Swap grids
        int *swap = grid->data;
        grid->data = temp->data;
        temp->data = swap;
    }
    
    free_grid(temp);
}

// STATIC evolution with OpenMP parallelization
void evolve_static_omp(Grid *grid, int steps) {
    Grid *temp = create_grid(grid->rows, grid->cols);
    
    for (int step = 0; step < steps; step++) {
        #pragma omp parallel for collapse(2) schedule(static)
        for (int i = 0; i < grid->rows; i++) {
            for (int j = 0; j < grid->cols; j++) {
                int neighbors = count_neighbors(grid, i, j);
                int cell = get_cell(grid, i, j);
                
                if (cell == 1) {
                    set_cell(temp, i, j, (neighbors == 2 || neighbors == 3) ? 1 : 0);
                } else {
                    set_cell(temp, i, j, (neighbors == 3) ? 1 : 0);
                }
            }
        }
        
        int *swap = grid->data;
        grid->data = temp->data;
        temp->data = swap;
    }
    
    free_grid(temp);
}

// Print grid (for debugging small grids)
void print_grid(Grid *grid) {
    for (int i = 0; i < grid->rows; i++) {
        for (int j = 0; j < grid->cols; j++) {
            printf("%c ", get_cell(grid, i, j) ? '#' : '.');
        }
        printf("\n");
    }
    printf("\n");
}

void write_csv_result(const char *filename, const char *study, int rows, int cols, 
                      int steps, int mode, int mpi_procs, int omp_threads, double time) {
    FILE *fp = fopen(filename, "a");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open CSV file %s\n", filename);
        return;
    }
    
    fprintf(fp, "%s,%d,%d,%d,%s,%d,%d,%.6f\n", 
            study, rows, cols, steps, mode == 0 ? "ordered" : "static", 
            mpi_procs, omp_threads, time);
    fclose(fp);
}

// Write grid to PGM image file (P2 ASCII format)
void write_pgm(Grid *grid, const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return;
    }
    
    fprintf(fp, "P2\n");
    fprintf(fp, "%d %d\n", grid->cols, grid->rows);
    fprintf(fp, "255\n");
    
    for (int i = 0; i < grid->rows; i++) {
        for (int j = 0; j < grid->cols; j++) {
            fprintf(fp, "%d ", get_cell(grid, i, j) ? 255 : 0);
        }
        fprintf(fp, "\n");
    }
    
    fclose(fp);
}

// Read grid from PGM image file
Grid* read_pgm(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        return NULL;
    }
    
    char magic[3];
    int cols, rows, maxval;
    
    if (fscanf(fp, "%2s", magic) != 1 || strcmp(magic, "P2") != 0) {
        fclose(fp);
        return NULL;
    }
    
    while (fgetc(fp) != '\n');  // skip to end of line
    
    // Skip comments
    int c;
    while ((c = fgetc(fp)) == '#') {
        while (fgetc(fp) != '\n');
    }
    ungetc(c, fp);
    
    if (fscanf(fp, "%d %d", &cols, &rows) != 2) {
        fclose(fp);
        return NULL;
    }
    
    if (fscanf(fp, "%d", &maxval) != 1) {
        fclose(fp);
        return NULL;
    }
    
    Grid *grid = create_grid(rows, cols);
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int val;
            if (fscanf(fp, "%d", &val) != 1) {
                free_grid(grid);
                fclose(fp);
                return NULL;
            }
            set_cell(grid, i, j, val > 0 ? 1 : 0);
        }
    }
    
    fclose(fp);
    return grid;
}