"""
_automata.pyx

Wrapping cython code for automata.c code
"""

import cython
import numpy as np
cimport numpy as np

cdef extern from "c_automata.h":
    void pdf(double *output, int N, int c_cells, int steps, int runs, double *probs)
    void init_state(int *array, int N, int m)
    void iterate(int *array, int N, int steps, double *probs, int *out_counts)
    void iterate_endcount(int *array, int N, int steps, double *probs, int *out_counts)
    void type_count(int *array, int N, int *output)


@cython.boundscheck(False)
@cython.wraparound(False)
def _init_state(int N, int m):
    cdef int[:, ::1] arr = np.zeros((N, N), dtype='i4')
    init_state(&arr[0, 0], N, m)
    return np.asarray(arr)
	

@cython.boundscheck(False)
@cython.wraparound(False)
def _pdf(int N, int c_cells, int steps, int runs, double[::1] probs not None):
    if len(probs) != 5:
        raise TypeError("Probability must be of length 4")
        
    cdef double[::1] output = np.zeros(N ** 2, np.float)
    pdf(&output[0], N, c_cells, steps, runs, &probs[0])
    return np.asarray(output)


@cython.boundscheck(False)
@cython.wraparound(False)
def _iterate(int[:, ::1] arr not None, int N, int steps, double[::1] probs not None):
    if len(probs) != 5:
        raise TypeError("Probability must be of length 4")
        
    cdef int[:, ::1] out_counts = np.zeros((steps, 4), dtype='i4')
    iterate(&arr[0, 0], N, steps, &probs[0], &out_counts[0, 0])
    
    return np.asarray(out_counts)
