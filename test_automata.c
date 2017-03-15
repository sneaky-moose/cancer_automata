#include "automata.h"
#include "arrays.h"
#include "random.h"


int main() 
{
	int i, j, rows, cols, *arr;
	int types[4];
	
	rows = cols = 10;
	
	/* initialize random number generator */
	rand_init(-1);
	
	/* allocate 2D array & initialize values */
	arr = arr_alloc(rows * cols);
	/*
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < cols; j++)
		{
			arr[i][j] = randint(0, 3);
		}
	}
	*/
	arr[5 * rows + 5] = 1;
	//arr2_print(arr, rows , cols);
	
	iterate(arr, rows, 200);
	
	//automata_print(arr, rows);
	type_count(arr, rows, types);
	
	for (i = 0; i < 4; i++)
	{
		printf("%d\n", types[i]);
	}
	
	arr_free(arr);
}