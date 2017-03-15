/*
  Automata functions
  
Things to do:
- link to python script
*/

#include <stdio.h>
#include <unistd.h>
#include "arrays.h"
#include "random.h"

void iterate(int *array, int N, int steps);
void step(int *array, int *mask, int N, double *probs);
int *order_neighbours(int *array, int N, int i, int j, int k);
int within(int N, int i, int j);

void automata_print(int *array, int N);
char *rep(int value);
void type_count(int *array, int N, int *output);
