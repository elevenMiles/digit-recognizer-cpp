#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <initializer_list>
#include <iostream>

class Matrix
{
private:
	int m_rows{};
	int m_cols{};
	std::vector<float> m_data;

public:
	Matrix(int rows, int cols);
	Matrix(int rows, int cols, std::initializer_list<float> data);

	int size() const { return m_data.size(); }
	int row_count() const { return m_rows; }
	int col_count() const { return m_cols; }

	float& operator()(int row, int col);
	float  operator()(int row, int col) const;

	Matrix transpose() const;
	Matrix applyFunc(float(*func)(float)) const;
};

Matrix operator*(const Matrix& A, const Matrix& B);
std::ostream& operator<<(std::ostream& out, const Matrix& M);

#endif