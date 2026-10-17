#include "Matrix.h"
#include "NeuralNetwok.h"

#include <cassert>

float sigmoid(float x)
{
	constexpr float E{ 2.71828f };
	return 1.0f / (1.0f + std::pow(E, -x));
}

NeuralNetwork::NeuralNetwork(int inputNodes, int hiddenNodes, int outputNodes, float learningRate)
	: inodes { inputNodes   }
	, hnodes { hiddenNodes  }
	, onodes { outputNodes  }
	, lr     { learningRate }
	, W_ih   { Matrix::random(hnodes, inodes) - 0.5f }
	, W_ho   { Matrix::random(onodes, hnodes) - 0.5f }
{}

void NeuralNetwork::train(const Matrix& INPUTS, const Matrix& TARGETS)
{
	assert(INPUTS.col_count() == 1 && "NeuralNetwork::train(INPUTS, TARGETS) -> INPUTS.shape() must be [n, 1]");
	assert(TARGETS.col_count() == 1 && "NeuralNetwork::train(INPUTS, TARGETS) -> TARGETS.shape() must be [n, 1]");

	Matrix hidden_inputs  { Matrix::dot(W_ih, INPUTS) };
	Matrix hidden_outputs { hidden_inputs.applyFunc(sigmoid) };

	Matrix final_inputs   { Matrix::dot(W_ho, hidden_outputs) };
	Matrix final_outputs  { final_inputs.applyFunc(sigmoid) };

	Matrix output_errors  { TARGETS - final_outputs };
	Matrix hidden_errors  { Matrix::dot(W_ho.transpose(), output_errors) };

	// Update the weights for the links between hidden and output layers
	W_ho += lr * Matrix::dot(output_errors * final_outputs * (1.0f - final_outputs), hidden_outputs.transpose());

	// Update the weights for the links between input and hidden layers
	W_ih += lr * Matrix::dot(hidden_errors * hidden_outputs * (1.0f - hidden_outputs), INPUTS.transpose());
}

Matrix NeuralNetwork::query(const Matrix& INPUTS)
{
	assert(INPUTS.col_count() == 1 && "NeuralNetwork::query(INPUTS) -> INPUTS.shape() must be [n, 1]");

	Matrix hidden_inputs{ Matrix::dot(W_ih, INPUTS) };
	Matrix hidden_outputs{ hidden_inputs.applyFunc(sigmoid) };

	Matrix final_inputs{ Matrix::dot(W_ho, hidden_outputs) };
	Matrix final_outputs{ final_inputs.applyFunc(sigmoid) };

	return final_outputs;
}