/*
  Automata functions
  
Things to do:
- link to python script
*/

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <gsl/gsl_rng.h>
#include "arrays.h"

/* model parameter struct */
typedef struct {
	double probs[5];
	int competition;
	double alpha;
	double beta;
} Params;

extern const Params params_default;

/* model function pointer */
/* a pointer to function that takes an integer pointer, an integer 
   and a struct type Params, returns void */
typedef void (*modelPtr)(int *, int, Params);

/* automata pdf calculating functions */
void pdf(double *output, int N, int c_cells, int steps, int runs, modelPtr model, Params params);
void pdf_rolling(double *output, int N, int c_cells, int init_steps, int samples, int sample_gap, int runs, modelPtr model, Params params);

/* automata iteration functions */
void iterate(int *array, int N, int steps, modelPtr model, Params params, int *out_counts);
void iterate_endcount(int *array, int N, int steps, modelPtr model, Params params, int *out_counts);

/* state properties functions */
void type_count(int *array, int N, int *output);
void init_state(int *array, int N, int m);

/* automata iteration functions */
void model_simple(int *array, int N, Params params);
void model_extend(int *array, int N, Params params);
double cell_density(int *array, int N, int i, int j, int cell_type);
void proliferate(int *array, int N, int i, int j, double k1, int competition);
int *order_neighbours(int *array, int N, int i, int j, int k);
int within(int N, int i, int j);

/* display functions */
void iterate_display(int *array, int N, int steps, modelPtr model, Params params, int time_delay);
void automata_print(int *array, int N);
char *rep(int value);

/* random number generator handling */
int rng_initialize(int seed);
void rng_free(int rng_own);
