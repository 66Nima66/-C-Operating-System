// -----------------------------------
//NIMA BAHRAMI
// CSCI 340 - Operating Systems I 
// Spring 2023 (Anthony Leclerc)
// utils.c implementation file
// Homework Assignment 1
// -----------------------------------
#include <stdio.h>	// standard I/O functions
#include <stdlib.h> 	// atof()
#include <math.h>	// math functions
#include "utils.h"	// function prototypes and data structures
#define MAX_NUM_POINTS 4
#define MAX_LEN 200 
// -------------------------------------------------

int read_lines(char* filename, line_t* line_arr, int n) {
// Step 1: open the file for reading
FILE* fp = fopen(filename, "r");
if (fp == NULL) {
printf("Error opening file, please make sure you Entered txtFile!\n");
return -1;
}

int linesRead = 0;
double temArray[MAX_NUM_POINTS]; // Array to store the values of the line
char stringInLine[MAX_LEN]; // Array to store the line

while (fgets(stringInLine, MAX_LEN, fp) != NULL && linesRead < n) {

  int j =0; 
  int k = 0;
  for ( int i = 0; stringInLine[i]  != '\0'; i++) {

    if (stringInLine[i] == ','){
      double tempDouble = atof( stringInLine + j); //convert to double
      j = i + 1 ; //store the double in array
      temArray[k] = tempDouble;
      k++;
    }

  }
    double tempDouble = atof( stringInLine + j); //convert to double
    temArray[k] = tempDouble;
    
//print temArray
// printf(" temArray[0] = %f temArray[1] = %f temArray[2] = %f temArray[3] = %f \n", temArray[0], temArray[1], temArray[2], temArray[3]);

line_arr[linesRead].p0.x = temArray[0]; // Store the first point in the line
line_arr[linesRead].p0.y = temArray[1]; // Store the first point in the line
line_arr[linesRead].p1.x = temArray[2]; // Store the second point in the line
line_arr[linesRead].p1.y = temArray[3]; // Store the second point in the line


linesRead++;

}

fclose(fp);
return linesRead;
}

double calc_direction(line_t* line_ptr) {
  
  double x0 = line_ptr->p0.x; // Get the x value of the first point
  double y0 = line_ptr->p0.y; // Get the y value of the first point
  double x1 = line_ptr->p1.x; // Get the x value of the second point
  double y1 = line_ptr->p1.y; // Get the y value of the second point
  double yDiference = y1 - y0; // Calculate the y difference
  double xDiference = x1 - x0; // Calculate the x difference
  double result = atan2(yDiference, xDiference); // atan2() function to calculate the direction
  return result;
}

double calc_magnitude(line_t* line_ptr) {

  double x0 = line_ptr -> p0.x; // Get the x value of the first point
  double y0 = line_ptr -> p0.y; // Get the y value of the first point
  double x1 = line_ptr -> p1.x; // Get the x value of the second point
  double y1 = line_ptr -> p1.y; // Get the y value of the second point
  double xDiference = x1 - x0; // Calculate the x difference
  double yDiference = y1 - y0; // Calculate the y difference
  double result = sqrt(xDiference * xDiference + yDiference * yDiference); // Calculate the magnitude
  return result;

}



