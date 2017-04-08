/*
  Automata functions
  
Things to do:
- link to python script
*/

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <assert.h>
#include <gsl/gsl_rng.h>
#include "arrays.h"

/* model parameter struct */
typedef struct _m_params {
	double probs[5];
	int competition;
} m_params;

extern const m_params m_params_default;

/* model function pointer */
/* a pointer to function that takes an integer pointer, an integer 
   and a struct type m_params, returns void */
typedef void (*modelPtr)(int *, int, m_params);

/* automata pdf calculating functions */
void pdf(double *output, int N, int c_cells, int steps, int runs, modelPtr model, m_params params);
void pdf_rolling(double *output, int N, int c_cells, int init_steps, int samples, int sample_gap, int runs, modelPtr model, m_params params);

/* automata iteration functions */
void iterate(int *array, int N, int steps, modelPtr model, m_params params, int *out_counts);
void iterate_endcount(int *array, int N, int steps, modelPtr model, m_params params, int *out_counts);

/* state properties functions */
void type_count(int *array, int N, int *output);
void init_state(int *array, int N, int m);

/* automata iteration functions */
void model_simple(int *array, int N, m_params params);
void proliferate(int *array, int N, int i, int j, double k1, int competition);
int *order_neighbours(int *array, int N, int i, int j, int k);
int within(int N, int i, int j);

/* display functions */
void iterate_display(int *array, int N, int steps, modelPtr model, m_params params, int time_delay);
void automata_print(int *array, int N);
char *rep(int value);

/* random number generator handling */
int rng_initialize(int seed);
void rng_free(int rng_own);
