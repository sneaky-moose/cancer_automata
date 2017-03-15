int seek_neighbour(int **array, int N, int i, int j)
{
	int value;
	
	if (0 <= i && i < N && 0 <= j && j < N) /* within boundary */
	{
		value = array[i][j];
		if (value == 1 || value == 0)
		{
			return value;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}
}

void get_neighbours(int **array, int N, int i, int j, int *neighbours)
{
	int k, out[2];
	
	for (k = 0; k < 4; k++)
	{
		order_neighbours(i, j, k, out);
		neighbours[k] = seek_neighbour(array, N, out[0], out[1]);
	}
	neighbours[0] = seek_neighbour(array, N, i, j+1);
	neighbours[1] = seek_neighbour(array, N, i+1, j);
	neighbours[2] = seek_neighbour(array, N, i, j-1);
	neighbours[3] = seek_neighbour(array, N, i-1, j);
}