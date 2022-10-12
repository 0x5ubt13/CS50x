#include "helpers.h"
#include <stdio.h>
#include <inttypes.h>
#include <math.h>

// image[height][] is the row
// image[][width] is the column

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Calculate the average of brightness for every pixel's channel and equal all 3 values to the same to create grey
    for (int i = 0; i < height; i++) // row
    {
        for (int j = 0; j < width; j++) // column
        {
            uint8_t red   = image[i][j].rgbtRed;
            uint8_t green = image[i][j].rgbtGreen;
            uint8_t blue  = image[i][j].rgbtBlue;
            
            uint8_t average = round((red + green + blue) / 3.0);

            // Write new values
            image[i][j].rgbtRed   = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtBlue  = average;

            // Debug:
            // printf("R:%02x G:%02x B:%02x\n", image[i][j].rgbtRed, image[i][j].rgbtGreen, image[i][j].rgbtBlue);
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // sepiaRed   = .393 * originalRed + .769 * originalGreen + .189 * originalBlue
    // sepiaGreen = .349 * originalRed + .686 * originalGreen + .168 * originalBlue
    // sepiaBlue  = .272 * originalRed + .534 * originalGreen + .131 * originalBlue

    for (int i = 0; i < height; i++) // row
    {
        for (int j = 0; j < width; j++) // column
        {
            float red   = image[i][j].rgbtRed;
            float green = image[i][j].rgbtGreen;
            float blue  = image[i][j].rgbtBlue;
            
            int new_red = round(.393 * red + .769 * green + .189 * blue);
            if (new_red > 255) 
            { 
                new_red = 255; 
            }

            int new_green = round(.349 * red + .686 * green + .168 * blue);
            if (new_green > 255) 
            { 
                new_green = 255; 
            }

            int new_blue  = round(.272 * red + .534 * green + .131 * blue);
            if (new_blue > 255) 
            {
                new_blue = 255; 
            }
            
            // Write new values
            image[i][j].rgbtRed   = new_red;
            image[i][j].rgbtGreen = new_green;
            image[i][j].rgbtBlue  = new_blue;

            // Debug:
            // if (image[i][j].rgbtBlue > 200)
            // {
            //     printf("Height %i, Width %i:\nOriginal: \tR:%02x G:%02x B:%02x\nNew: \t\tR:%02x G:%02x B:%02x\n", i, j, red, green, blue, image[i][j].rgbtRed, image[i][j].rgbtGreen, image[i][j].rgbtBlue);
            // }
        }
    }

    return;
}

// Helper func to reverse rows
void ReverseArray(RGBTRIPLE arr[], int size)
{
    for (int i = 0; i < size/2; i++)
    {
        RGBTRIPLE temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Invert rows
    for (int i = 0; i < height; i++) // row
    {
        ReverseArray(image[i], width);
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Using box blur, averaging 3x3 grid for each pixel

    // Create shadow grid to store values
    RGBTRIPLE shadow_grid[height][width];  
    for (int i = 0; i < height; i++) // row
    {
        for (int j = 0; j < width; j++) // column
        {
            // Define neighbours and sum averages
            int average_red   = 0;
            int average_green = 0;
            int average_blue  = 0;
            float total = 1.0;

            // Upper row
            if (i != 0) 
            {
                // UP LEFT
                if (j != 0)
                {
                    average_red   += image[i - 1][j - 1].rgbtRed;
                    average_green += image[i - 1][j - 1].rgbtGreen;
                    average_blue  += image[i - 1][j - 1].rgbtBlue; 
                    total++;
                }

                // UP RIGHT
                if (j != width)
                {
                    average_red   += image[i - 1][j + 1].rgbtRed;
                    average_green += image[i - 1][j + 1].rgbtGreen;
                    average_blue  += image[i - 1][j + 1].rgbtBlue; 
                    total++;
                }

                // UP
                average_red   += image[i - 1][j].rgbtRed;
                average_green += image[i - 1][j].rgbtGreen;
                average_blue  += image[i - 1][j].rgbtBlue;
                total++;
            }

            // Center row
            // LEFT
            if (j != 0)
            {
                average_red   += image[i][j - 1].rgbtRed;
                average_green += image[i][j - 1].rgbtGreen;
                average_blue  += image[i][j - 1].rgbtBlue;
                total++;
            }

            // CENTER (itself) (always true)
            average_red   += image[i][j].rgbtRed;
            average_green += image[i][j].rgbtGreen;
            average_blue  += image[i][j].rgbtBlue;            
                
            // RIGHT
            if (j != width)
            {
                average_red   += image[i][j + 1].rgbtRed;
                average_green += image[i][j + 1].rgbtGreen;
                average_blue  += image[i][j + 1].rgbtBlue;
                total++;
            }

            // Lower row
            if (i != height)
            {
                // DOWN LEFT
                if (j != 0)
                {
                    average_red   += image[i + 1][j - 1].rgbtRed;
                    average_green += image[i + 1][j - 1].rgbtGreen;
                    average_blue  += image[i + 1][j - 1].rgbtBlue;
                    total++;
                }
                
                // DOWN RIGHT
                if (j != width)
                {
                    average_red   += image[i + 1][j + 1].rgbtRed;
                    average_green += image[i + 1][j + 1].rgbtGreen;
                    average_blue  += image[i + 1][j + 1].rgbtBlue;
                    total++;
                }

                // DOWN
                average_red   += image[i + 1][j].rgbtRed;
                average_green += image[i + 1][j].rgbtGreen;
                average_blue  += image[i + 1][j].rgbtBlue;
                total++;
            }

            // Apply blur effect to the shadow image
            shadow_grid[i][j].rgbtRed   = round(average_red   / total);
            shadow_grid[i][j].rgbtGreen = round(average_green / total);
            shadow_grid[i][j].rgbtBlue  = round(average_blue  / total);
        }
    }

    // Copy shadow grid to original grid
    for (int i = 0; i < height; i++) // row
    {
        for (int j = 0; j < width; j++) // column
        {
            image[i][j].rgbtRed   = shadow_grid[i][j].rgbtRed;
            image[i][j].rgbtGreen = shadow_grid[i][j].rgbtGreen;
            image[i][j].rgbtBlue  = shadow_grid[i][j].rgbtBlue;
        }
    }

    return;
}