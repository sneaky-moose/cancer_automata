/*
 Tools for creating dynamic arrays in C - QUICK AND DIRTY
*/

#include <stdio.h>
#include <stdlib.h>

/* functions for 1D case */
int *arr_alloc(int length);
void arr_free(int *arr);
void arr_print(int *arr, int length);

/* functions for 2D case */
int **arr2_alloc(int rows, int cols);
void arr2_free(int **array, int rows, int cols);
void arr2_print(int **array, int rows, int cols);