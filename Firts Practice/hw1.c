//NIMA BAHRAMI
// these are the ONLY library header files that can be used. UNDER NO
// CIRCUMSTANCES can additional library headers be included!
#include <stdio.h>	// I/O functions
#include <stdlib.h> 	// atof() function
#include <math.h>	// math functions (eg. sin(), cos(), sqrt())
#include "utils.h"	// user defined functions, constants, and data types
// ----------------------------------------------
// define constants
// define constants
#define PI 3.1415926535  // constant Pi
#define TOTAL_LINES 10   // constant that defines the total number
                         // of line_t values that can be handled

// -----------------------------------
int main(int argc, char *argv[]) {
  int linesRead;
  line_t *line_arr;
  
  // dynamic memory for array
  line_arr = (line_t*) malloc(TOTAL_LINES * sizeof(line_t));

  if (argc != 2) {
    printf("Usage: ./hw1 lines.txt\n");
    return -1;
  }
  
  //read the file
  linesRead = read_lines(argv[1], line_arr, TOTAL_LINES);
  if (linesRead == -1) {
    printf("Error reading file, please try again\n");
    return -1;
  }
  
  // 5. calculate direction and magnitude for each line
  for (int i = 0; i < linesRead; i++) {
    double direction = calc_direction(&line_arr[i]);
    double magnitude = calc_magnitude(&line_arr[i]);
   //directionin degree
    double directionInDegree = direction * 180 / PI;
    double x0 = line_arr[i].p0.x;
    double y0 = line_arr[i].p0.y;
    double x1 = line_arr[i].p1.x;
    double y1 = line_arr[i].p1.y;
    printf("Line %d: magnitude = %.2f, direction in radian = %.2f , direction in Degree = %.2f , value of p0( %.2f , %.2f), vlaue of p1( %.2f , %.2f )\n", i+1, magnitude, direction, directionInDegree ,x0, y0, x1, y1);
   
  }
  
  //free dynamic memory
  free(line_arr);
  
  return 0;
}



 