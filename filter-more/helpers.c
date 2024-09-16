#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int average =
                round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
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
            int temp1 = image[i][j].rgbtBlue;
            int temp2 = image[i][j].rgbtGreen;
            int temp3 = image[i][j].rgbtRed;

            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;

            image[i][width - j - 1].rgbtBlue = temp1;
            image[i][width - j - 1].rgbtGreen = temp2;
            image[i][width - j - 1].rgbtRed = temp3;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int blurBlue[height][width];
    int blurGreen[height][width];
    int blurRed[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int count = 0;

            int sumBlue = 0;
            int sumGreen = 0;
            int sumRed = 0;

            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if (!(i + k < 0 || i + k > height - 1 || j + l < 0 || j + l > width - 1))
                    {
                        count++;
                        sumBlue += image[i + k][j + l].rgbtBlue;
                        sumGreen += image[i + k][j + l].rgbtGreen;
                        sumRed += image[i + k][j + l].rgbtRed;
                    }
                }
            }

            blurBlue[i][j] = round(sumBlue / (float) count);
            blurGreen[i][j] = round(sumGreen / (float) count);
            blurRed[i][j] = round(sumRed / (float) count);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = blurBlue[i][j];
            image[i][j].rgbtGreen = blurGreen[i][j];
            image[i][j].rgbtRed = blurRed[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int gxBlue[height][width];
    int gxGreen[height][width];
    int gxRed[height][width];
    int gyBlue[height][width];
    int gyGreen[height][width];
    int gyRed[height][width];

    int gxMatrix[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int gyMatrix[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int gxBlueValue = 0;
            int gxGreenValue = 0;
            int gxRedValue = 0;

            int gyBlueValue = 0;
            int gyGreenValue = 0;
            int gyRedValue = 0;

            for (int k = -1; k < 2; k++)
            {
                for (int l = -1; l < 2; l++)
                {
                    if (!(i + k < 0 || i + k > height - 1 || j + l < 0 || j + l > width - 1))
                    {
                        gxBlueValue += image[i + k][j + l].rgbtBlue * gxMatrix[k + 1][l + 1];
                        gxGreenValue += image[i + k][j + l].rgbtGreen * gxMatrix[k + 1][l + 1];
                        gxRedValue += image[i + k][j + l].rgbtRed * gxMatrix[k + 1][l + 1];

                        gyBlueValue += image[i + k][j + l].rgbtBlue * gyMatrix[k + 1][l + 1];
                        gyGreenValue += image[i + k][j + l].rgbtGreen * gyMatrix[k + 1][l + 1];
                        gyRedValue += image[i + k][j + l].rgbtRed * gyMatrix[k + 1][l + 1];
                    }
                }
            }

            gxBlue[i][j] = gxBlueValue;
            gxGreen[i][j] = gxGreenValue;
            gxRed[i][j] = gxRedValue;

            gyBlue[i][j] = gyBlueValue;
            gyGreen[i][j] = gyGreenValue;
            gyRed[i][j] = gyRedValue;
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (round(sqrt(pow(gxBlue[i][j], 2) + pow(gyBlue[i][j], 2))) <= 255)
            {
                image[i][j].rgbtBlue = round(sqrt(pow(gxBlue[i][j], 2) + pow(gyBlue[i][j], 2)));
            }
            else
            {
                image[i][j].rgbtBlue = 255;
            }

            if (round(sqrt(pow(gxGreen[i][j], 2) + pow(gyGreen[i][j], 2))) <= 255)
            {
                image[i][j].rgbtGreen = round(sqrt(pow(gxGreen[i][j], 2) + pow(gyGreen[i][j], 2)));
            }
            else
            {
                image[i][j].rgbtGreen = 255;
            }

            if (round(sqrt(pow(gxRed[i][j], 2) + pow(gyRed[i][j], 2))) <= 255)
            {
                image[i][j].rgbtRed = round(sqrt(pow(gxRed[i][j], 2) + pow(gyRed[i][j], 2)));
            }
            else
            {
                image[i][j].rgbtRed = 255;
            }
        }
    }

    return;
}
