#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <stdint.h>


#include "../include/getImageSize.h"
#include "../include/ImageProcessing.h"
#include "../include/constants.h"

uint32_t WIDTH = 235;
uint32_t HEIGHT = 215;
uint32_t CHANNELS = 3;

void filter( char fullname [256], char filteredName[256], uint8_t window){
    

    getImageSize(fullname, &WIDTH, &HEIGHT);
    // Load the image
    // uint8_t* image = read_image( fullname, WIDTH, HEIGHT, CHANNELS, CHANNELS);
    
    //uint8_t* filtered_image;

    

    processImage( fullname , filteredName, 1, window, window, 0, HEIGHT );

    //write_image(filteredName, filtered_image);
    // Free the memory of the images
    //free(filtered_image);
    

}



int main(int argc, char **argv){
  
  if (argc == 4) {
    filter(argv[1],argv[2], (uint8_t) atoi(argv[3]));
  }else{
    printf("Error , first arg = name, second arg = out, third arg = wSize\n");
    return 1;
  }
  return 0;

}