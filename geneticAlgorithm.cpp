//#include "Headers/neuralNetwork.h"
#include "Headers/AlgorithmeGenetique.h"


AlgorithmeGenetique::AlgorithmeGenetique(int nombre_individus, int nombre_neurone_entre, int nombre_neurone_cache, int nombre_neurone_sortie) {
    this->nombre_individus = nombre_individus;
    std::srand(std::time(nullptr)); // Initialize random seed

    individus.resize(nombre_individus, NeuralNetwork(nombre_neurone_entre, nombre_neurone_cache, nombre_neurone_sortie));
    fitness.resize(nombre_individus, -1);
}

int AlgorithmeGenetique::most_frequent_element(const std::vector<int>& lst) {
    int element = -1;
    int max_occurrences = 0;
    for (int x : lst) {
        int occurrences = std::count(lst.begin(), lst.end(), x);
        if (occurrences > max_occurrences) {
            element = x;
            max_occurrences = occurrences;
        }
    }
    return element;
}

int AlgorithmeGenetique::individual_action(int i) {
    std::vector<double> action = individus[i].forward_propagation({1.0, 0.5, -1.5}); // Example input values
    if (action[0] == 1 && action[1] == 0) {
        return 1;
    } else if (action[1] == 1 && action[0] == 0) {
        return -1;
    }
    return 0;
}

void AlgorithmeGenetique::reproduction() {
    std::vector<NeuralNetwork> individual_list;
    int i_max_fitness = 0;
    int value_max_fitness = 0;

    for (size_t i = 0; i < individus.size(); ++i) {
        int fitness_ = fitness[i]; // Assuming fitness score is stored in fitness vector
        if (fitness_ > value_max_fitness) {
            value_max_fitness = fitness_;
            i_max_fitness = i;
        }
        for (int j = 0; j < fitness_; ++j) {
            individual_list.push_back(individus[i]);
        }
    }

    individus[0] = individus[i_max_fitness];
    individus[10].first_layer_weights;

    for (size_t i = 1; i < individus.size(); ++i) {
        individus[i] = individus[i_max_fitness];
    }
}

void AlgorithmeGenetique::mutation(int prob, int prob_0, int prob_inverse, float noise_value) {
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-noise_value, noise_value);

    for (int i = 2; i < individus.size(); i += 2) {
        for (int y = 0; y < individus[i].first_layer_weights.size(); ++y) {
            for (int y2 = 0; y2 < individus[i].first_layer_weights[y].size(); ++y2) {
                if (std::rand() % prob == 1) {
                    individus[i].first_layer_weights[y][y2] += distribution(generator);
                } else if (std::rand() % prob_0 == 1) {
                    individus[i].first_layer_weights[y][y2] = 0;
                } else if (std::rand() % prob_inverse == 1) {
                    individus[i].first_layer_weights[y][y2] = -individus[i].first_layer_weights[y][y2];
                }

                if (std::rand() % prob == 1) {
                    individus[i + 1].first_layer_weights[y][y2] += distribution(generator);
                } else if (std::rand() % prob_0 == 1) {
                    individus[i + 1].first_layer_weights[y][y2] = 0;
                } else if (std::rand() % prob_inverse == 1) {
                    individus[i + 1].first_layer_weights[y][y2] = -individus[i + 1].first_layer_weights[y][y2];
                }
            }
        }

        for (size_t y = 0; y < individus[i].second_layer_weights.size(); ++y) {
            for (size_t y2 = 0; y2 < individus[i].second_layer_weights[y].size(); ++y2) {
                if (std::rand() % prob == 1) {
                    individus[i].second_layer_weights[y][y2] += distribution(generator);
                } else if (std::rand() % prob_0 == 1) {
                    individus[i].second_layer_weights[y][y2] = 0;
                } else if (std::rand() % prob_inverse == 1) {
                    individus[i].second_layer_weights[y][y2] = -individus[i].second_layer_weights[y][y2];
                }

                if (std::rand() % prob == 1) {
                    individus[i + 1].second_layer_weights[y][y2] += distribution(generator);
                } else if (std::rand() % prob_0 == 1) {
                    individus[i + 1].second_layer_weights[y][y2] = 0;
                } else if (std::rand() % prob_inverse == 1) {
                    individus[i + 1].second_layer_weights[y][y2] = -individus[i + 1].second_layer_weights[y][y2];
                }
            }
        }
    }
}

int main() {

    return 0;
}
