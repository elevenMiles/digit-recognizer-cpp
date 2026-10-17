#include "Canvas.h"
#include "DigitBuffer.h"
#include "Matrix.h"
#include "NeuralNetwok.h"

#include <Windows.h>
#include <algorithm> // for std::clamp

#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

std::string getMessage(int index)
{
    switch (index)
    {
    case 0: return "No Python."; 
    case 1: return "No TensorFlow.";
    case 2: return "No NumPy.";
    case 3: return "Just pure C++.";
    case 4: return "Raw math.";
    case 5: return "And a neural network built entirely from scratch.";
    }
}

int getGuessedDigit(NeuralNetwork& neuralNetwork, Matrix& INPUT)
{
    Matrix OUTPUT{ neuralNetwork.query(INPUT) };

    float max = 0.0f;
    int digit = -1;
    for (int i = 0; i < 10; ++i)
    {
        float current = OUTPUT(i);
        if (current > max)
        {
            max = current;
            digit = i;
        }
    }

    return digit;
}

void trainWithDigits(NeuralNetwork& neuralNetwork, int epoch = 20)
{
    // DATA PREPARETION

    std::vector<Matrix> inputs_mat;
    inputs_mat.reserve(1041);

    std::vector<char> outputs_digit;
    outputs_digit.reserve(1041);

    std::vector<Matrix> outputs_mat(neuralNetwork.get_onodes());

    for (int i = 0; i < outputs_mat.size(); ++i)
    {
        outputs_mat[i].initialize(10, 1);
        outputs_mat[i] += 0.0f;
        outputs_mat[i](i) = 1.0f;
    }

    std::fstream train_file { "digits_dataset\\digits_train.csv", std::ios::in };

    if (train_file.is_open())
    {
        std::string line;
        while (std::getline(train_file, line))
        {
            std::stringstream ss { line };

            std::string digit_str;
            std::getline(ss, digit_str, ',');
            int digit = std::stoi(digit_str);
            outputs_digit.push_back(digit);

            Matrix INPUT { 400, 1 };

            std::string value_str;
            for (int i = 0; i < 400; ++i)
            {
                std::getline(ss, value_str, ',');
                int value = std::stoi(value_str);

                INPUT(i) = (value / 255.0f * 0.99f) + 0.01f;
            }

            inputs_mat.push_back(INPUT);
        }
    }

    train_file.close();

    // TRAINING TIMEEEE

    for (int i = 0; i < epoch; ++i)
    {
        std::cout << getMessage(i) << '\n';
        for (int j = 0; j < inputs_mat.size(); ++j)
        {
            neuralNetwork.train(inputs_mat[j], outputs_mat[outputs_digit[j]]);
        }
    }
    std::cout << "NETWORK TRAINED.\n\n\n\n\n";
}

int testWithDigits(NeuralNetwork& neuralNetwork)
{
    std::cout << "TESTING.\n";

    // PREPARING DATA

    std::vector<Matrix> inputs_mat;
    inputs_mat.reserve(100);

    std::vector<char> outputs_digit;
    outputs_digit.reserve(100);

    std::fstream test_file{ "digits_dataset\\digits_test.csv", std::ios::in };

    if (test_file.is_open())
    {
        std::string line;
        while (std::getline(test_file, line))
        {
            std::stringstream ss{ line };

            std::string digit_str;
            std::getline(ss, digit_str, ',');
            int digit = std::stoi(digit_str);
            outputs_digit.push_back(digit);

            Matrix INPUT{ 400, 1 };

            std::string value_str;
            for (int i = 0; i < 400; ++i)
            {
                std::getline(ss, value_str, ',');
                int value = std::stoi(value_str);

                INPUT(i) = (value / 255.0f * 0.99f) + 0.01f;
            }

            inputs_mat.push_back(INPUT);
        }
    }

    test_file.close();

    // TESTING PART

    int accuracy = 0;

    for (int i = 0; i < inputs_mat.size(); ++i)
    {
        int digit = getGuessedDigit(neuralNetwork, inputs_mat[i]);
        if (digit == outputs_digit[i])
        {
            accuracy += 1;
        }
    }

    return accuracy;
}

void playWithCanvas(NeuralNetwork& neuralNetwork)
{
    constexpr int canvas_size = 20;
    Canvas<canvas_size, canvas_size> canvas;
    DigitBuffer<canvas_size, canvas_size> digitBuffer;

    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    float cursor_posx{ canvas_size / 2.0f };
    float cursor_posy{ canvas_size / 2.0f };

    float sensitivity{ 0.05f };

    SetCursorPos(centerX, centerY);

    POINT currentPos;

    bool wasSpaceDown = false;

    int guess = 0;

    while (true)
    {
        if (GetAsyncKeyState('X') & 0x8000) break;

        GetCursorPos(&currentPos);

        int deltaX = currentPos.x - centerX;
        int deltaY = currentPos.y - centerY;

        cursor_posx += deltaX * sensitivity;
        cursor_posy += deltaY * sensitivity;

        cursor_posx = std::clamp<float>(cursor_posx, 0.0f, canvas_size - 1.0f);
        cursor_posy = std::clamp<float>(cursor_posy, 0.0f, canvas_size - 1.0f);

        if (deltaX != 0 || deltaY != 0)
        {
            SetCursorPos(centerX, centerY);
        }

        if (GetAsyncKeyState('D') & 0x8000)
        {
            digitBuffer.setBrush((int)cursor_posx, (int)cursor_posy, 255);
        }

        if (GetAsyncKeyState('C') & 0x8000)
        {
            digitBuffer.clear();
        }

        if (GetAsyncKeyState(VK_SPACE) & 0x8000)
        {
            if (!wasSpaceDown)
            {
                Matrix INPUT { digitBuffer.getMatrix() };

                guess = getGuessedDigit(neuralNetwork, INPUT);

                wasSpaceDown = true;
            }
        }
        else { wasSpaceDown = false; }

        canvas.drawBuffer(digitBuffer);
        canvas.draw((int)cursor_posx, (int)cursor_posy, 255);

        canvas.print();

        std::cout << "Press \'X\' to exit.\n";
        std::cout << "Press \'C\' to clear canvas.\n";
        std::cout << "Press spacebar to see guess -> " << guess << '\n';

        canvas.fill(0);

        Sleep(2);
    }

}

int main()
{
    NeuralNetwork neuralNetwork{ 400, 100, 10, 0.3f };
    
    trainWithDigits(neuralNetwork, 6);
    playWithCanvas(neuralNetwork);

    return 0;
}