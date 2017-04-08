/*
 Functions for advancing the state of the automata
 */

#include "c_automata.h"

#define T_NORMAL 0
#define T_CANCER 1
#define T_CANCER_TEMP 111
#define T_EFFECTOR 2
#define T_DEAD 3

/* gsl random number global */ 
/* 
   commit the sin of a global variable so that random number generator does not
   have to be initialized every time a random variable is required.
*/
int rng_initialized = 0;
gsl_rng * rng;


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
	competition : cancer cells compete for resources
*/
void pdf(double *output, int N, int c_cells, int steps, int runs, double *probs, int competition)
{
	int i, rng_own, *arr, *temp_output;
	int types[4];
	
	rng_own = rng_initialize(-1);
	
	arr = arr_alloc(N * N);  /* allocate memory for automata state */
	temp_output = arr_alloc(N * N);  /* allocate memory for counting occurences of x_c values */
	
	/* simulate automata systems */
	for (i = 0; i < runs; i++)
	{
		init_state(arr, N, c_cells);
		iterate_endcount(arr, N, steps, probs, competition, types);
		
		/* DEBUG */
		//automata_print(arr, N);
		//arr_print(types, 4);
		temp_output[types[1]]++;
	}
	
	/* divide by the number of runs to get pdf*/
	for (i = 0; i < N * N; i++)
	{
		output[i] = (double) temp_output[i] / (double) runs;
	}
	
	arr_free(arr);
	arr_free(temp_output);
	rng_free(rng_own);
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
	competition : cancer cells compete for resources

returns :
	array : used for computation, so the final state of the system
			remains in this variable
	out_counts : sums of each cell state kind (N, C, E, ...) at each
				 step of automata evolution
				 (must be integer array of length (steps x 4 (# of states)))
*/
void iterate(int *array, int N, int steps, double *probs, int competition, int *out_counts)
{
	int i, rng_own;
	
	rng_own = rng_initialize(-1);
	
	for (i = 0; i < steps; i++)
	{
		step(array, N, probs, competition);  /* apply automata iteration rules */
		type_count(array, N, &(out_counts[i * 4]));  /* count the number of cells for each type */
	}
	
	rng_free(rng_own);
}


/*
iterate_endcount : identical to iterate except that counting of cell states
				   occurs only for the final automata state.
args :
	array : initial automata state (integer array of length N x N)
	N     : side length of automata
	steps : number of iterations to perform
	probs : transition probabilities of cellular automata
	competition : cancer cells compete for resources

returns :
	array : used for computation, so the final state of the system
			remains in this variable
	out_counts : sums of each cell state kind (N, C, E, ...) at final
				 step of automata evolution
				 (must be integer array of length 4 (# of states)
*/
void iterate_endcount(int *array, int N, int steps, double *probs, int competition, int *out_counts)
{
	int i, rng_own;
	
	rng_own = rng_initialize(-1);
	
	for (i = 0; i < steps; i++)
	{
		step(array, N, probs, competition);  /* apply automata iteration rules */
	}

	type_count(array, N, out_counts);  /* count the number of cells for each type */
	
	rng_free(rng_own);
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
	int i, x, y, rng_own;
	int placed = 0;
	
	rng_own = rng_initialize(-1);
	
	/* initialize array to zero */
	for (i = 0; i < N * N; i++)
	{
		array[i] = 0;
	}
	
	while (placed < m)
	{
		x = gsl_rng_uniform_int(rng, N);
		y = gsl_rng_uniform_int(rng, N);
		
		if (array[N * x + y] != 1)
		{
			array[N * x + y] = 1;
			placed++;
		}
		
	}
	
	rng_free(rng_own);
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
	competition : cancer cells compete for resources
*/
void step(int *array, int N, double *probs, int competition)
{
	int i, j, id, *p;
	double r;
	
	/* apply automata rules */
	id = 0;
	for (i = 0; i < N; i++)
	{
		for (j = 0; j < N; j++)
		{
			r = gsl_rng_uniform(rng); /* generate random number */
			p = &array[id];           /* pointer to cell in array */
			
			if (*p == T_NORMAL && r < probs[0]) 	  /* N -> C :: MUTATION */
			{
				*p = T_CANCER; /* set to C */
			}
			else if (*p == T_CANCER)
			{
				proliferate(array, N, i, j, probs[1], competition);
				if (r < probs[2]) /* C -> E :: EFFECTION */
				{
					*p = T_EFFECTOR; /* set to E */
				}
			}
			else if (*p == T_EFFECTOR && r < probs[3]) /* E -> D :: DEATH */
			{
				*p = T_DEAD; /* set to D */
			}
			else if (*p == T_DEAD && r < probs[4]) /* D -> N :: REBIRTH */
			{
				*p = T_NORMAL; /* set to N */
			}
			
			id++;
		}
	}
	
	id = 0;
	while (id < N * N)
	{
		if (array[id] == T_CANCER_TEMP)
		{
			array[id] = T_CANCER;
		}
		id++;
	}
}


/*
proliferate : handle proliferation of C cells into neighbouring N cells
args:
	array : automata state
	N	  : side length of automata
	i, j  : coordinate position of proliferating cell
	k1    : proliferation rate
	competition : C cells compete for resources
		if True (1) then use modified prolif. probability formula
			k1_prime = k1 * (1 -  ((double) neigh_c) / 4.00);
		else use
			k1_prime = k2

*/
void proliferate(int *array, int N, int i, int j, double k1, int competition)
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
		
		if (p == NULL) /* not within automata */
		{
			continue;
		}	
		else if (*p == T_NORMAL) /* normal type neighbour */
		{
			norm_neighbours[neigh_n] = p;
			neigh_n++;
		}
		else if (*p == T_CANCER /*|| *p == T_CANCER_TEMP*/)
		{
			neigh_c++;
		}
	}
	
	/* compute proliferation probability */
	if (competition)
	{
		k1_prime = k1 * (1 -  ((double) neigh_c) / 4.00);
	}
	else
	{
		k1_prime = k1;
	}
	
	/* decide if cancer proliferates */
	r = gsl_rng_uniform(rng);
	if (r < k1_prime && neigh_n > 0) /* proliferate */
	{
		/* choose a normal cell to invade */
		rr = gsl_rng_uniform_int(rng, neigh_n);
		*(norm_neighbours[rr]) = T_CANCER_TEMP;
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
		iterate(array, N, 1, probs, 1, dummy_count);
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

int rng_initialize(int seed)
{
	const gsl_rng_type *T;
	
	if (rng_initialized == 0)
	{
		gsl_rng_env_setup();
		T = gsl_rng_default;
		rng = gsl_rng_alloc(T);
		
		if (seed >= 0)
		{
			gsl_rng_set(rng, (unsigned long int) seed);
		}
		else
		{
			gsl_rng_set(rng, (unsigned long int) time(NULL)); 
		}
		rng_initialized = 1;
		
		return 1;
	}
	else
	{
		return 0;
	}
}

void rng_free(int rng_own)
{
	if (rng_own == 1 && rng_initialized == 1)
	{
		gsl_rng_free(rng);
		rng_initialized = 0;
	}
	
}