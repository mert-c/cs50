#include "helpers.h"
#include <math.h>
#include <stdio.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg = round((image[i][j].rgbtRed+image[i][j].rgbtGreen+image[i][j].rgbtBlue)/(float)3);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width/2; j++)
        {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width-j-1];
            image[i][width-j-1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int sumB;
    int sumG;
    int sumR;
    float counter;

    //create a temporary table of colors to not alter the calculations
    RGBTRIPLE temp[height][width];

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            sumB = 0;
            sumG = 0;
            sumR = 0;
            counter = 0.00;

            // sums values of the pixel and 8 neighboring ones, skips iteration if it goes outside the pic
            for (int k = -1; k < 2; k++)
            {
                if (j + k < 0 || j + k > height - 1)
                {
                    continue;
                }

                for (int h = -1; h < 2; h++)
                {
                    if (i + h < 0 || i + h > width - 1)
                    {
                        continue;
                    }

                    sumB += image[j + k][i + h].rgbtBlue;
                    sumG += image[j + k][i + h].rgbtGreen;
                    sumR += image[j + k][i + h].rgbtRed;
                    counter++;
                }
            }

            temp[j][i].rgbtBlue = round(sumB / (float)counter);
            temp[j][i].rgbtGreen = round(sumG / (float)counter);
            temp[j][i].rgbtRed = round(sumR / (float)counter);
        }
    }

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            image[j][i].rgbtBlue = temp[j][i].rgbtBlue;
            image[j][i].rgbtGreen = temp[j][i].rgbtGreen;
            image[j][i].rgbtRed = temp[j][i].rgbtRed;
        }
    }
    return;
}


// Detect edges
RGBTRIPLE getimagevalue(int height, int width, RGBTRIPLE image[height][width], int i, int j, int l, int m)
{
    RGBTRIPLE black;
    black.rgbtRed = 255;
    black.rgbtGreen = 255;
    black.rgbtBlue = 255;

    if (j + l - 1 < 0 || j + l - 1> height)
    {
        return black;
    }
    if (i + m - 1 < 0 || i + m - 1 > width)
    {
        return black;
    }

    return image[j + l - 1][i + m - 1];
}


// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

// loop through each row and each pixel of the image
for (int i = 0; i < height; i++)
{
    for (int j = 0; j < width; j++)
    {
        int redx = 0;
        int greenx = 0;
        int bluex = 0;
        int redy = 0;
        int greeny = 0;
        int bluey = 0;

        // loop through the pixels 1 row and 1 column from the pixel
        for (int k = 0; k < 3; k++)
        {
            for (int l = 0; l < 3; l++)
            {
                // consider pixels in edges
                if (i - 1 + k < 0 || i - 1 + k > height - 1 || j - 1 + l < 0 || j - 1 + l > width - 1)
                {
                    continue;
                }

                // calcuate Gx for each color
                redx = redx + (image[i - 1 + k][j - 1 + l].rgbtRed * Gx[k][l]);
                greenx = greenx + (image[i - 1 + k][j - 1 + l].rgbtGreen * Gx[k][l]);
                bluex = bluex + (image[i - 1 + k][j - 1 + l].rgbtBlue * Gx[k][l]);

                // calculate Gy for each color
                redy = redy + (image[i - 1 + k][j - 1 + l].rgbtRed * Gy[k][l]);
                greeny = greeny + (image[i - 1 + k][j - 1 + l].rgbtGreen * Gy[k][l]);
                bluey = bluey + (image[i - 1 + k][j - 1 + l].rgbtBlue * Gy[k][l]);
            }
        }
        // calculate square root of Gx2 + Gy2 for each color
        float r = sqrt((redx * redx) + (redy * redy));
        float g = sqrt((greenx * greenx) + (greeny * greeny));
        float b = sqrt((bluex * bluex) + (bluey * bluey));

        // round to nearest integer and cap at 255
        int red =  round(r);
        int green = round(g);
        int blue = round(b);

        if (red > 255)
            red = 255;
        if (green > 255)
            green = 255;
        if (blue > 255)
            blue = 255;

        // replace color values with new value
        image[i][j].rgbtRed = red;
        image[i][j].rgbtBlue = blue;
        image[i][j].rgbtGreen = green;
        }
    }
    return;
}
