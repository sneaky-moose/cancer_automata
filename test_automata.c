#include "automata.h"
#include "arrays.h"
#include "random.h"


int main() 
{
	int i, j, N, *arr;
	int types[4]; 
	double probs[] = {0.00, 0.60, 0.05, 0.05, 0.05};
	
	N = 20;
	
	/* allocate 2D array & initialize values */
	arr = arr_alloc(N * N);
	
	init_state(arr, N, 5); 
	
	iterate_display(arr, N, 100, probs, 0);
	
	automata_print(arr, N);
	
	
	type_count(arr, N, types);
	
	for (i = 0; i < 4; i++)
	{
		printf("%d\n", types[i]);
	}
	
	arr_free(arr);
}