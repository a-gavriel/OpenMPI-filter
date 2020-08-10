#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <mpi.h>
#include <time.h>      // Date Time
#include <sys/times.h> // User Time System Time
#include <sys/time.h>  // Elapsed Time

#include "../include/getImageSize.h"
#include "../include/ImageProcessing.h"


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

  //Time Vars      -------------------------
  clock_t beginProcess,endProcess;
  struct tms sys_usr_start_tms, sys_usr_end_tms;
  struct timeval  tv1, tv2;

  // Start timers
  beginProcess = clock();  
  times(&sys_usr_start_tms);
  gettimeofday(&tv1, NULL);
  // End Time Vars -------------------------
  
  int rank, n_ranks;
  MPI_Status status;
  //MPI_Request request;
  // First call MPI_Init
  MPI_Init(&argc, &argv);

  // Get my rank and the number of ranks
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &n_ranks);


  uint8_t* image;
  uint8_t* filtered_image;

  


  unsigned int row_first, rows_per_rank, row_last;
  
  getImageSize(input_image, &WIDTH, &HEIGHT);
  rows_per_rank = HEIGHT /(n_ranks-1);
  
  filtered_image = (uint8_t*) malloc(WIDTH*(HEIGHT)*CHANNELS * sizeof(uint8_t));


  if (rank == 0){
    
    //printf("%dx%d - %u\n", WIDTH, HEIGHT,rows_per_rank);


    struct timeval  ttv1, ttv2;
    
    gettimeofday(&ttv1, NULL);
    for(int i=0;i<n_ranks-1;++i){
      row_first = i * rows_per_rank;
      MPI_Recv(filtered_image+row_first*WIDTH*CHANNELS, WIDTH*rows_per_rank*CHANNELS, MPI_BYTE, i+1, 0, MPI_COMM_WORLD, &status);    
      ///printf("received image from %d\n",i+1);

    }    
    gettimeofday(&ttv2, NULL);
    double receive_time = (double) (ttv2.tv_usec - ttv1.tv_usec) / 1000000 + (double) (ttv2.tv_sec - ttv1.tv_sec);
    
    
    write_image(ouput_image, filtered_image);

    // End timers
    times(&sys_usr_end_tms);
    endProcess = clock();
    gettimeofday(&tv2, NULL);

    // Calculating times
    double elapsed_time = (double) (tv2.tv_usec - tv1.tv_usec) / 1000000 + (double) (tv2.tv_sec - tv1.tv_sec);
    double process_time = (double)(endProcess - beginProcess) / CLOCKS_PER_SEC;
    double sys_time = (double)(sys_usr_end_tms.tms_stime - sys_usr_start_tms.tms_stime)/100;
    double usr_time = (double)(sys_usr_end_tms.tms_utime - sys_usr_start_tms.tms_utime)/100;
    double suspended_time = elapsed_time - process_time;

    printf(KNRM"************************************************************ \n");
    printf(KCYN"Statistics: \n");    
    printf(KMAG"\nTotal time %f \n", elapsed_time);
    printf("  - Receive time %f \n",   receive_time);   
    //printf("  - Wait time %f \n", sleep_timer);  
    //printf("  - Other time %f \n",  suspended_time - sleep_timer - blocked_timer);    
    printf("- Processing time %f \n",   process_time);
    printf("  - System time %f \n",  sys_time );
    printf("  - User time %f \n",  usr_time);


    printf(KNRM"************************************************************ \n");
    
    
    
  }else{   
    image = read_image( input_image, WIDTH, HEIGHT, CHANNELS, CHANNELS);
    /*
    unsigned int extra = 0;
    if(rank == n_ranks - 1){
      extra = HEIGHT % rows_per_rank;
    }
    */
    // Figure out the first and the last iteration for this rank
    row_first = (rank-1) * rows_per_rank;
    row_last = row_first + rows_per_rank + 0;
    
    processImage( image , filtered_image, 1, window_size, window_size, row_first, row_last );
    MPI_Send(filtered_image+row_first*WIDTH*CHANNELS, WIDTH*rows_per_rank*CHANNELS, MPI_BYTE, 0, 0, MPI_COMM_WORLD);
    //printf("send image from %d\n",rank);
      
  }

  // Free the memory of the images  
  free(filtered_image);
  
  





  return MPI_Finalize();


}