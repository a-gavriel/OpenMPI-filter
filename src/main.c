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


  uint8_t* image;
  uint8_t* filtered_image;

  getImageSize(input_image, &WIDTH, &HEIGHT);
  image = read_image( input_image, WIDTH, HEIGHT, CHANNELS, CHANNELS);
  filtered_image = (uint8_t*) malloc(WIDTH*(HEIGHT)*CHANNELS * sizeof(uint8_t));

  unsigned int row_first, rows_per_rank, row_last;
  rows_per_rank = HEIGHT /(n_ranks-1);
  

  if (rank == 0){
            
    for(int i=0;i<n_ranks-1;++i)   {
      row_first = i * rows_per_rank;
      MPI_Recv(filtered_image+row_first*WIDTH*CHANNELS, WIDTH*rows_per_rank*CHANNELS, MPI_BYTE, i+1, 0, MPI_COMM_WORLD, &status);    
    }
    write_image(ouput_image, filtered_image);
    printf("received image from %d\n",rank);
    
  }else{   
    /* 
    if( HEIGHT%(n_ranks-1) > 0 ){
    // Add 1 in case the number of ranks doesn't divide the number of numbers
    rows_per_rank += 1;
  }*/
    unsigned int extra = 0;
    if(rank == n_ranks - 1){
      extra = HEIGHT % rows_per_rank;
    }
    // Figure out the first and the last iteration for this rank
    row_first = (rank-1) * rows_per_rank;
    row_last = row_first + rows_per_rank + 0;
    
    processImage( image , filtered_image, 1, window_size, window_size, row_first, row_last );
    MPI_Send(filtered_image+row_first*WIDTH*CHANNELS, WIDTH*rows_per_rank*CHANNELS, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    printf("send image from %d\n",rank);
      
  }

  // Free the memory of the images  
  free(filtered_image);
  
  





  return MPI_Finalize();


}