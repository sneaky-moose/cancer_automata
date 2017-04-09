"""
_automata.pyx

Wrapping cython code for automata.c code
"""

import cython
import numpy as np
cimport numpy as np
cimport c_automata

@cython.boundscheck(False)
@cython.wraparound(False)
def init_state(int N, int m):
    cdef int[:, ::1] arr = np.zeros((N, N), dtype='i4')
    c_automata.init_state(&arr[0, 0], N, m)
    return np.asarray(arr)
	

@cython.boundscheck(False)
@cython.wraparound(False)
def pdf(int N, int c_cells, int steps, int runs, probs, competition=True, alpha=None, beta=None):
    if len(probs) != 5:
        raise TypeError("Probability must be of length 5")
    
    cdef double[5] _probs = np.asarray(probs, dtype=np.float);
    cdef double[::1] output = np.zeros(N ** 2, np.float)
    
    cdef c_automata.Params params
    params.probs = _probs
    params.competition = <int>(competition)
    params.alpha = 0.0 if alpha is None else <double>alpha
    params.beta = 0.0 if beta is None else <double>beta
    
    cdef c_automata.modelPtr model
    if alpha is None and beta is None:
        model = c_automata.model_simple
    else:
        model = c_automata.model_extend
    
    c_automata.pdf(&output[0], N, c_cells, steps, runs, model, params)
    
    return np.asarray(output)


@cython.boundscheck(False)
@cython.wraparound(False)
def pdf_rolling(int N, int c_cells, int init_steps, int samples, int sample_gap, int runs, probs, competition=True, alpha=None, beta=None):
    if len(probs) != 5:
        raise TypeError("Probability must be of length 5")
    
    #cdef int comp = 1 if competition else 0
    
    cdef double[5] _probs = np.asarray(probs, dtype=np.float);
    cdef double[::1] output = np.zeros(N ** 2, np.float)
    
    cdef c_automata.Params params
    params.probs = _probs
    params.competition = <int>(competition)
    params.alpha = 0.0 if alpha is None else <double>alpha
    params.beta = 0.0 if beta is None else <double>beta
    
    cdef c_automata.modelPtr model
    if alpha is None and beta is None:
        model = c_automata.model_simple
    else:
        model = c_automata.model_extend
        
    c_automata.pdf_rolling(&output[0], N, c_cells, init_steps, samples, sample_gap, runs, model, params)
    
    return np.asarray(output)
    

@cython.boundscheck(False)
@cython.wraparound(False)
def iterate(int[:, ::1] arr not None, int steps, probs, competition=True, alpha=None, beta=None):
    if len(probs) != 5:
        raise TypeError("Probability must be of length 5")
    
    cdef double[5] _probs = np.asarray(probs, dtype=np.float);
    cdef int[:, ::1] out_counts = np.zeros((steps, 4), dtype='i4')
    
    cdef c_automata.Params params
    params.probs = _probs
    params.competition = <int>(competition)
    params.alpha = 0.0 if alpha is None else <double>alpha
    params.beta = 0.0 if beta is None else <double>beta
    
    cdef c_automata.modelPtr model
    if alpha is None and beta is None:
        model = c_automata.model_simple
    else:
        model = c_automata.model_extend
            
    c_automata.iterate(&arr[0, 0], arr.shape[0], steps, model, params, &out_counts[0, 0])
    
    return np.asarray(out_counts)
