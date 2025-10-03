OPENBLASROOT = ${OPENBLAS_ROOT}
OPENBLAS = -L${OPENBLAS_ROOT}/lib -lopenblas -lpthread -fopenmp

BLISROOT = ${blis_root}
BLIS = -L${BLISROOT}/lib -lblis -lpthread -lm -fopenmp

CC = gcc
CFLAGS = -m64 -O3 -fopenmp
FLOAT_FLAGS = -DUSE_FLOAT
DOUBLE_FLAGS = -DUSE_DOUBLE

GEMM = ../gemm.c
DATA_FOLDER = ${data_folder}

OPENBLAS_f = $(DATA_FOLDER)/gemm_openblas_f.exe
BLIS_f = $(DATA_FOLDER)/gemm_blis_f.exe

OPENBLAS_d = $(DATA_FOLDER)/gemm_openblas_d.exe
BLIS_d = $(DATA_FOLDER)/gemm_blis_d.exe

.PHONY: float double clean all

all: float double

float: $(OPENBLAS_f) $(BLIS_f)

$(OPENBLAS_f): $(GEMM)
	@$(CC) $(CFLAGS) $(FLOAT_FLAGS) -DOPENBLAS $< -I${OPENBLAS_ROOT}/include $(OPENBLAS) -o $@

$(BLIS_f): $(GEMM)
	@$(CC) $(CFLAGS) $(FLOAT_FLAGS) -DBLIS $< -I$(BLISROOT)/include/blis $(BLIS) -o $@

double: $(OPENBLAS_d) $(BLIS_d)

$(OPENBLAS_d): $(GEMM)
	@$(CC) $(CFLAGS) $(DOUBLE_FLAGS) -DOPENBLAS $< -I${OPENBLAS_ROOT}/include $(OPENBLAS) -o $@

$(BLIS_d): $(GEMM)
	@$(CC) $(CFLAGS) $(DOUBLE_FLAGS) -DBLIS $< -I$(BLISROOT)/include/blis $(BLIS) -o $@

clean:
	@rm -f $(DATA_FOLDER)/*.exe

clean-all:
	@rm -f $(DATA_FOLDER)/*.exe
	@rm -rf ./THIN
	@rm -rf ./EPYC
