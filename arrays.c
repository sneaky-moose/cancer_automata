/*
 Tools for creating dynamic arrays in C - QUICK AND DIRTY
*/

#include "arrays.h"


/* functions for 1D case */
int *arr_alloc(int length)
{
	int *arr;
	arr = (int *) calloc(length, sizeof(int));
	if (arr == NULL)
	{
		fprintf(stderr, "Out of memory!");
		exit(1);
	}
	return arr;
}

void arr_free(int *arr)
{
	free(arr);
}

void arr_print(int *arr, int length)
{
	int i;
	
	printf("[");
	for (i = 0; i < length - 1; i++)
	{
		printf("%d, ", arr[i]);
	}
	printf("%d]\n", arr[length - 1]);
}


/* functions for 2D case */
int **arr2_alloc(int rows, int cols)
{
	int i, **arr;
	
	/* allocate 2D array head */
	/* this will point to an array of pointers which point to the rows */
	arr = (int **) calloc(rows, sizeof(int *));
	if (arr == NULL)
	{
		fprintf(stderr, "Out of memory!");
		exit(1);
	}
	
	/* allocate the rows of the 2D array */
	for (i = 0; i < rows; i++)
	{
		arr[i] = (int *) calloc(cols, sizeof(int));
		if (arr[i] == NULL)
		{
			fprintf(stderr, "Out of memory!");
			exit(1);
		}
	}
	
	return arr;
}

void arr2_free(int **array, int rows, int cols)
{
	int i;
	for (i = 0; i < rows; i++)
	{
		free(array[i]);
	}
	free(array);
}

void arr2_print(int **array, int rows, int cols)
{
	int i, j;
	for (i = 0; i < rows; i++)
	{
		printf("[");
		for (j = 0; j < cols - 1; j++)
		{
			printf("%3d, ", array[i][j]);
		}
		printf("%3d]\n", array[i][cols-1]);
	}
}