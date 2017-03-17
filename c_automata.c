/*
 Functions for advancing the state of the automata
 */

#include "c_automata.h"

#define TEMP_CANCER 111


/* ------------------------------------------------------------------------------------- */
/* automata pdf calculating functions */
/* ------------------------------------------------------------------------------------- */

/*
pdf : calculate pdf of final number of cancer cells in automata after 
	  'runs' iterations

args :
	output  : pdf of number of cancer cells in final automata state
			  i.e. pdf(# of cancer cells = i) -> output[i]
		 	  (output must be an array of length N * N)
	N 	    : side length of automata (N >= 1)
	c_cells : number of cancer cells in initial automata state (c_cells >= 0)
	steps   : steps in each automata simulation
	runs 	: number of runs used to generate pdf distribution (runs >= 0)
	probs   : transition probabilities of automata
*/
void pdf(double *output, int N, int c_cells, int steps, int runs, double *probs)
{
	int i, *arr, *temp_output;
	int types[4];
	
	arr = arr_alloc(N * N);  /* allocate memory for automata state */
	temp_output = arr_alloc(N * N);  /* allocate memory for counting occurences of x_c values */
	
	/* simulate automata systems */
	for (i = 0; i < runs; i++)
	{
		init_state(arr, N, c_cells);
		iterate_endcount(arr, N, steps, probs, types);
		
		temp_output[types[1]]++;
	}
	
	/* divide by the number of runs to get pdf*/
	for (i = 0; i < N * N; i++)
	{
		output[i] = (double) temp_output[i] / (double) runs;
	}
	
	arr_free(arr);
	arr_free(temp_output);
}


/* ------------------------------------------------------------------------------------- */
/* automata iteration functions */
/* ------------------------------------------------------------------------------------- */

/*
iterate : evolve the state of the automata by applying iteration rules
		  a "steps" number of times
args :
	array : initial automata state (integer array of length N x N)
	N     : side length of automata
	steps : number of iterations to perform
	probs : transition probabilities of cellular automata

returns :
	array : used for computation, so the final state of the system
			remains in this variable
	out_counts : sums of each cell state kind (N, C, E, ...) at each
				 step of automata evolution
				 (must be integer array of length (steps x 4 (# of states)))
*/
void iterate(int *array, int N, int steps, double *probs, int *out_counts)
{
	int i;
	
	rand_init(-1);  /* initialize random number generator */
	
	for (i = 0; i < steps; i++)
	{
		step(array, N, probs);  /* apply automata iteration rules */
		type_count(array, N, &(out_counts[i * 4]));  /* count the number of cells for each type */
	}
}


/*
iterate_endcount : identical to iterate except that counting of cell states
				   occurs only for the final automata state.
args :
	array : initial automata state (integer array of length N x N)
	N     : side length of automata
	steps : number of iterations to perform
	probs : transition probabilities of cellular automata

returns :
	array : used for computation, so the final state of the system
			remains in this variable
	out_counts : sums of each cell state kind (N, C, E, ...) at final
				 step of automata evolution
				 (must be integer array of length 4 (# of states)
*/
void iterate_endcount(int *array, int N, int steps, double *probs, int *out_counts)
{
	int i;
	
	rand_init(-1);  /* initialize random number generator */
	
	for (i = 0; i < steps; i++)
	{
		step(array, N, probs);  /* apply automata iteration rules */
	}

	type_count(array, N, out_counts);  /* count the number of cells for each type */
}


/*
init_state : initializes the state of the automata by randomly distributing 
			 m cancer cells

args : 
	array : empty array of length N x N
	N 	  : side length of automata
	m	  : number of cancer cells to populate the state of
			automata with
			(the rest of cells will be normal)

returns :
	array : state of automata
*/
void init_state(int *array, int N, int m)
{
	int i, x, y;
	int placed = 0;
	
	rand_init(-1);
	
	/* initialize array to zero */
	for (i = 0; i < N * N; i++)
	{
		array[i] = 0;
	}
	
	while (placed < m)
	{
		x = randint(0, N - 1);
		y = randint(0, N - 1);
		
		if (array[N * x + y] != 1)
		{
			array[N * x + y] = 1;
			placed++;
		}
		
	}
}



/* ------------------------------------------------------------------------------------- */
/* automata iteration functions */
/* ------------------------------------------------------------------------------------- */

/*
step : apply the automata rules to the state of the automata
args:
	array : automata state
	N	  : side length of automata
	prob  : transition probabilities {k0, k1, k2, k3, k4} of
			automata states.
*/
void step(int *array, int N, double *probs)
{
	int i, j, id, *p;
	double r;
	
	/* apply automata rules */
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
				proliferate(array, N, i, j, probs[1]); /* mask original cancer cell for invasion */
				if (r < probs[2]) /* C -> E :: EFFECTION */
				{
					*p = 2; /* set to E */
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
	
	id = 0;
	while (id < N * N)
	{
		if (array[id] == TEMP_CANCER)
		{
			array[id] = 1;
		}
		id++;
	}
}


void proliferate(int *array, int N, int i, int j, double k1)
{
	int k, rr, *p;
	int neigh_c = 0;
	int neigh_n = 0;
	int *norm_neighbours[4];
	double r, k1_prime;
	
	/* count numbers of normal & cancer cells */
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
			norm_neighbours[neigh_n] = p;
			neigh_n++;
		}
		else if (*p == 1 || *p == TEMP_CANCER)
		{
			neigh_c++;
		}
	}
	
	/* decide if cancer proliferates */
	r = randdouble();
	k1_prime = k1 * (1 -  ((double) neigh_c) / 4.00);
	
	if (r < k1_prime && neigh_n > 0) /* proliferate */
	{
		/* choose a normal cell to invade */
		rr = randint(0, neigh_n - 1);
		/*printf("%d --> %d\n", *(norm_neighbours[rr]), TEMP_CANCER);*/
		*(norm_neighbours[rr]) = TEMP_CANCER;
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

/* ------------------------------------------------------------------------------------- */
/* display functions */
/* ------------------------------------------------------------------------------------- */

/*
iterate_display : step automata through "steps" iterations printing
				  the state of the system to stdout
args :
	array : initial automata state
	N     : side length of automata
	steps : number of iterations to perform
	probs : transition probabilities of cellular automata
	time_delay : number of milli-seconds between each state output
				 (default: 40,000 -- set to zero to use default)
*/
void iterate_display(int *array, int N, int steps, double *probs, int time_delay)
{
	int i;
	int dummy_count[4];
	
	/* Handle default for time_delay */
	if (time_delay == 0)
	{
		time_delay = 400000;
	}
	else
	{
		assert(time_delay >= 0);
	}

	/* Iterate and print through automata states */
	for (i = 0; i < steps; i++)
	{
		automata_print(array, N);
		usleep(time_delay);
		iterate(array, N, 1, probs, dummy_count);
	}
	automata_print(array, N);
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
	
	/* Head Boundary */
	printf(" ");
	for (i = 0; i < N; i++)
	{
		printf("__");
	}
	printf(" \n");
	
	/* Automata Grid */
	id = 0;
	for (i = 0; i < N; i++)
	{
		printf("|");
		for (j = 0; j < N; j++)
		{
			printf("%s ", rep(arr[id]));
			id++;
		}
		printf("|\n");
	}
	
	/* Tail Boundary */
	printf(" ");
	for (i = 0; i < N; i++)
	{
		printf("‾‾");
	}
	printf(" \n");
}


/*
rep : returns the representation character for a cell of the automata

args:
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


/*
type_count : count the number of each cell type in the automata

args :
	array 	: automata state
	N 		: side length of automata

returns :
	output  : integer array with the number of each type of cell
			  in automata state

Notes :
	the cell types are numbered
		0 -> N, 1 -> C, 2 -> E, 3 -> D
	and have sums located at the respective index of output
	eg. # of E cells = output[2]
*/
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
