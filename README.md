# Digit Recognizer: a neural network from scratch in C++

A handwritten-digit recognizer built with **no Python, no TensorFlow, no NumPy, and no ML libraries at all**. The matrix math, the neural network, the backpropagation, and even the drawing canvas are all written by hand in plain C++20.

Train it, then draw a digit right in your terminal and watch the network guess what you drew.

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey)
![Dependencies](https://img.shields.io/badge/dependencies-none-brightgreen)

<!-- Add a screenshot or GIF of the canvas here, e.g.: ![demo](docs/demo.gif) -->

## Features

- **Custom `Matrix` class** with operator overloading, transpose, dot product, and element-wise ops. It is the only math tool the network uses.
- **Feed-forward neural network** (input → hidden → output) trained with **backpropagation** and stochastic gradient descent.
- **Interactive terminal canvas**: draw a digit with your mouse, press a key, and get a prediction.
- **Fast to train**: the default 6 epochs take only a few seconds in a Release build, so you can play with it right away.
- **Self-contained**: a small 20×20 digit dataset is included, with nothing to download.

## How it works

### The network

| Layer  | Size | Meaning                                      |
|--------|------|----------------------------------------------|
| Input  | 400  | One neuron per pixel of the 20×20 image      |
| Hidden | 100  | Learns intermediate features of the strokes  |
| Output | 10   | One neuron per digit, 0 to 9                 |

Every neuron uses a **sigmoid** activation. The learning rate is `0.3`. Weights start as random values between −0.5 and 0.5. This network has no bias terms.

The prediction is simply the output neuron with the highest activation.

### Forward pass

Each layer multiplies its inputs by a weight matrix and squashes the result with a sigmoid:

```
hidden = sigmoid(W_ih · input)
output = sigmoid(W_ho · hidden)
```

### Learning (backpropagation)

For every training image the network:

1. Makes a prediction with the forward pass.
2. Compares it to the target, a "one-hot" vector such as `[0,0,0,1,0,0,0,0,0,0]` for a 3, to get the **output error**.
3. Sends that error backwards through the transposed weights to work out how much each **hidden neuron** was responsible for it.
4. Nudges each weight matrix to reduce the error:

```
ΔW_ho = lr · (error_out    ⊙ out    ⊙ (1 − out))    · hiddenᵀ
ΔW_ih = lr · (error_hidden ⊙ hidden ⊙ (1 − hidden)) · inputᵀ
```

The `out ⊙ (1 − out)` term is the derivative of the sigmoid, and `⊙` is element-wise multiplication. Training is online: weights update after every single image, with no batching, and the whole training set is repeated for several **epochs** (6 by default).

### Input preprocessing

Pixel values (0 to 255) are rescaled to the range **0.01 to 1.0**. Avoiding exact zeros keeps the weight updates from dying out on blank pixels. The drawing canvas uses the same scaling, so what you draw looks to the network exactly like a training image.

## The dataset

`digits_dataset/` contains two CSV files:

| File               | Images | Per digit |
|--------------------|--------|-----------|
| `digits_train.csv` | 1,040  | ~104      |
| `digits_test.csv`  | 100    | 10        |

Each row is `label, p0, p1, ..., p399`: the digit (0 to 9) followed by 400 grayscale pixel values (0 to 255) of a 20×20 image in row-major order.

## Getting started

### Requirements

- **Windows**: the interactive canvas uses the Win32 API (`GetAsyncKeyState`, `SetCursorPos`) for input.
- **Visual Studio** with the C++ desktop workload and C++20 support.

### Build and run

1. Clone the repository:
   ```bash
   git clone https://github.com/<your-username>/<repo-name>.git
   ```
2. Open `digit_recognizer_neural_net.slnx` in Visual Studio.
3. The project is set to platform toolset `v145`. If your Visual Studio doesn't have it, right-click the project → **Properties** → **General** → **Platform Toolset** and pick the one you have installed.
4. Choose **Release | x64** and press **Ctrl+F5** (Run without debugging).

> The program loads the dataset from `digits_dataset\` using a relative path, so it must run with the project folder as its working directory. This is Visual Studio's default.

On launch the network trains for a few seconds (Debug builds are slower), then the drawing canvas appears.

## Using the canvas

The mouse acts as a relative "joystick": the system cursor is kept at the centre of the screen, and your mouse movement moves a cursor on the 20×20 canvas.

| Key       | Action                      |
|-----------|-----------------------------|
| Mouse     | Move the cursor             |
| `D` (hold)| Draw                        |
| `C`       | Clear the canvas            |
| `Space`   | Ask the network to guess    |
| `X`       | Exit                        |

Tips for the best results: draw the digit large, roughly centred, and with a continuous stroke, the way the training digits look.

## Results

On the 100 bundled test images, with the default settings (400-100-10, learning rate 0.3, 6 epochs), the network scored **100 / 100 in each of 20 independent runs**, every run starting from fresh random weights.

| Epochs | Mean test accuracy (20 runs) |
|--------|------------------------------|
| 1      | 97.3 %                       |
| 3      | 99.9 %                       |
| 6      | 100 %                        |

The test set is small and drawn from the same distribution as the training set, so treat these numbers as a sanity check rather than a benchmark. Digits you draw by hand with a mouse will look a little different from the dataset, so expect real-world accuracy to be lower.

## Project structure

```
.
├── main.cpp                 # Data loading, training, testing, interactive canvas loop
├── NeuralNetwork.cpp        # Network: forward pass (query) and backpropagation (train)
├── NeuralNetwok.h           # NeuralNetwork class declaration
├── Matrix.h / Matrix.cpp    # Hand-written linear algebra
├── Canvas.h                 # ASCII renderer for the terminal canvas
├── DigitBuffer.h            # Drawing buffer, brush, and conversion to network input
└── digits_dataset/
    ├── digits_train.csv
    └── digits_test.csv
```
