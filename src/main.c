#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <mpi.h>

#include "../include/getImageSize.h"
#include "../include/ImageProcessing.h"
#include "../include/constants.h"

unsigned int WIDTH = 235;
unsigned int HEIGHT = 215;
unsigned int CHANNELS = 3;

int main(int argc, char **argv){  
  if(argc != 4){
    printf("Error , first arg = name, second arg = out, third arg = wSize\n");
    return 1;
  }  
  char* input_image = argv[1];
  char* ouput_image = argv[2];
  uint8_t window_size = (uint8_t) atoi(argv[3]);
  if (window_size < 2){
    printf("Error, window_size must be greater than 1\n");
    return 1;
  }
  
  int rank, n_ranks;
  MPI_Status status;
  MPI_Request request;
  // First call MPI_Init
  MPI_Init(&argc, &argv);

  // Get my rank and the number of ranks
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);
  if(n_ranks != 2){
    printf("only runs with n = 2\n");
    return 1;
  }

  uint8_t* image;
  uint8_t* filtered_image;

  getImageSize(input_image, &WIDTH, &HEIGHT);
  image = read_image( input_image, WIDTH, HEIGHT, CHANNELS, CHANNELS);
  filtered_image = (uint8_t*) malloc(WIDTH*(HEIGHT)*CHANNELS * sizeof(uint8_t));

  if (rank == 0){
    
    
    
    MPI_Recv(filtered_image, WIDTH*HEIGHT*CHANNELS, MPI_BYTE, 1, 0, MPI_COMM_WORLD, &status);
    
    write_image(ouput_image, filtered_image);
    printf("received image from %d\n",rank);
    
  }else{    

    processImage( image , filtered_image, 1, window_size, window_size, 0, HEIGHT );
    MPI_Send(filtered_image, WIDTH*HEIGHT*CHANNELS, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    printf("send image from %d\n",rank);
      
  }

  // Free the memory of the images  
  free(filtered_image);
  
  





  return MPI_Finalize();


}