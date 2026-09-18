#include "Matrix.h"

#include <vector>
#include <cassert>
#include <iostream>

Matrix::Matrix(int rows, int cols)
	: m_rows{ rows }
	, m_cols{ cols }
	, m_data(rows * cols)
{}

Matrix::Matrix(int rows, int cols, std::initializer_list<float> data)
	: Matrix(rows, cols)
{
	assert(m_data.size() == data.size() && "Initializer list size does not match with Matrix data size");
	m_data.assign(data);
}

float& Matrix::operator()(int row, int col)
{
	assert(row >= 0 && row < m_rows && col >= 0 && col < m_cols && "Matrix index out of range");
	return m_data[row * m_cols + col];
}

float  Matrix::operator()(int row, int col) const
{
	assert(row >= 0 && row < m_rows && col >= 0 && col < m_cols && "Matrix index out of range");
	return m_data[row * m_cols + col];
}

Matrix Matrix::transpose() const
{
	Matrix OUT{ m_cols, m_rows };

	for (int row{ 0 }; row < m_rows; ++row)
	{
		for (int col{ 0 }; col < m_cols; ++col)
		{
			OUT(col, row) = (*this)(row, col);
		}
	}

	return OUT;
}

Matrix Matrix::applyFunc(float(*func)(float)) const
{
	Matrix OUT{ m_rows, m_cols };

	for (int i{ 0 }; i < size(); ++i)
		OUT.m_data[i] = func(m_data[i]);

	return OUT;
}

Matrix operator*(const Matrix& A, const Matrix& B)
{
	assert(A.col_count() == B.row_count() && "Matrix dimensions does not match");

	int Acol_Brow_Count{ A.col_count() };
	int rows{ A.row_count() };
	int cols{ B.col_count() };

	Matrix OUT{ rows, cols };

	for (int row{ 0 }; row < rows; ++row)
	{
		for (int col{ 0 }; col < cols; ++col)
		{
			float dot{ 0.0f };
			for (int Acol_Brow{ 0 }; Acol_Brow < Acol_Brow_Count; ++Acol_Brow)
			{
				dot += A(row, Acol_Brow) * B(Acol_Brow, col);
			}
			OUT(row, col) = dot;
		}
	}

	return OUT;
}

std::ostream& operator<<(std::ostream& out, const Matrix& M)
{
	for (int r{ 0 }; r < M.row_count(); ++r)
	{
		out << "[ ";
		for (int c{ 0 }; c < M.col_count(); ++c)
		{
			out << M(r, c) << ' ';
		}
		out << "]\n";
	}

	return out;
}