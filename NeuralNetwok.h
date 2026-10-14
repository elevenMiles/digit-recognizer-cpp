#ifndef NEURAL_NETWORK_H
#define NEURAL_NETWORK_H

#include "Matrix.h"

class NeuralNetwork
{
private:
	int inodes {};
	int hnodes {};
	int onodes {};
	float lr   {};

	Matrix W_ih;
	Matrix W_ho;

public:
	NeuralNetwork(int inputNodes, int hiddenNodes, int outputNodes, float learningRate);

	void train(const Matrix& INPUTS, const Matrix& TARGETS);
	Matrix query(const Matrix& INPUTS);
};

#endif