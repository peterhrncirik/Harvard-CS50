#include "helpers.h"
#include "math.h"


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            // Calculate average
            float avg = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;

            //Update pixels accordingly
            image[i][j].rgbtBlue = round(avg);
            image[i][j].rgbtGreen = round(avg);
            image[i][j].rgbtRed = round(avg);
        }
    }
}


// Convert image to sepia
int maximum (int number)
{
    if (number > 255)
    {
        number = 255;
    }
    return number;
}

void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            int sepiaRed = maximum(round(.393 * (float)image[i][j].rgbtRed + .769 * (float)image[i][j].rgbtGreen + .189 * (float)image[i][j].rgbtBlue));
            int sepiaGreen = maximum(round(.349 * (float)image[i][j].rgbtRed + .686 * (float)image[i][j].rgbtGreen + .168 * (float)image[i][j].rgbtBlue));
            int sepiaBlue = maximum(round(.272 * (float)image[i][j].rgbtRed + .534 * (float)image[i][j].rgbtGreen + .131 * (float)image[i][j].rgbtBlue));

            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp;
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width/2; j++){
            tmp = image[i][j];
            image[i][j] = image[i][width-j-1];
            image[i][width-j-1] = tmp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    // New variables
    RGBTRIPLE imageCopy[height][width];


    // Make a copy
    for(int row = 0; row < height; row++){
        for(int column = 0; column < width; column++){
            imageCopy[row][column] = image[row][column];
        }
    }

    // Main loop
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            int newBlue = 0;
            int newGreen = 0;
            int newRed = 0;
            float counter = 0;

            // CORNERS

                // Bottom left
                if(i >= 0 && j - 1 >= 0){
                    newBlue += imageCopy[i][j - 1].rgbtBlue;
                    newGreen += imageCopy[i][j - 1].rgbtGreen;
                    newRed += imageCopy[i][j - 1].rgbtRed;
                    counter++;
                }

                // Bottom right
                if(i >= 0 && j >= 0){
                    newBlue += imageCopy[i][j].rgbtBlue;
                    newGreen += imageCopy[i][j].rgbtGreen;
                    newRed += imageCopy[i][j].rgbtRed;
                    counter++;
                }

                // Upper left
                if(i - 1 >= 0 && j >= 0){
                    newBlue += imageCopy[i - 1][j].rgbtBlue;
                    newGreen += imageCopy[i - 1][j].rgbtGreen;
                    newRed += imageCopy[i - 1][j].rgbtRed;
                    counter++;
                }

                // Upper right
                if(i - 1 >= 0 && j - 1 >= 0){
                    newBlue += imageCopy[i - 1][j - 1].rgbtBlue;
                    newGreen += imageCopy[i - 1][j - 1].rgbtGreen;
                    newRed += imageCopy[i - 1][j - 1].rgbtRed;
                    counter++;
                }

            // EDGES

                // Left edge
                if((i + 1 >= 0 && j >= 0) && (i + 1 < height && j >= 0)){
                    newBlue += imageCopy[i + 1][j].rgbtBlue;
                    newGreen += imageCopy[i + 1][j].rgbtGreen;
                    newRed += imageCopy[i + 1][j].rgbtRed;
                    counter++;
                }

                // Right edge
                if((i + 1 >= 0 && j - 1 >= 0) && (i + 1 < height && j - 1 >= 0)){
                    newBlue += imageCopy[i + 1][j - 1].rgbtBlue;
                    newGreen += imageCopy[i + 1][j - 1].rgbtGreen;
                    newRed += imageCopy[i + 1][j - 1].rgbtRed;
                    counter++;
                }

                // Upper edge
                if((i - 1 >= 0 && j + 1 >= 0) && (i - 1 >= 0 && j + 1 < width)){
                    newBlue += imageCopy[i - 1][j + 1].rgbtBlue;
                    newGreen += imageCopy[i - 1][j + 1].rgbtGreen;
                    newRed += imageCopy[i - 1][j + 1].rgbtRed;
                    counter++;
                }

                // Bottom edge
                if((i >= 0 && j + 1 >= 0) && (i >= 0 && j + 1 < width)){
                    newBlue += imageCopy[i][j + 1].rgbtBlue;
                    newGreen += imageCopy[i][j + 1].rgbtGreen;
                    newRed += imageCopy[i][j + 1].rgbtRed;
                    counter++;
                }

            // MIDDLE PIXELS

                if((i + 1 >= 0 && j + 1 >= 0) && (i + 1 < height && j + 1 < width)){
                    newBlue += imageCopy[i + 1][j + 1].rgbtBlue;
                    newGreen += imageCopy[i + 1][j + 1].rgbtGreen;
                    newRed += imageCopy[i + 1][j + 1].rgbtRed;
                    counter++;
                }

            // Calculate average
            image[i][j].rgbtBlue = round(newBlue / counter);
            image[i][j].rgbtGreen = round(newGreen / counter);
            image[i][j].rgbtRed = round(newRed / counter);
        }
    }

return;
}
