#include "Random.h"
#include "Matrix.h"

#include <vector>
#include <cassert>
#include <string>
#include <format>
#include <iostream>

Matrix::Matrix(int rows, int cols)
	: m_rows{ rows }
	, m_cols{ cols }
	, m_data(rows * cols)
{}

Matrix::Matrix(int rows, int cols, std::initializer_list<float> data)
	: Matrix(rows, cols)
{
	assert(m_data.size() == data.size() 
		&& "Matrix::Matrix(rows, cols, data) -> \"data\" size do not match with Matrix size.");
	m_data.assign(data);
}

void Matrix::initialize(int rows, int cols)
{
	m_rows = rows;
	m_cols = cols;
	m_data.resize(rows * cols);
}

std::string Matrix::shape() const
{
	return std::format("[{}, {}]", row_count(), col_count());
}

float& Matrix::operator()(int row, int col)
{
	assert(row >= 0 && row < m_rows && col >= 0 && col < m_cols 
		&& "Matrix::operator()(row, col) -> \"row\" or \"col\" out of range.");
	return m_data[row * m_cols + col];
}

float Matrix::operator()(int row, int col) const
{
	assert(row >= 0 && row < m_rows && col >= 0 && col < m_cols 
		&& "Matrix::operator()(row, col) -> \"row\" or \"col\" out of range.");
	return m_data[row * m_cols + col];
}

float& Matrix::operator()(int index)
{
	assert(index >= 0 && index < m_data.size() 
		&& "Matrix::operator()(index) -> \"index\" out of range.");
	return m_data[index];
}

float Matrix::operator()(int index) const
{
	assert(index >= 0 && index < m_data.size() 
		&& "Matrix::operator()(index) -> \"index\" out of range.");
	return m_data[index];
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

Matrix Matrix::xaviar_normal(int layer_in, int layer_out)
{
	float stddev {std::sqrt(2.0f / (layer_in + layer_out))};

	Matrix OUT{ layer_out, layer_in };

	for (int i{ 0 }; i < OUT.size(); ++i)
	{
		OUT(i) = Random::get_normal_dist(0.0f, stddev);
	}

	return OUT; 
}

Matrix Matrix::zeros(int rows, int cols)
{
	Matrix OUT{ rows, cols };

	for (int i{ 0 }; i < OUT.size(); ++i)
	{
		OUT(i) = 0.0f;
	}

	return OUT;
}

Matrix Matrix::dot(const Matrix& A, const Matrix& B)
{
	assert(A.col_count() == B.row_count() 
		&& "Matrix::dot(A, B) -> column count of Matrix \"A\" must be equal to row count of Matrix \"B\"");

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

// operator -

Matrix Matrix::operator-() const
{
	Matrix OUT{ row_count(), col_count() };

	for (int i{ 0 }; i < OUT.size(); ++i)
	{
		OUT(i) = -((*this)(i));
	}
	
	return OUT;
}

// operator +=

Matrix& Matrix::operator+=(float x)
{
	for (int i{ 0 }; i < size(); ++i)
	{
		(*this)(i) += x;
	}

	return (*this);
}

Matrix& Matrix::operator+=(const Matrix& M)
{
	assert((row_count() == M.row_count()) && (col_count() == M.col_count()) 
		&& "Matrix::operator+=(M) -> Dimensions of right operand Matrix(\"M\") must be equal to dimensions of left operand Matrix.");
	for (int i{ 0 }; i < size(); ++i)
	{
		(*this)(i) += M(i);
	}

	return (*this);
}

// operator -=

Matrix& Matrix::operator-=(float x)
{
	for (int i{ 0 }; i < size(); ++i)
	{
		(*this)(i) -= x;
	}

	return (*this);
}

Matrix& Matrix::operator-=(const Matrix& M)
{
	assert((row_count() == M.row_count()) && (col_count() == M.col_count()) 
		&& "Matrix::operator-=(M) -> Dimensions of right operand Matrix(\"M\") must be equal to dimensions of left operand Matrix.");
	for (int i{ 0 }; i < size(); ++i)
	{
		(*this)(i) -= M(i);
	}

	return (*this);
}

// operator *=
Matrix& Matrix::operator*=(float s)
{
	for (int i{ 0 }; i < size(); ++i)
	{
		(*this)(i) *= s;
	}

	return (*this);
}

// operator +

Matrix operator+(const Matrix& M, float x)
{
	Matrix OUT{ M.row_count(), M.col_count() };

	for (int i{ 0 }; i < OUT.size(); ++i)
	{
		OUT(i) = M(i) + x;
	}

	return OUT;
}

Matrix operator+(float x, const Matrix& M)
{
	return M + x;
}

Matrix operator+(const Matrix& A, const Matrix& B)
{
	assert(A.row_count() == B.row_count() && A.col_count() == B.col_count() 
		&& "Matrix::operator+(A, B) -> Dimensions of Matrix(\"A\") must be equal to dimensions of Matrix(\"B\").");

	Matrix OUT{ A.row_count(), A.col_count() };

	for (int i{ 0 }; i < OUT.size(); ++i)
	{
		OUT(i) = A(i) + B(i);
	}

	return OUT;
}

// operator -

Matrix operator-(const Matrix& M, float x)
{
	Matrix OUT{ M.row_count(), M.col_count() };

	for (int i{ 0 }; i < OUT.size(); ++i)
	{
		OUT(i) = M(i) - x;
	}

	return OUT;
}

Matrix operator-(float x, const Matrix& M)
{
	Matrix OUT{ M.row_count(), M.col_count() };

	for (int i{ 0 }; i < OUT.size(); ++i)
	{
		OUT(i) = x - M(i);
	}

	return OUT;
}

Matrix operator-(const Matrix& A, const Matrix& B)
{
	assert(A.row_count() == B.row_count() && A.col_count() == B.col_count() 
		&& "Matrix::operator-(A, B) -> Dimensions of Matrix(\"A\") must be equal to dimensions of Matrix(\"B\").");

	Matrix OUT{ A.row_count(), A.col_count() };

	for (int i{ 0 }; i < OUT.size(); ++i)
	{
		OUT(i) = A(i) - B(i);
	}

	return OUT;
}

// operator *

Matrix operator*(const Matrix& M, float x)
{
	Matrix OUT{ M.row_count(), M.col_count() };

	for (int i{ 0 }; i < OUT.size(); ++i)
	{
		OUT(i) = M(i) * x;
	}

	return OUT;
}

Matrix operator*(float s, const Matrix& M)
{
	return M * s;
}

Matrix operator*(const Matrix& A, const Matrix& B)
{
	assert(A.row_count() == B.row_count() && A.col_count() == B.col_count() 
		&& "Matrix::operator*(A, B) -> Dimensions of Matrix(\"A\") must be equal to dimensions of Matrix(\"B\").");

	Matrix OUT{ A.row_count(), A.col_count() };

	for (int i{ 0 }; i < OUT.size(); ++i)
	{
		OUT(i) = A(i) * B(i);
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
