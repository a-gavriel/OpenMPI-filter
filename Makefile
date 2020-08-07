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


CFILES = $(SRC_DIR)/getImageSize.c $(SRC_DIR)/main.c
filter: 
	$(CC) $(CFILES) -o $(BIN_DIR)/filter $(LDLIBS) $(CFLAGS)

