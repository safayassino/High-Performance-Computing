#include "game_of_life.h"

int main(int argc, char *argv[]) {
    if (argc < 6) {
        printf("Usage: %s <rows> <cols> <steps> <mode:0=ordered,1=static> <output_prefix>\n", argv[0]);
        printf("Example: %s 8 8 1 1 test\n", argv[0]);
        printf("  Creates: test_init.pgm, test_step001.pgm, etc.\n");
        return 1;
    }
    
    int rows = atoi(argv[1]);
    int cols = atoi(argv[2]);
    int steps = atoi(argv[3]);
    int mode = atoi(argv[4]);
    char *prefix = argv[5];
    
    printf("Validation Run\n");
    printf("Grid: %dx%d, Steps: %d, Mode: %s\n",
           rows, cols, steps, mode == ORDERED ? "ORDERED" : "STATIC");
    
    // Create grid
    Grid *grid = create_grid(rows, cols);
    memset(grid->data, 0, grid->rows * grid->cols * sizeof(int));

    // the glider pattern at position (1,1)
    set_cell(grid, 1, 2, 1);
    set_cell(grid, 2, 3, 1);
    set_cell(grid, 3, 1, 1);
    set_cell(grid, 3, 2, 1);
    set_cell(grid, 3, 3, 1);
    
    char filename[256];
    sprintf(filename, "%s_init.pgm", prefix);
    write_pgm(grid, filename);
    printf("Saved: %s\n", filename);
    
    // Evolve step by step and save each generation
    if (mode == ORDERED) {
        for (int step = 1; step <= steps; step++) {
            evolve_ordered(grid, 1);
            sprintf(filename, "%s_step%03d.pgm", prefix, step);
            write_pgm(grid, filename);
            printf("Saved: %s\n", filename);
        }
    } else {
        for (int step = 1; step <= steps; step++) {
            evolve_static(grid, 1);
            sprintf(filename, "%s_step%03d.pgm", prefix, step);
            write_pgm(grid, filename);
            printf("Saved: %s\n", filename);
        }
    }
    
    
    free_grid(grid);
    return 0;
}
