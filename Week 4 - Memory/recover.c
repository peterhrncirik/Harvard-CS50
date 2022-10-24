#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[])
{

    // Check if input is correct
    if(argc != 2){
        printf("Usage: ./recover image\n");
        return 1;
    }


    // Open & Check file
    FILE *card = fopen(argv[1], "r");

    if(!card){
        printf("Error: Can not open the image.\n");
        return 1;
    }

   // Repeat until EOF
   unsigned char buffer[512];
   int counter = 0;
   char* filename = malloc(sizeof(char));
   FILE *img;

   while(fread(buffer, 512, 1, card) == 1) {

        // Look for JPEG header
        if(buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && ((buffer[3] & 0xf0) == 0xe0)){

            // Is it the first JPEG?
            if(access("000.jpg", F_OK) == -1){

                // First found, create & open new file, write into it
                sprintf(filename, "%03i.jpg", counter);
                img = fopen(filename, "w");
                fwrite(buffer, 512, 1, img);

            } else {

                // Else close current file and open new
                fclose(img);
                counter++;
                sprintf(filename, "%03i.jpg", counter);
                img = fopen(filename, "w");
                fwrite(buffer, 512, 1, img);
            }

        } else {

            // Already found JPEG, keep writing into it
            fwrite(buffer, 512, 1, img);
        }

   }


    // Close items & free memory
        fclose(img);
        fclose(card);
        free(filename);

}
