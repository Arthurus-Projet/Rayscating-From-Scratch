#include "Headers/neuralNetwork.h"
#include <iostream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <fstream>

// Constructor
NeuralNetwork::NeuralNetwork(int input_neurons, int hidden_neurons, int output_neurons) {
    this->hidden_neurons = hidden_neurons;

    // Initialize random seed
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::srand(seed);

    //std::vector<double> s1 = {3.8, 3.8, 2.8, 2.8, 2.8, 3.11562, 5.8, 5.8, 5.8, 5.8, 5.8, 1.53601, 1.23725, 100.0};
    //std::vector<double> s2 = {2.06702 2.2595 3.59444 5.10828 4.71376 4.38585 4.10505 3.8584 3.63685 3.43382 3.24432 3.06439 2.89076 100.};

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
        //second_layer_values[j] = threshold(second_layer_values[j]);
    }
    return second_layer_values;
}

std::vector<double> NeuralNetwork::forward_propagation(std::vector<double> input_values) {
    calculate_first_layer_values(input_values);
    return calculate_second_layer_values();
}



void NeuralNetwork::save(const std::string& filename) {
        std::ofstream outFile(filename, std::ios::binary);
        if (outFile.is_open()) {
            // Sérialiser first_layer_weights
            size_t firstLayerWeightsSize = first_layer_weights.size();
            outFile.write(reinterpret_cast<const char*>(&firstLayerWeightsSize), sizeof(firstLayerWeightsSize));
            for (const auto& innerVec : first_layer_weights) {
                size_t innerSize = innerVec.size();
                outFile.write(reinterpret_cast<const char*>(&innerSize), sizeof(innerSize));
                outFile.write(reinterpret_cast<const char*>(innerVec.data()), innerSize * sizeof(double));
            }

            // Sérialiser second_layer_weights
            size_t secondLayerWeightsSize = second_layer_weights.size();
            outFile.write(reinterpret_cast<const char*>(&secondLayerWeightsSize), sizeof(secondLayerWeightsSize));
            for (const auto& innerVec : second_layer_weights) {
                size_t innerSize = innerVec.size();
                outFile.write(reinterpret_cast<const char*>(&innerSize), sizeof(innerSize));
                outFile.write(reinterpret_cast<const char*>(innerVec.data()), innerSize * sizeof(double));
            }

            // Sérialiser first_layer_values
            size_t firstLayerValuesSize = first_layer_values.size();
            outFile.write(reinterpret_cast<const char*>(&firstLayerValuesSize), sizeof(firstLayerValuesSize));
            outFile.write(reinterpret_cast<const char*>(first_layer_values.data()), firstLayerValuesSize * sizeof(double));

            // Sérialiser second_layer_values
            size_t secondLayerValuesSize = second_layer_values.size();
            outFile.write(reinterpret_cast<const char*>(&secondLayerValuesSize), sizeof(secondLayerValuesSize));
            outFile.write(reinterpret_cast<const char*>(second_layer_values.data()), secondLayerValuesSize * sizeof(double));

            outFile.close();
        } else {
            std::cerr << "Unable to open file for writing: " << filename << std::endl;
        }
    }

    // Méthode pour désérialiser la classe à partir d'un fichier
    void NeuralNetwork::load(const std::string& filename) {
        std::ifstream inFile(filename, std::ios::binary);
        if (inFile.is_open()) {
            // Désérialiser first_layer_weights
            size_t firstLayerWeightsSize;
            inFile.read(reinterpret_cast<char*>(&firstLayerWeightsSize), sizeof(firstLayerWeightsSize));
            first_layer_weights.resize(firstLayerWeightsSize);
            for (auto& innerVec : first_layer_weights) {
                size_t innerSize;
                inFile.read(reinterpret_cast<char*>(&innerSize), sizeof(innerSize));
                innerVec.resize(innerSize);
                inFile.read(reinterpret_cast<char*>(innerVec.data()), innerSize * sizeof(double));
            }

            // Désérialiser second_layer_weights
            size_t secondLayerWeightsSize;
            inFile.read(reinterpret_cast<char*>(&secondLayerWeightsSize), sizeof(secondLayerWeightsSize));
            second_layer_weights.resize(secondLayerWeightsSize);
            for (auto& innerVec : second_layer_weights) {
                size_t innerSize;
                inFile.read(reinterpret_cast<char*>(&innerSize), sizeof(innerSize));
                innerVec.resize(innerSize);
                inFile.read(reinterpret_cast<char*>(innerVec.data()), innerSize * sizeof(double));
            }

            // Désérialiser first_layer_values
            size_t firstLayerValuesSize;
            inFile.read(reinterpret_cast<char*>(&firstLayerValuesSize), sizeof(firstLayerValuesSize));
            first_layer_values.resize(firstLayerValuesSize);
            inFile.read(reinterpret_cast<char*>(first_layer_values.data()), firstLayerValuesSize * sizeof(double));

            // Désérialiser second_layer_values
            size_t secondLayerValuesSize;
            inFile.read(reinterpret_cast<char*>(&secondLayerValuesSize), sizeof(secondLayerValuesSize));
            second_layer_values.resize(secondLayerValuesSize);
            inFile.read(reinterpret_cast<char*>(second_layer_values.data()), secondLayerValuesSize * sizeof(double));

            inFile.close();
        } else {
            std::cerr << "Unable to open file for reading: " << filename << std::endl;
        }
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
