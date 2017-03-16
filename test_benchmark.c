#include "automata.h"
#include "arrays.h"
#include "random.h"


int main() 
{
	int i, j, N, *arr; 
	int types[4]; 
	double probs[] = {0.10, 0.60, 0.05, 0.05, 0.05};
	int out_counts[4];
	
	N = 10000; 
	
	/* allocate 2D array & initialize values */
	arr = arr_alloc(N * N);
	
	for (i = 0; i < N; i++)
	{
		for (j = 0; j <  N; j++)
		{
			arr[N * i + j] = randint(0, 3);
		}
	}
	printf("foo\n");
	//arr2_print(arr, rows , cols);
	
	iterate_endcount(arr, N, 5, probs, out_counts);
	
	//automata_print(arr, rows);
	type_count(arr, N, types);
	
	for (i = 0; i < 4; i++)
	{
		printf("%d\n", types[i]);
	}
	
	arr_free(arr);
}