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

void arr2_print(int *arr, int rows, int cols)
{
	int i, j;
	int id = 0;
	
	for (i = 0; i < rows; i++)
	{
		printf("[");
		for (j = 0; j < cols - 1; j++)
		{
			printf("%3d, ", arr[id]);
			id++;
		}
		printf("%3d]\n", arr[id]);
		id++;
	}
}