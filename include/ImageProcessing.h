//
// Created by erick on 20/5/20.
// 

#ifndef T1_RAW_IMAGEPROCESSING_H
#define T1_RAW_IMAGEPROCESSING_H


#include <stdio.h>
#include <stdint.h>
// Bibioteca de lectura y escritura de imagenes tomada de
// https://github.com/nothings/stb
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "constants.h"
#include "Filters.h"



/*  Loads an image from memory
 *
 *  filename:   Path to the image
 *  width:      Width size of the image
 *  height:     Height size of the image
 *  channels:   Number of channels the image has
 *  desired_channels:   Number of channels we want to load from the image
 *
 */
uint8_t* read_image(char* filename, int width, int height, int channels, int desired_channels){


    // Read the image with a given w*h*c, last parameter is to ignore channels
    uint8_t* image = stbi_load(filename, &width, &height, &channels, desired_channels);
    if(image == NULL) {
        printf("Error in loading the image\n");
        exit(1);
    }

    // Return the image
    return image;
}

void write_image(char* filename, uint8_t* image){
    // Write the image in png
    stbi_write_png(filename, WIDTH, HEIGHT, CHANNELS, image, WIDTH*CHANNELS);
}

/*  Function that takes an image and applies to it a given filter using the defined
    kernel size. Because of the filter implementation, every image is zero padded.

    target: Image to be filtered
    filter: Filter function to be applied
    kernel_size: Kernel size of the filter
*/
uint8_t* apply_filter(  uint8_t* target,
                        uint8_t* filtered_image,
                        uint8_t (*filter)(uint8_t*, size_t*, uint8_t),
                        uint8_t kernel_size,                        
                        unsigned int start_row,
                        unsigned int end_row){

    //Prepare variables needed for the convolution process
    size_t pos[2];    

    // Start the convolution
    // HEIGHT and WIDTH limits have to be multiplicated to take into account
    // the channels of the image
    for (size_t h = start_row*WIDTH*CHANNELS; h < end_row*WIDTH*CHANNELS; h += WIDTH*CHANNELS)
    {
        pos[1] = h;
        for (size_t w = 0; w < WIDTH*CHANNELS; w += CHANNELS)
        {
            // Calc the new kernel center
            pos[0] = w;

            // Apply the filter to each channel
            for (size_t c = 0; c < CHANNELS; ++c)
            {
                pos[0] = w + c;
                // Set the new value in the filtered image
                *(filtered_image + h + w + c) = (*filter)(target, pos, kernel_size);

            }
        }
    }

    // Return result
    return 0;

}


/*  Takes and RGB image and calculates the mean value of each channel, then
 *  selects the channel with the highest value and returns a value between 0-2
 *  representing the selected rgb channel
 *
 *  image: ptr to the image target
 *
 *  return: [0, 3] value representing the rgb channel
 */
uint8_t get_strongest_channel(uint8_t* image){

    // Define accumulator variable
    float* mean_rgb_values = calloc(3, sizeof(float));


    // HEIGHT and WIDTH limits have to be multiplicated to take into account
    // the channels of the image
    for (size_t h = 0; h < HEIGHT*WIDTH*CHANNELS; h += WIDTH*CHANNELS){

        for (size_t w = 0; w < WIDTH*CHANNELS; w += CHANNELS){

            // Add the r value
            mean_rgb_values[0] += (float)*(image + h + w) / (float)(HEIGHT*WIDTH);
            // Add the g value
            mean_rgb_values[1] += (float)*(image + h + w + 1) / (float)(HEIGHT*WIDTH);
            // Add the b value
            mean_rgb_values[2] += (float)*(image + h + w + 2) / (float)(HEIGHT*WIDTH);
        }
    }
    uint8_t result;
    // If the r channel is stronger or equal to the rest, select it
    if((mean_rgb_values[0] >= mean_rgb_values[1]) && (mean_rgb_values[0] >= mean_rgb_values[2])){
        result = 0;
    }
        // If the g channel is stronger or equal to the rest, select it
    else if(mean_rgb_values[1] >= mean_rgb_values[2]){
        result = 1;
    }
        // If the b channel is stronger or equal to the rest, select it
    else{
        result = 2;
    }
    free(mean_rgb_values);
    return result;
}


int processImage(uint8_t* image, uint8_t* filtered_image, 
    int filters, 
    uint8_t kmedian, uint8_t kavg, 
    uint32_t start_row, uint32_t end_row){

    if (filters == 1){
        filtered_image = apply_filter(image, filtered_image, &median_filter, kmedian, start_row, end_row);
        stbi_image_free(image);

    }else if(filters == 2){
        filtered_image = apply_filter(image, filtered_image, &avg_filter, kavg, start_row, end_row);
        stbi_image_free(image);

    }else if(filters == 3){
        filtered_image = apply_filter(image, filtered_image, &median_filter, kmedian, start_row, end_row);
        stbi_image_free(image);
        image = filtered_image;
        filtered_image = apply_filter(image, filtered_image, &avg_filter, kavg, start_row, end_row);
        free(image);

    }else if(filters == 4){    
        filtered_image = apply_filter(image, filtered_image, &avg_filter, kavg, start_row, end_row);
        stbi_image_free(image);
        image = filtered_image;
        filtered_image = apply_filter(image, filtered_image, &median_filter, kmedian, start_row, end_row);
        free(image);

    }else{
        // Start the convolution
        filtered_image = apply_filter(image, filtered_image, &median_filter, 3, start_row, end_row);
        // Store the filtered image
        stbi_image_free(image);
    }

    

    
    
    return 0;

}

#endif //T1_RAW_IMAGEPROCESSING_H
