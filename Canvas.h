#ifndef CANVAS_H
#define CANVAS_H

#include "DigitBuffer.h"

#include <cmath>	 // for std::round
#include <vector>    // for std::vector
#include <string>    // for std::string
#include <algorithm> // for std::clamp
#include <cassert>   // for assert()
#include <iostream>  // for std::cout, std::cin

template <int WIDTH, int HEIGHT>
class Canvas
{
private:
	std::vector<int> data;

	char getShadingChar(int val)
	{
		const char symbols[]{ " .-=+*#%@" };
		constexpr int len = (sizeof(symbols) / sizeof(char)) - 1;

		val = std::clamp<int>(val, 0, 255);
		int index = val * (len - 1) / 255.0f;

		return symbols[index];
	}

public:
	Canvas() 
		: data(WIDTH * HEIGHT)
	{}

	void draw(int x, int y, int val)
	{
		if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
		data[y * WIDTH + x] = val;
	}

	void draw(int index, int val)
	{
		if (index < 0 || index >= data.size()) return;
		data[index] = val;
	}

	void drawBuffer(DigitBuffer<WIDTH, HEIGHT>& buffer)
	{
		assert(buffer.size() == data.size() && "Canvas::drawBuffer(buffer) -> buffer.size() is not equal to canvas.size()");

		for (int i = 0; i < data.size(); ++i)
		{
			data[i] = buffer.get(i);
		}
	}

	void print()
	{
		std::string output = "\033[H";
		output.reserve(output.size() + (HEIGHT + 2) * (WIDTH * 2 + 1));

		for (int i = 0; i < WIDTH; ++i) output += "--"; 
		output += '\n';

		for (int row = 1; row < HEIGHT - 1; ++row)
		{
			for (int col = 0; col < WIDTH; ++col)
			{
				output += getShadingChar(data[row * WIDTH + col]);
				output += ' ';
			}
			output += '\n';
		}

		for (int i = 0; i < WIDTH; ++i) output += "--"; 
		output += '\n';

		std::cout.write(output.data(), output.size());
	}

	void fill(int val)
	{
		for (int i = 0; i < data.size(); ++i)
			data[i] = val;
	}
};

#endif