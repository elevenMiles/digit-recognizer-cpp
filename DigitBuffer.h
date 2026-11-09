#ifndef DIGIT_BUFFER_H
#define DIGIT_BUFFER_H

#include "Matrix.h"

#include <cassert>
#include <vector>
#include <string>
#include <string_view>
#include <fstream>

template <int WIDTH, int HEIGHT>
class DigitBuffer
{
private:
	std::vector<int> buffer;

	constexpr bool isInRange(int x, int y)
	{
		return x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT;
	}

public:
	DigitBuffer()
		: buffer(WIDTH * HEIGHT)
	{}

	void set(int x, int y, int val)
	{
		if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return;
		buffer[y * WIDTH + x] = val;
	}

	void set(int index, int val)
	{
		if (index < 0 || index >= buffer.size()) return;
		buffer[index] = val;
	}

	int get(int x, int y)
	{
		assert(isInRange(x, y) && "DigitBuffer::get(x, y) -> x or y is out of range.");
		return buffer[y * WIDTH + x];
	}

	int get(int index)
	{
		assert(index >= 0 && index < buffer.size() && "DigitBuffer::get(index) -> index is out of range.");
		return buffer[index];
	}

	int get_width() { return WIDTH; }

	int get_height() { return HEIGHT; }

	int size() { return buffer.size(); }

	void setBrush(int x, int y, int val)
	{
		for (int offset = -1; offset <= 1; ++offset)
		{
			int intensity = std::abs(offset) == 0 ? val : val * 0.75f;

			int x_offset = x + offset;
			int y_offset = y + offset;

			if (isInRange(x_offset, y))
			{
				if (get(x_offset, y) < val)
				{
					set(x_offset, y, intensity);
				}
			}

			if (isInRange(x, y_offset))
			{
				if (get(x, y_offset) < val)
				{
					set(x, y_offset, intensity);
				}
			}
		}
	}

	Matrix getMatrix()
	{
		Matrix OUT { (int)buffer.size(), 1};

		for (int i = 0; i < buffer.size(); ++i)
		{
			OUT(i, 0) = (buffer[i] / 255.0f * 0.99f) + 0.01f;
		}

		return OUT;
	}

	void clear()
	{
		for (int i = 0; i < buffer.size(); ++i)
		{
			buffer[i] = 0;
		}
	}
};

#endif