#include "helpers.h"
#include <stdio.h>
#include <math.h>
#include <cs50.h>

//My Prototypes
void swap(RGBTRIPLE *x, RGBTRIPLE *y);
RGBTRIPLE pixelB(int i, int j, int height, int width, RGBTRIPLE image[height][width]);
bool validPixel(int i, int j, int height, int width);
int cap(int x);
RGBTRIPLE findEdges(int i, int j, int height, int width, RGBTRIPLE image[height][width]);


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //cycle through every pixel
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //get the average of the values, keep in mind float and rounding
            int average = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.00);

            //update the values to be grayscale
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = average;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    //cycle through all the pixels
    for (int i = 0; i < height; i++)
    {
        //only cycle through have the width because we'll be swaping them
        for (int j = 0; j < width / 2; j++)
        {
            //swap two pixels
            swap(&image[i][j], &image[i][width - 1 - j]);
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    //make a copy of the image
    RGBTRIPLE imageB[height][width];

    //loop through all the pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //swap each pixel with the blurred pixel
            imageB[i][j] = pixelB(i, j, height, width, image);
        }
    }

    //after all the pixel's are blurred based off the orginal image then
    for (int i = 0; i < height; i++)
    {
        //cycle through all the pixels
        for (int j = 0; j < width; j++)
        {

            //update every pixel to the blurred pixel
            image[i][j] = imageB[i][j];
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    //make a copy of the image
    RGBTRIPLE imageC[height][width];

    //Cycle through all the pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //Save all the edges to imageC
            imageC[i][j] = findEdges(i, j, height, width, image);
        }
    }

    //after all the pixel's are edged based off the orginal image then
    for (int i = 0; i < height; i++)
    {
        //cycle through all the pixels
        for (int j = 0; j < width; j++)
        {

            //update every pixel to the edge pixels
            image[i][j] = imageC[i][j];
        }
    }
}

//my functions

//Swap Two Pixels
void swap(RGBTRIPLE *x, RGBTRIPLE *y)
{
    RGBTRIPLE temp = *x;
    *x = *y;
    *y = temp;
}

//Get a blurred Pixel
RGBTRIPLE pixelB(int i, int j, int height, int width, RGBTRIPLE image[height][width])
{
    //define integers for each value
    int red, blue, green;

    //set integers to 0
    red = blue = green = 0;

    //create an interger to count the number of pixels
    int pixelcount = 0;

    //cycle through the pixel, starting with one before and going till on after on each row
    for (int di = -1; di <= 1; di++)
    {
        //and each column
        for (int dj = -1; dj <= 1; dj++)
        {
            //create a new integer ni and nj to store the new pixel locations
            int ni = i + di;
            int nj = j + dj;

            //test that each new pixel location is a valid Pixel
            if (validPixel(ni, nj, height, width))
            {
                //update the pixelcount
                pixelcount++;

                //add value to red
                red += image[ni][nj].rgbtRed;

                //add value to green
                green += image[ni][nj].rgbtGreen;

                //add value to blue
                blue += image[ni][nj].rgbtBlue;
            }
        }
    }

    //define a new RGBTRIPLE called blurry
    RGBTRIPLE blurry;

    //find average of red and update
    blurry.rgbtRed = round((float) red / pixelcount);

    //find average of green and update
    blurry.rgbtGreen = round((float) green / pixelcount);

    //find average of ble and update
    blurry.rgbtBlue = round((float) blue / pixelcount);

    //return the new RGBTRIPLE called blurry which now stores the blurry values
    return blurry;
}

bool validPixel(int i, int j, int height, int width)
{
    //Check if the pixel is outside the image or an edge case
    return i >= 0 && i < height && j >= 0 && j < width;
}

// If the value is over 255 cap the value at 255.
int cap(int x)
{
    //super cool if statement
    return x > 255 ? 255 : x;
}

//find the edge pixels.
RGBTRIPLE findEdges(int i, int j, int height, int width, RGBTRIPLE image[height][width])
{
    //set integars for all the colors
    int red, redGx, redGy, blue, blueGx, blueGy, green, greenGx, greenGy;
    red = redGx = redGy = green = greenGx = greenGy = blue = blueGx = blueGy = 0;

    //Set an array for sobels operator
    int Gx[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int Gy[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    //set an integar for C to track the correct sobel's operator
    int c = 0;

    //cylce through a 3x3 grid
    for (int di = -1; di <= 1; di++)
    {
        for (int dj = -1; dj <= 1; dj++)
        {
            //set new integars to keep track of the grid
            int ni = i + di;
            int nj = j + dj;

            //if it's a valid pixel
            if (validPixel(ni, nj, height, width))
            {
                //store the Gx and Gy for red
                redGx += Gx[c] * image[ni][nj].rgbtRed;
                redGy += Gy[c] * image[ni][nj].rgbtRed;

                //Green
                greenGx += Gx[c] * image[ni][nj].rgbtGreen;
                greenGy += Gy[c] * image[ni][nj].rgbtGreen;

                //And blue
                blueGx += Gx[c] * image[ni][nj].rgbtBlue;
                blueGy += Gy[c] * image[ni][nj].rgbtBlue;
            }
            else
            {
                //If any part of the grid is outside of the image record 0
                redGx += Gx[c] * 0;
                redGy += Gy[c] * 0;

                greenGx += Gx[c] * 0;
                greenGy += Gy[c] * 0;

                blueGx += Gx[c] * 0;
                blueGy += Gy[c] * 0;

            }
            //incease c by 1
            c++;
        }
    }
    //square root of Gx^2 + Gy^2
    red = cap(round(sqrt(pow(redGx, 2) + pow(redGy, 2))));
    green = cap(round(sqrt(pow(greenGx, 2) + pow(greenGy, 2))));
    blue = cap(round(sqrt(pow(blueGx, 2) + pow(blueGy, 2))));


    //Send back the adjusted pixels.
    RGBTRIPLE edgy;
    edgy.rgbtRed = red;
    edgy.rgbtGreen = green;
    edgy.rgbtBlue = blue;
    return edgy;
}
