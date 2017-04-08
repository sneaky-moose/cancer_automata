cdef extern from "c_automata.h":
	ctypedef struct Params:
		double probs[5];
		int competition;
	
	ctypedef void (*modelPtr)(int *, int, Params);

	void pdf(double *output, int N, int c_cells, int steps, int runs, modelPtr model, Params params);
	void pdf_rolling(double *output, int N, int c_cells, int init_steps, int samples, int sample_gap, int runs, modelPtr model, Params params);
	void init_state(int *array, int N, int m)
	void iterate(int *array, int N, int steps, modelPtr model, Params params, int *out_counts);
	void model_simple(int *array, int N, Params params);
	
    #void iterate_endcount(int *array, int N, int steps, double *probs, int competition, int *out_counts)
    #void type_count(int *array, int N, int *output)