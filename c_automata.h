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


/* automata pdf calculating functions */
void pdf(double *output, int N, int c_cells, int steps, int runs, double *probs, int competition);

/* automata iteration functions */
void iterate(int *array, int N, int steps, double *probs, int competition, int *out_counts);
void iterate_endcount(int *array, int N, int steps, double *probs, int competition, int *out_counts);

/* state properties functions */
void type_count(int *array, int N, int *output);
void init_state(int *array, int N, int m);

/* automata iteration functions */
void step(int *array, int N, double *probs, int competition);
void proliferate(int *array, int N, int i, int j, double k1, int competition);
int *order_neighbours(int *array, int N, int i, int j, int k);
int within(int N, int i, int j);

/* display functions */
void iterate_display(int *array, int N, int steps, double *probs, int time_delay);
void automata_print(int *array, int N);
char *rep(int value);

/* random number generator handling */
int rng_initialize(int seed);
void rng_free(int rng_own);
