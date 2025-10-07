#define _POSIX_C_SOURCE 200809L

#include "game_of_life.h"
#include <sys/time.h>
#include <unistd.h>
#include <mpi.h>

#ifdef _OPENMP
#include <omp.h>
#endif

double get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec * 1e-6;
}

void print_usage(char *progname) {
    printf("Usage: %s -i|-r -k <size> -e <0|1> -f <filename> -n <steps>\n", progname);
    printf("  -i             Initialize playground\n");
    printf("  -r             Run playground\n");
    printf("  -k <size>      Playground size\n");
    printf("  -e <0|1>       Evolution: 0=ordered, 1=static\n");
    printf("  -f <filename>  File to read/write\n");
    printf("  -n <steps>     Number of steps\n");
}

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    int opt;
    int init_mode = -1;
    int rows = 0, cols = 0;
    int evolution = 1;
    char *filename = NULL;
    int steps = 0;
    
    while ((opt = getopt(argc, argv, "irk:e:f:n:")) != -1) {
        switch (opt) {
            case 'i': init_mode = 1; break;
            case 'r': init_mode = 0; break;
            case 'k':
                if (sscanf(optarg, "%d,%d", &rows, &cols) == 1) {
                    cols = rows;
                } else if (sscanf(optarg, "%d,%d", &rows, &cols) != 2) {
                    if (rank == 0) fprintf(stderr, "Error: Invalid size\n");
                    MPI_Finalize();
                    return 1;
                }
                break;
            case 'e': evolution = atoi(optarg); break;
            case 'f': filename = optarg; break;
            case 'n': steps = atoi(optarg); break;
            default:
                if (rank == 0) print_usage(argv[0]);
                MPI_Finalize();
                return 1;
        }
    }
    
    if (init_mode == -1 || !filename) {
        if (rank == 0) print_usage(argv[0]);
        MPI_Finalize();
        return 1;
    }
    
    if (init_mode == 1) {
        if (rank == 0) {
            if (rows == 0 || cols == 0) {
                fprintf(stderr, "Error: Size required\n");
                MPI_Finalize();
                return 1;
            }
            Grid *grid = create_grid(rows, cols);
            initialize_random(grid, 0.3);
            write_pgm(grid, filename);
            printf("Initialized %dx%d grid to %s\n", rows, cols, filename);
            free_grid(grid);
        }
    } else {
        Grid *global_grid = NULL;
        if (rank == 0) {
            global_grid = read_pgm(filename);
            if (!global_grid) {
                fprintf(stderr, "Error: Cannot read %s\n", filename);
                MPI_Abort(MPI_COMM_WORLD, 1);
            }
            rows = global_grid->rows;
            cols = global_grid->cols;
        }
        
        MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
        
        int local_rows = rows / size;
        Grid *local_grid = create_grid(local_rows + 2, cols);
        
        if (rank == 0) {
            for (int i = 0; i < local_rows; i++) {
                for (int j = 0; j < cols; j++) {
                    set_cell(local_grid, i + 1, j, get_cell(global_grid, i, j));
                }
            }
            for (int r = 1; r < size; r++) {
                int *send_buf = malloc(local_rows * cols * sizeof(int));
                for (int i = 0; i < local_rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        send_buf[i * cols + j] = get_cell(global_grid, r * local_rows + i, j);
                    }
                }
                MPI_Send(send_buf, local_rows * cols, MPI_INT, r, 0, MPI_COMM_WORLD);
                free(send_buf);
            }
            free_grid(global_grid);
        } else {
            int *recv_buf = malloc(local_rows * cols * sizeof(int));
            MPI_Recv(recv_buf, local_rows * cols, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (int i = 0; i < local_rows; i++) {
                for (int j = 0; j < cols; j++) {
                    set_cell(local_grid, i + 1, j, recv_buf[i * cols + j]);
                }
            }
            free(recv_buf);
        }
        
        MPI_Barrier(MPI_COMM_WORLD);
        double start = MPI_Wtime();
        
        if (evolution == 0) {
            if (rank == 0) printf("Warning: ORDERED mode not parallelizable with MPI\n");
        } else {
            evolve_static_mpi(local_grid, steps, rank, size, rows);
        }
        
        double elapsed = MPI_Wtime() - start;
        
        if (rank == 0) {
#ifdef _OPENMP
            printf("Grid: %dx%d, Steps: %d, Mode: STATIC, Procs: %d, Threads: %d\n",
                   rows, cols, steps, size, omp_get_max_threads());
#else
            printf("Grid: %dx%d, Steps: %d, Mode: STATIC, Procs: %d\n",
                   rows, cols, steps, size);
#endif
            printf("Time: %.6f seconds\n", elapsed);
            printf("Cells/sec: %.2e\n", (double)rows * cols * steps / elapsed);
            
#ifdef _OPENMP
            int threads = omp_get_max_threads();
#else
            int threads = 1;
#endif
            write_csv_result("results_mpi.csv", "mpi", rows, cols, 
                            steps, evolution, size, threads, elapsed);
        }
        
        free_grid(local_grid);
    }
    
    MPI_Finalize();
    return 0;
}