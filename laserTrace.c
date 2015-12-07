#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#include "headers/canny.h"
#include "headers/ilda.h"
#include "source/canny.c"
#include "source/ilda.c"

void main(int argc, char *argv[])
{
   char *infilename = NULL;  /* Name of the input image */
   char *dirfilename = NULL; /* Name of the output gradient direction image */
   char outfilename[128];    /* Name of the output "edge" image */
   char composedfname[128];  /* Name of the output "direction" image */
   unsigned char *image;     /* The input image */
   unsigned char *edge;      /* The output edge image */
   int rows, cols;           /* The dimensions of the image. */
   int compression;
   float sigma,              /* Standard deviation of the gaussian kernel. */
	 tlow,               /* Fraction of the high threshold in hysteresis. */
	 thigh;              /* High hysteresis threshold control. The actual
			        threshold is the (100 * thigh) percentage point
			        in the histogram of the magnitude of the
			        gradient image that passes non-maximal
			        suppression. */

    infilename = malloc(sizeof(char) * 128);
    printf("File: ");
    scanf("%128s", infilename);
    printf("Sigma: ");
    scanf("%f", &sigma);
    printf("Low Threshold: ");
    scanf("%f", &tlow);
    printf("High Threshold: ");
    scanf("%f", &thigh);
    printf("ILDA Compression: ");
    scanf("%d", &compression);

    int result ;
    char * str = malloc(sizeof(char) * 256);
    sprintf(str, "convert %s in.pgm", infilename);
    result=system(str);
    infilename = "in.pgm";

    if(argc == 6) dirfilename = infilename;
    else dirfilename = NULL;

   /****************************************************************************
   * Read in the image. This read function allocates memory for the image.
   ****************************************************************************/
   if(VERBOSE) printf("Reading the image %s.\n", infilename);
   if(read_pgm_image(infilename, &image, &rows, &cols) == 0){
      fprintf(stderr, "Error reading the input image, %s.\n", infilename);
      exit(1);
   }


   /****************************************************************************
   * Perform the edge detection. All of the work takes place here.
   ****************************************************************************/
   if(VERBOSE) printf("Starting Canny edge detection.\n");
   if(dirfilename != NULL){
      sprintf(composedfname, "%s_s_%3.2f_l_%3.2f_h_%3.2f.fim", infilename, sigma, tlow, thigh);
      dirfilename = composedfname;
   }
   canny(image, rows, cols, sigma, tlow, thigh, &edge, dirfilename);


   /****************************************************************************
   * Write out the edge image to a file.
   ****************************************************************************/
   sprintf(outfilename, "%s_s_%3.2f_l_%3.2f_h_%3.2f.pgm", infilename,
      sigma, tlow, thigh);
   if(VERBOSE) printf("Writing the edge iname in the file %s.\n", outfilename);
   if(write_pgm_image(outfilename, edge, rows, cols, "", 255) == 0){
      fprintf(stderr, "Error writing the edge image, %s.\n", outfilename);
      exit(1);
   }

    sprintf(str, "convert %s out.bmp", outfilename, sigma, tlow, thigh);
    result=system(str);

    height = rows;
    width = cols;

    mapOutline(edge);
    compressILDA(compression);
    saveILDA(NULL, 0, 0);

    /*Delete irrelevant files*/
    unlink("in.pgm");
    unlink(outfilename);

}
