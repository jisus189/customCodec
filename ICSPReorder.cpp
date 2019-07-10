#include "pch.h"
/*
template<class T>
vector<short> DCTCompressor::zig_zag_matrix(vector<vector<T>> &mat)
{
	vector<T> res;
	int row = 0, col = 0, i = 0;

	while (row < mat.size())
	{
		res.push_back(mat[row][col]);

		if (row == mat.size() - 1)
		{
			row = col + 1;
			col = mat.size() - 1;
		}
		else if (col == 0)
		{
			col = row + 1;
			row = 0;
		}
		else
		{
			row++;
			col--;
		}
	}
	return res;
}
*/