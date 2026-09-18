#include "Matrix.h"

#include <iostream>
#include <cmath>

float sigmoid(float x)
{
	constexpr float E{ 2.71828f };
	return 1.0f / (1.0f + std::pow(E, -x));
}

int main()
{
	Matrix M{ 3, 3, {
		0.9f, 0.3f, 0.4f,
		0.2f, 0.8f, 0.2f,
		0.1f, 0.5f, 0.6f
	}};

	std::cout << "M=\n" << M << '\n';
	std::cout << "M_transpose=\n" << M.transpose() << '\n';

	return 0;
}
