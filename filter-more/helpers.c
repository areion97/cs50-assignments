#include "helpers.h"
#include <stdio.h>
#include <math.h>

// Convert image to grayscale
// we can take the average of the red, green, and blue values to determine what shade of grey to make the new pixel.
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int average = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            average = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen +  image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE imagecopy [height][width];

    int red_sum = 0;
    int green_sum = 0;
    int blue_sum = 0;
    float counter = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            counter = 0;
            red_sum = 0;
            green_sum = 0;
            blue_sum = 0;

            for (int k = i - 1 ; k < i + 2; k++)
            {
                if (k >= 0 && k < height)
                {
                    for (int l = j - 1 ; l < j + 2; l++)
                    {
                        if (l >= 0 && l < width)
                        {
                            counter++;
                            red_sum += image[k][l].rgbtRed;
                            green_sum += image[k][l].rgbtGreen;
                            blue_sum += image[k][l].rgbtBlue;
                        }
                    }
                }
            }


            if (red_sum != 0)
            {
                imagecopy[i][j].rgbtRed = round(red_sum / counter);
            }
            if (green_sum != 0)
            {
                imagecopy[i][j].rgbtGreen = round(green_sum / counter);
            }
            if (blue_sum != 0)
            {
                imagecopy[i][j].rgbtBlue = round(blue_sum / counter);
            }
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = imagecopy[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int xkernel[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int ykernel[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};
    RGBTRIPLE imagecopy [height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width ; j++)
        {
            int gxred = 0;
            int gyred = 0;
            int gxblue = 0;
            int gyblue = 0;
            int gxgreen = 0;
            int gygreen = 0;

            for (int k = i - 1, x = 0; k < i + 2; k++, x++)
            {
                if (k >= 0 && k < height)
                {
                    for (int l = j - 1, y = 0; l < j + 2; l++, y++)
                    {
                        if (l >= 0 && l < width)
                        {
                            gxred += image[k][l].rgbtRed * xkernel[x][y];
                            gyred += image[k][l].rgbtRed * ykernel[x][y];
                            gxblue += image[k][l].rgbtBlue * xkernel[x][y];
                            gyblue += image[k][l].rgbtBlue * ykernel[x][y];
                            gxgreen += image[k][l].rgbtGreen * xkernel[x][y];
                            gygreen += image[k][l].rgbtGreen * ykernel[x][y];
                        }
                    }
                }
            }
            int redValue = round(sqrt(gxred * gxred + gyred * gyred));
            if (redValue > 255)
            {
                redValue = 255;
            }

            int blueValue = round(sqrt(gxblue * gxblue + gyblue * gyblue));
            if (blueValue > 255)
            {
                blueValue = 255;
            }
            int greenValue = round(sqrt(gxgreen * gxgreen + gygreen * gygreen));
            if (greenValue > 255)
            {
                greenValue = 255;
            }

            imagecopy[i][j].rgbtRed = redValue;
            imagecopy[i][j].rgbtBlue = blueValue;
            imagecopy[i][j].rgbtGreen = greenValue;

        }

    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = imagecopy[i][j];
        }
    }

    return;
}
