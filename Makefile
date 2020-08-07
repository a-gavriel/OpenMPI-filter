all: createBin filter
.PHONY: clean filter

CFLAGS = -Wall
CC := mpicc
LDLIBS = -lm

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin
IDIR := include

MFLAG = -lm
OBJ = hellomake.o hellofunc.o 
DEPS = getImageSize.h ImageProcessing.h constants.h

clean:	
	@rm -rf $(BIN_DIR)

createBin:
	@mkdir -p $(BIN_DIR)

run: filter
	cd bin && mpirun -n 2 ./filter Lenna.jpg out.jpg 4

run2: filter
	cd bin && mpirun -n 2 ./filter Lenna.jpg out.jpg 20

run3: filter
	cd bin && mpirun -n 3 ./filter Lenna.jpg out.jpg 4

run4: filter
	cd bin && mpirun -n 4 ./filter Lenna.jpg out.jpg 4

CFILES = $(SRC_DIR)/getImageSize.c $(SRC_DIR)/main.c
filter: 
	$(CC) $(CFILES) -o $(BIN_DIR)/filter $(LDLIBS) $(CFLAGS)


