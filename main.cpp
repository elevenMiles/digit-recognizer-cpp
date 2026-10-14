#include "Matrix.h"
#include "NeuralNetwok.h"

#include <iostream>
#include <cmath>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <random>

char getShadingChar(float intensity)
{
	if (intensity > 0.85f) return '#';
	if (intensity > 0.75f) return 'x';
	if (intensity > 0.5f) return '-';
	if (intensity > 0.25f) return '.';
	return ' ';
}

int random(int min, int max)
{
	static std::random_device rd;
	static std::mt19937 mt{ rd() };

	std::uniform_real_distribution<float> distr(min, max);

	return distr(mt);
}

int main()
{
	int random_value { random(1, 99) }; // must be between 0 - 100

	int input_nodes  { 785 };
	int hidden_nodes { 100 };
	int output_nodes { 10  };

	float learning_rate { 0.3f };

	NeuralNetwork neuralNetwork { input_nodes, hidden_nodes, output_nodes, learning_rate };

	Matrix TEST_INPUTS { 785, 1 };

	std::ifstream mnist_file;
	std::string number_string;

	mnist_file.open("mnist_dataset/mnist_train_100.csv");

	int counter{ 0 };
	if (mnist_file.is_open())
	{
		std::string line;
		while (std::getline(mnist_file, line))
		{
			std::stringstream ss{ line };
			int cell_array[785];

			for (int i{ 0 }; i < 785; ++i)
			{
				std::string cell;
				std::getline(ss, cell, ',');
				cell_array[i] = std::stoi(cell);
			}

			Matrix INPUTS{ 785, 1 };

			for (int i{ 1 }; i < 785; ++i)
			{
				float intensity{ (cell_array[i] / 255.0f * 0.99f) + 0.01f };
				INPUTS(i, 0) = intensity;
			}

			Matrix TARGETS{ 10, 1 };
			TARGETS += 0.01f;
			TARGETS(cell_array[0], 0) = 0.99f;

			neuralNetwork.train(INPUTS, TARGETS);
			std::cout << "Training iteration " << ++counter << '\n';

			if (counter == random_value)
			{
				TEST_INPUTS = INPUTS;
			}
		}
	}

	mnist_file.close();

	std::cout << "Neural network successfully trained.\n\n";
	
	for (int i{ 1 }; i < TEST_INPUTS.size(); ++i)
	{
		std::cout << getShadingChar(TEST_INPUTS(i)) << ' ';
		if (i % 28 == 0) std::cout << '\n';
	}

	std::cout << "\n\n";

	Matrix TEST_OUTPUTS { neuralNetwork.query(TEST_INPUTS) };

	float max { 0.0f };
	int digit { 0 };
	for (int i{ 0 }; i < 10; ++i)
	{
		float current { TEST_OUTPUTS(i) };
		if (current > max)
		{
			max = current;
			digit = i;
		}
	}

	std::cout << "output -> " << digit << '\n';

	return 0;
}