/*
 Utility for generating random numbers -- QUICK & DIRTY
*/

#include <stdlib.h>
#include <time.h>
#include <math.h>

void rand_init(int seed);
int randint(int low, int high);
double randdouble();