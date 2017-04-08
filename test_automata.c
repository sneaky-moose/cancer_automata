#include "c_automata.h"
#include "arrays.h"


int main() 
{
	int i, j, N, *arr;
	int types[4]; 
	Params params = { .probs = {0.01, 0.5, 0.3, 0.4, 0.3}, .competition = 1, .alpha = 3.0, .beta = 0.0};
	/*double probs[] = {0.00, 0.48, 0.1, 0.3, 0.1};*/
	
	N = 20;
	
	/* allocate 2D array & initialize values */
	arr = arr_alloc(N * N);
	
	init_state(arr, N, 10);
	/*
	for (i = 0; i < 3; i++)
	{
		for (j = 0; j < 3; j++)
		{
			arr[i * N + j] = 1;
		}
	}
	*/
	
	iterate_display(arr, N, 100, &model_extend, params, 1000000);
	
	automata_print(arr, N);
	
	type_count(arr, N, types);
	
	for (i = 0; i < 4; i++)
	{
		printf("%d\n", types[i]);
	}
	
	arr_free(arr);
}