#include "helpers.h"
#include <math.h>
#include <getopt.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double K = ((double)image[i][j].rgbtRed + (double)image[i][j].rgbtGreen + (double)image[i][j].rgbtBlue) / 3;
            image[i][j].rgbtRed = round(K);
            image[i][j].rgbtGreen = round(K);
            image[i][j].rgbtBlue = round(K);
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
            image[i][j].rgbtRed = image[i][j].rgbtRed + image[i][width - 1 - j].rgbtRed;
            image[i][width - 1 - j].rgbtRed = image[i][j].rgbtRed - image[i][width - 1 - j].rgbtRed;
            image[i][j].rgbtRed = image[i][j].rgbtRed - image[i][width - 1 - j].rgbtRed;
            image[i][j].rgbtGreen = image[i][j].rgbtGreen + image[i][width - 1 - j].rgbtGreen;
            image[i][width - 1 - j].rgbtGreen = image[i][j].rgbtGreen - image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtGreen = image[i][j].rgbtGreen - image[i][width - 1 - j].rgbtGreen;
            image[i][j].rgbtBlue = image[i][j].rgbtBlue + image[i][width - 1 - j].rgbtBlue;
            image[i][width - 1 - j].rgbtBlue = image[i][j].rgbtBlue - image[i][width - 1 - j].rgbtBlue;
            image[i][j].rgbtBlue = image[i][j].rgbtBlue - image[i][width - 1 - j].rgbtBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE new[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int K = 0, RED = 0, GREEN = 0, BLUE = 0;

            for (int row = fmax(0, i - 1); row <= fmin(width - 1, i + 1); row++)
            {
                for (int column = fmax(0, j - 1); column <= fmin(height - 1, j + 1); column++)
                {
                    RED = RED + image[row][column].rgbtRed;
                    GREEN = GREEN + image[row][column].rgbtGreen;
                    BLUE = BLUE + image[row][column].rgbtBlue;
                    K++;
                }
            }

            new[i][j].rgbtRed = round((float)RED / K);
            new[i][j].rgbtGreen = round((float)GREEN / K);
            new[i][j].rgbtBlue = round((float)BLUE / K);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = new[i][j].rgbtRed;
            image[i][j].rgbtGreen = new[i][j].rgbtGreen;
            image[i][j].rgbtBlue = new[i][j].rgbtBlue;

        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE new[height + 2][width + 2];
    int Gx, Gy;
    for (int j = 0; j <= width + 1; j++)
    {
        new[0][j].rgbtRed = 0;
        new[0][j].rgbtGreen = 0;
        new[0][j].rgbtBlue = 0;
        new[height + 1][j].rgbtRed = 0;
        new[height + 1][j].rgbtGreen = 0;
        new[height + 1][j].rgbtBlue = 0;
    }
    for (int i = 0; i <= height + 1; i++)
    {
        new[i][0].rgbtRed = 0;
        new[i][0].rgbtGreen = 0;
        new[i][0].rgbtBlue = 0;
        new[i][width + 1].rgbtRed = 0;
        new[i][width + 1].rgbtGreen = 0;
        new[i][width + 1].rgbtBlue = 0;
    }
    for (int i = 1; i <= height; i++)
    {
        for (int j = 1; j <= width; j++)
        {
            new[i][j].rgbtRed = image[i - 1][j - 1].rgbtRed;
            new[i][j].rgbtGreen = image[i - 1][j - 1].rgbtGreen;
            new[i][j].rgbtBlue = image[i - 1][j - 1].rgbtBlue;
        }
    }
    for (int i = 1; i <= height; i++)
    {
        for (int j = 1; j <= width; j++)
        {
            Gx = new[i - 1][j - 1].rgbtRed * (-1) + new[i - 1][j + 1].rgbtRed + new[i][j - 1].rgbtRed * (-2) + new[i][j + 1].rgbtRed * 2 +
            new[i + 1][j - 1].rgbtRed * (-1) + new[i + 1][j + 1].rgbtRed;
            Gy = new[i - 1][j - 1].rgbtRed * (-1) + new[i - 1][j].rgbtRed * (-2) + new[i - 1][j + 1].rgbtRed * (-1) +
            new[i + 1][j - 1].rgbtRed + new[i + 1][j + 1].rgbtRed + new[i + 1][j].rgbtRed * (2);
            if (round(sqrt(Gx * Gx + Gy * Gy)) > 255)
            {
                image[i - 1][j - 1].rgbtRed = 255;
            }
            else
            {
                image[i - 1][j - 1].rgbtRed = round(sqrt(Gx * Gx + Gy * Gy));
            }
            Gx = new[i - 1][j - 1].rgbtGreen * (-1) + new[i - 1][j + 1].rgbtGreen + new[i][j - 1].rgbtGreen * (-2) +
            new[i][j + 1].rgbtGreen * 2 + new[i + 1][j - 1].rgbtGreen * (-1) + new[i + 1][j + 1].rgbtGreen;
            Gy = new[i - 1][j - 1].rgbtGreen * (-1) + new[i - 1][j].rgbtGreen * (-2) + new[i - 1][j + 1].rgbtGreen * (-1) +
            new[i + 1][j - 1].rgbtGreen + new[i + 1][j + 1].rgbtGreen + new[i + 1][j].rgbtGreen * (2);
            if (round(sqrt(Gx * Gx + Gy * Gy)) > 255)
            {
                image[i - 1][j - 1].rgbtGreen = 255;
            }
            else
            {
                image[i - 1][j - 1].rgbtGreen = round(sqrt(Gx * Gx + Gy * Gy));
            }
            Gx = new[i - 1][j - 1].rgbtBlue * (-1) + new[i - 1][j + 1].rgbtBlue + new[i][j - 1].rgbtBlue * (-2) + new[i][j + 1].rgbtBlue * 2 +
            new[i + 1][j - 1].rgbtBlue * (-1) + new[i + 1][j + 1].rgbtBlue;
            Gy = new[i - 1][j - 1].rgbtBlue * (-1) + new[i - 1][j].rgbtBlue * (-2) + new[i - 1][j + 1].rgbtBlue * (-1) +
            new[i + 1][j - 1].rgbtBlue + new[i + 1][j + 1].rgbtBlue + new[i + 1][j].rgbtBlue * (2);
            if (round(sqrt(Gx * Gx + Gy * Gy)) > 255)
            {
                image[i - 1][j - 1].rgbtBlue = 255;
            }
            else
            {
                image[i - 1][j - 1].rgbtBlue = round(sqrt(Gx * Gx + Gy * Gy));
            }
        }
    }
    return;
}
