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
	cd bin && mpirun -n 2 ./filter test1.jpg out.jpg 20

cluster: filter
	cp bin/filter ../shared-task/filter
	cd ../shared-task && mpirun --hostfile /etc/hosts -np 6 ./filter test1.jpg out.jpg 5

CFILES = $(SRC_DIR)/getImageSize.c $(SRC_DIR)/main.c
filter: createBin
	$(CC) $(CFILES) -o $(BIN_DIR)/filter $(LDLIBS) $(CFLAGS)


