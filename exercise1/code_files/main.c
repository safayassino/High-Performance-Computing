#define _POSIX_C_SOURCE 200809L

#include "game_of_life.h"
#include <sys/time.h>
#include <unistd.h>

#ifdef USE_OPENMP
#include <omp.h>
#endif

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

void print_usage(char *progname) {
    printf("Usage: %s -i|-r -k <size> -e <0|1> -f <filename> -n <steps>\n", progname);
    printf("  -i             Initialize playground (write to file)\n");
    printf("  -r             Run playground (read from file)\n");
    printf("  -k <size>      Playground size (rows,cols or single value for square)\n");
    printf("  -e <0|1>       Evolution: 0=ordered, 1=static\n");
    printf("  -f <filename>  File to read/write\n");
    printf("  -n <steps>     Number of evolution steps\n");
}

int main(int argc, char *argv[]) {
    int opt;
    int init_mode = -1;  // -1=unset, 0=run, 1=init
    int rows = 0, cols = 0;
    int evolution = 1;   // default: static
    char *filename = NULL;
    int steps = 0;
    
    while ((opt = getopt(argc, argv, "irk:e:f:n:")) != -1) {
        switch (opt) {
            case 'i':
                init_mode = 1;
                break;
            case 'r':
                init_mode = 0;
                break;
            case 'k':
                if (sscanf(optarg, "%d,%d", &rows, &cols) == 1) {
                    cols = rows;  // square grid
                } else if (sscanf(optarg, "%d,%d", &rows, &cols) != 2) {
                    fprintf(stderr, "Error: Invalid size format\n");
                    return 1;
                }
                break;
            case 'e':
                evolution = atoi(optarg);
                break;
            case 'f':
                filename = optarg;
                break;
            case 'n':
                steps = atoi(optarg);
                break;
            default:
                print_usage(argv[0]);
                return 1;
        }
    }
    
    if (init_mode == -1 || !filename) {
        print_usage(argv[0]);
        return 1;
    }
    
    if (init_mode == 1) {
        // Initialize mode
        if (rows == 0 || cols == 0) {
            fprintf(stderr, "Error: Size required for initialization\n");
            return 1;
        }
        Grid *grid = create_grid(rows, cols);
        initialize_random(grid, 0.3);
        write_pgm(grid, filename);
        printf("Initialized %dx%d grid to %s\n", rows, cols, filename);
        free_grid(grid);
    } else {
        // Run mode
        if (steps == 0) {
            fprintf(stderr, "Error: Number of steps required\n");
            return 1;
        }
        Grid *grid = read_pgm(filename);
        if (!grid) {
            fprintf(stderr, "Error: Cannot read file %s\n", filename);
            return 1;
        }
        
        printf("Grid: %dx%d, Steps: %d, Mode: %s\n",
               grid->rows, grid->cols, steps, evolution == 0 ? "ORDERED" : "STATIC");
        
        double start = get_time();
        if (evolution == 0) {
            evolve_ordered(grid, steps);
        } else {
            evolve_static(grid, steps);
        }
        double elapsed = get_time() - start;
        
        printf("Time: %.6f seconds\n", elapsed);
        printf("Cells/sec: %.2e\n", (double)grid->rows * grid->cols * steps / elapsed);
        
        free_grid(grid);
    }
    
    return 0;
}