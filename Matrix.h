#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <initializer_list>
#include <string>
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
	std::string shape() const;

	float& operator()(int row, int col);
	float  operator()(int row, int col) const;
	float& operator()(int index);
	float  operator()(int index) const;

	Matrix transpose() const;
	Matrix applyFunc(float(*func)(float)) const;

	static Matrix random(int rows, int cols);
	static Matrix dot(const Matrix& A, const Matrix& B);

	// operator -
	Matrix operator-() const;

	// operator +=
	Matrix& operator+=(float x);
	Matrix& operator+=(const Matrix& M);

	// operator -=
	Matrix& operator-=(float x);
	Matrix& operator-=(const Matrix& M);

	// operator *=
	Matrix& operator*=(float s);
};

// operator +
Matrix operator+(const Matrix& M, float x);
Matrix operator+(float x, const Matrix& M);
Matrix operator+(const Matrix& A, const Matrix& B);

// operator -
Matrix operator-(const Matrix& M, float x);
Matrix operator-(float x, const Matrix& M);
Matrix operator-(const Matrix& A, const Matrix& B);

// operator *
Matrix operator*(const Matrix& M, float s);
Matrix operator*(float s, const Matrix& M);
Matrix operator*(const Matrix& A, const Matrix& B);

std::ostream& operator<<(std::ostream& out, const Matrix& M);

#endif