#include "c_automata.h"
#include "arrays.h"

#define STEPS 200
#define STATES 4
#define M 3
/*
int main2() 
{
	int N, *arr, *out_counts;
	double probs[] = {0.00, 0.60, 0.05, 0.05, 0.05};
	
	N = 10;
	arr = arr_alloc(N * N);
	out_counts = arr_alloc(STEPS * STATES);
	
	init_state(arr, N, M);
	automata_print(arr, N);
	
	iterate(arr, N, STEPS, probs, 1, out_counts);
	arr2_print(out_counts, STEPS, STATES);
	
	
	arr_free(arr);
	arr_free(out_counts);
	return 0;
}

int main3()
{
	int N, *arr, *out_counts;
	double probs[] = {0.00, 0.60, 0.05, 0.05, 0.05};
	
	N = 100;
	arr = arr_alloc(N * N);
	out_counts = arr_alloc(STATES);
	
	init_state(arr, N, M);
	
	iterate_endcount(arr, N, STEPS, probs, 1, out_counts);
	arr_print(out_counts, STATES);
	
	arr_free(arr);
	arr_free(out_counts);
	return 0;
}
*/

int main()
{
	/*
	int i, N;
	double probs[] = {0.00, 0.60, 0.05, 0.05, 0.05};
	double output[100];
	
	N = 10;
	
	pdf(output, N, 5, 100, 10000, probs, 1);
	
	for (i = 0; i < N * N; i++)
	{
		printf("%f, \n", output[i]);
	}
	*/
	return 0;
}