#ifndef GET_IMG_SIZE
#define GET_IMG_SIZE
#include <stdint.h>

unsigned int reverse_bytes(unsigned int bytes);


#define readbyte(a,b) do if(((a)=getc((b))) == EOF) return 0; while (0)
#define readword(a,b) do { int cc_=0,dd_=0; \
                          if((cc_=getc((b))) == EOF \
        		  || (dd_=getc((b))) == EOF) return 0; \
                          (a) = (cc_<<8) + (dd_); \
                          } while(0)


int scanhead_JPEG (FILE * infile, unsigned int * image_width, unsigned int * image_height);

int scanhead_PNG(FILE * fp, unsigned int * image_width, unsigned int * image_height);

int getImageSize(char fname[256], unsigned int * image_width, unsigned int * image_height);


#endif