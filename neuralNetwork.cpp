#include "Headers/neuralNetwork.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

// Constructor
NeuralNetwork::NeuralNetwork(int input_neurons, int hidden_neurons, int output_neurons) {
    this->hidden_neurons = hidden_neurons;

    // Initialize random seed
    std::srand(std::time(0));

    // Initialize weights for the first layer
    first_layer_weights.resize(input_neurons + 1, std::vector<double>(hidden_neurons));
    for (int i = 0; i <= input_neurons; ++i) {
        for (int j = 0; j < hidden_neurons; ++j) {
            first_layer_weights[i][j] = ((std::rand() % 200) - 100) / 1000.0;
        }
    }

    // Initialize weights for the second layer
    second_layer_weights.resize(hidden_neurons + 1, std::vector<double>(output_neurons));
    for (int i = 0; i <= hidden_neurons; ++i) {
        for (int j = 0; j < output_neurons; ++j) {
            second_layer_weights[i][j] = ((std::rand() % 200) - 100) / 1000.0;
        }
    }
}

// Activation functions

double NeuralNetwork::relu(double x) {
    return std::max(0.0, x);
}

double NeuralNetwork::sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}

int NeuralNetwork::threshold(double x) {
    return x > 0 ? 1 : 0;
}

// Print functions

void NeuralNetwork::print_weights() {
    std::cout << "First layer weights:" << std::endl;
    for (const auto& row : first_layer_weights) {
        for (double weight : row) {
            std::cout << weight << " ";
        }
        std::cout << std::endl;
    }
}

void NeuralNetwork::print_second_layer_weights() {
    std::cout << "Second layer weights:" << std::endl;
    for (const auto& row : second_layer_weights) {
        for (double weight : row) {
            std::cout << weight << " ";
        }
        std::cout << std::endl;
    }
}

void NeuralNetwork::print_first_layer_values() {
    for (double value : first_layer_values) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

void NeuralNetwork::print_second_layer_values() {
    for (double value : second_layer_values) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

// Neural network calculations

std::vector<double> NeuralNetwork::calculate_first_layer_values(std::vector<double> input_values) {
    input_values.push_back(1.0);  // Adding 1 for the bias
    first_layer_values.resize(hidden_neurons + 1);

    for (int j = 0; j < hidden_neurons; ++j) {
        first_layer_values[j] = 0.0;
        for (int i = 0; i < input_values.size(); ++i) {
            first_layer_values[j] += input_values[i] * first_layer_weights[i][j];
        }
        first_layer_values[j] = relu(first_layer_values[j]);
    }
    first_layer_values[hidden_neurons] = 1.0;  // Bias for the output results of the output layer
    return first_layer_values;
}

std::vector<double> NeuralNetwork::calculate_second_layer_values() {
    second_layer_values.resize(second_layer_weights[0].size());

    for (int j = 0; j < second_layer_weights[0].size(); ++j) {
        second_layer_values[j] = 0.0;
        for (int i = 0; i < first_layer_values.size(); ++i) {
            second_layer_values[j] += first_layer_values[i] * second_layer_weights[i][j];
        }
        second_layer_values[j] = threshold(second_layer_values[j]);
    }
    return second_layer_values;
}

std::vector<double> NeuralNetwork::forward_propagation(std::vector<double> input_values) {
    calculate_first_layer_values(input_values);
    return calculate_second_layer_values();
}

/*

int main() {
    NeuralNetwork nn = NeuralNetwork(3, 4, 2);

    nn.print_weights();
    nn.print_second_layer_weights();

    std::vector<double> input_values = {1.0, 0.5, -1.5};
    std::vector<double> output = nn.forward_propagation(input_values);
    //nn.first_layer_weights().size();
    nn.second_layer_values;
    std::cout << "Output values:" << std::endl;
    for (int val : output) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}*/
