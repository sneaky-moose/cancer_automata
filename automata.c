/*
 Functions for advancing the state of the automata
 */

#include "automata.h"

/*
iterate : step automata through "steps" iterations
args :
	array : initial automata state
	N     : side length of automata
	steps : number of iterations to perform
*/
void iterate(int *array, int N, int steps)
{
	int i, *mask;
	double probs[5] = {0.00, 0.60, 0.05, 0.05, 0.05};
	
	mask = arr_alloc(N * N); /* allocate mask for new cancer */
	
	for (i = 0; i < steps; i++)
	{
		automata_print(array, N);
		usleep(400000);
		step(array, mask, N, probs);
	}
	//automata_print(array, N);
	
	
	arr_free(mask);
}

/*
step : apply the automata rules to the state of the automata
args:
	array : automata state
	mask  : array of same size as the automata state, this is
			used to identify newly created cancer cells in the
			automata and prevent them from proliferating for 1
			automata step.
	N	  : side length of automata
	prob  : transition probabilities {k0, k1, k2, k3, k4} of
			automata states.
*/
void step(int *array, int *mask, int N, double *probs)
{
	int i, j, k, l, x, y, id, *p;
	int neigh_tup[2];
	int neigh_c, neigh_n;
	double r, k1_prime;
	
	
	/* STAGE 1 ---------------------------------------------------*/
	id = 0;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			r = randdouble(); /* generate random number */
			p = &array[id];
			
			if (*p == 0 && r < probs[0]) 	  /* N -> C :: MUTATION */
			{
				*p = 1; /* set to C */
			} 
			else if (*p == 1)
			{
				if (r < probs[2]) /* C -> E :: EFFECTION */
				{
					*p = 2; /* set to E */
				}
				else
				{
					mask[id] = 1; /* mask original cancer cell for invasion */
				}
			}
			else if (*p == 2 && r < probs[3]) /* E -> D :: DEATH */
			{
				*p = 3; /* set to D */
			}
			else if (*p == 3 && r < probs[4]) /* D -> N :: REBIRTH */
			{
				*p = 0; /* set to N */
			}
			
			id++;
		}
	}
	
	/* STAGE 2 ----------------------------------------------------*/
	id = 0;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			if (mask[id] == 1)
			{
				/* count numbers of normal & cancer cells */
				neigh_c = neigh_n = 0;
				for (k = 0; k < 4; k++)
				{
					/* get neighbour pointer */
					p = order_neighbours(array, N, i, j, k);
					
					if (p == NULL)
					{
						continue;
					}	
					else if (*p == 0)
					{
						neigh_n++;
					}
					else if (*p == 1)
					{
						neigh_c++;
					}
				}
				
				/* decide if cancer proliferates */
				r = randdouble();
				k1_prime = probs[1] * (1 -  ((double) neigh_c) / 4.00);
				
				if (r < k1_prime) /* proliferate */
				{
					/* choose a normal cell to invade */
					r = randint(0, neigh_n - 1);
					
					l = 0;
					for (k = 0; k < 4; k++)
					{
						/* get neighbour pointer */
						p = order_neighbours(array, N, i, j, k);
						if (p == NULL)
						{
							continue;
						}
						if (*p == 0)
						{
							if (r == l)
							{
								*p=1;
								break;
							}
							else
							{
								l++;
							}
						}
					}
				}
				mask[id] = 0;
			}
			id++;
		}
	}
}


int *order_neighbours(int *array, int N, int i, int j, int k)
{
	int x, y;
	
	switch (k)
	{
		case 0:
			x = i; y = j + 1;
			break;
		case 1:
			x = i + 1; y = j;
			break;
		case 2:
			x = i; y = j - 1;
			break;
		case 3:
			x = i - 1; y = j;
			break;
	}
	
	if (within(N, x, y))
	{
		return &(array[N * x + y]);
	}
	else
	{
		return NULL;
	}
}

int within(int N, int i, int j)
{
	return 0 <= i && i < N && 0 <= j && j < N;
}


/*
  automata_print : prints a representation of the automata to the console
  arguments :
	  array : state of automata as 2D array of integers
	  N 	: side length of automata
*/
void automata_print(int *arr, int N)
{
	int i, j, id;
	
	printf("\n\n\n");
	id = 0;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			printf("%s ", rep(arr[id]));
			id++;
		}
		printf("\n");
	}
}


/*
  rep : returns the representation character for a cell of the automata
  arguments :
		value : integer value of the cell state
  
  Notes:
	0 -> N  (Normal)
	1 -> C  (Cancerous)
`	2 -> E 	(Cytotoxic)
	3 -> D  (Dead)
*/
char *rep(int value)
{
	switch (value)
	{
		case 0: 
			return " "; break;
		case 1:
			return "●"; break;
		case 2:
			return "○"; break;
		case 3:
			return "▴"; break;
		default:
			return "*"; break;
	}
}


void type_count(int *array, int N, int *output)
{
	int i, j, id;
	
	/* initialize output array to zero */
	for (i = 0; i < 4; i++)
	{
		output[i] = 0;
	}
	
	/* iterate through array */
	id = 0;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			output[array[id]]++; /* count type */
			id++;
		}
	}
}
