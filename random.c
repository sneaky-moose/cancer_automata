/*
 Utility for generating random numbers -- QUICK & DIRTY
*/

#include "random.h"


/*
	rand_int : initialize the pseudo random number generator seed
	args :
		seed : seed value used (if -1 unix time is used as the seed value)
*/
void rand_init(int seed)
{
	if (seed == -1)
	{
		srand(time(NULL));	
	}
	else
	{
		srand(seed);
	}
	rand();
}

double randdouble()
{
	return ((double) rand()) / ((double) RAND_MAX);
}


/*
	randint : returns random integer between 'low' and 'high' inclusive
*/
int randint(int low, int high)
{
	return floor(randdouble() * (high - low + 1)) + low;
}