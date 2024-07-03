#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

class NeuralNetwork {
private:
    std::vector<std::vector<double>> first_layer_weights;
    std::vector<std::vector<double>> second_layer_weights;
    std::vector<double> first_layer_values;
    std::vector<double> second_layer_values;
    int hidden_neurons;

    // ReLU activation function
    double relu(double x) {
        return std::max(0.0, x);
    }

    // Sigmoid activation function
    double sigmoid(double x) {
        return 1.0 / (1.0 + std::exp(-x));
    }

    // Threshold activation function
    int threshold(double x) {
        return x > 0 ? 1 : 0;
    }

public:
    NeuralNetwork(int input_neurons, int hidden_neurons, int output_neurons) {
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

    void print_weights() {
        // Display the weights of the first layer
        std::cout << "First layer weights:" << std::endl;
        for (const auto& row : first_layer_weights) {
            for (double weight : row) {
                std::cout << weight << " ";
            }
            std::cout << std::endl;
        }
    }

    void print_second_layer_weights() {
        // Display the weights of the second layer
        std::cout << "Second layer weights:" << std::endl;
        for (const auto& row : second_layer_weights) {
            for (double weight : row) {
                std::cout << weight << " ";
            }
            std::cout << std::endl;
        }
    }

    void print_first_layer_values() {
        // Display the output values of the first layer
        for (double value : first_layer_values) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    void print_second_layer_values() {
        // Display the output values of the second layer
        for (double value : second_layer_values) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }

    std::vector<double> calculate_first_layer_values(std::vector<double> input_values) {
        /*
        Matrix multiplication of input values by the weights of the first layer

        Args:
            input_values (vector<double>): List of input values
        */
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

    std::vector<double> calculate_second_layer_values() {
        // Matrix multiplication of hidden layer values by the weights of the second layer
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

    std::vector<double> forward_propagation(std::vector<double> input_values) {
        /*
        Take input values and return a binary output, 1 for a jump or 0 for no jump

        Args:
            input_values (vector<double>): List of input values
        */
        calculate_first_layer_values(input_values);
        return calculate_second_layer_values();
    }
};

int main() {
    NeuralNetwork nn(3, 4, 2);

    nn.print_weights();
    nn.print_second_layer_weights();

    std::vector<double> input_values = {1.0, 0.5, -1.5};
    std::vector<double> output = nn.forward_propagation(input_values);

    std::cout << "Output values:" << std::endl;
    for (int val : output) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
