"""
_automata.pyx

Wrapping cython code for automata.c code
"""

import cython
import numpy as np
cimport numpy as np

cdef extern from "c_automata.h":
    void pdf(double *output, int N, int c_cells, int steps, int runs, double *probs, int competition)
    void init_state(int *array, int N, int m)
    void iterate(int *array, int N, int steps, double *probs, int competition, int *out_counts)
    void iterate_endcount(int *array, int N, int steps, double *probs, int competition, int *out_counts)
    void type_count(int *array, int N, int *output)


@cython.boundscheck(False)
@cython.wraparound(False)
def _init_state(int N, int m):
    cdef int[:, ::1] arr = np.zeros((N, N), dtype='i4')
    init_state(&arr[0, 0], N, m)
    return np.asarray(arr)
	

@cython.boundscheck(False)
@cython.wraparound(False)
def _pdf(int N, int c_cells, int steps, int runs, probs, competition=True):
    if len(probs) != 5:
        raise TypeError("Probability must be of length 5")
    
    cdef int comp = 1 if competition else 0
    
    cdef double[::1] _probs = np.asarray(probs, dtype=np.float);
    cdef double[::1] output = np.zeros(N ** 2, np.float)
    pdf(&output[0], N, c_cells, steps, runs, &_probs[0], comp)
    return np.asarray(output)


@cython.boundscheck(False)
@cython.wraparound(False)
def _iterate(int[:, ::1] arr not None, int steps, probs, competition=True):
    if len(probs) != 5:
        raise TypeError("Probability must be of length 5")
    
    cdef int comp = 1 if competition else 0
    
    cdef double[::1] _probs = np.asarray(probs, dtype=np.float);
    cdef int[:, ::1] out_counts = np.zeros((steps, 4), dtype='i4')
    iterate(&arr[0, 0], arr.shape[0], steps, &_probs[0], comp, &out_counts[0, 0])
    
    return np.asarray(out_counts)
