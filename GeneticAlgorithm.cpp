//#include "Headers/neuralNetwork.h"
#include "Headers/GeneticAlgorithm.h"
#include <iostream>


GeneticAlgorithm::GeneticAlgorithm(int nombre_individus, int nombre_neurone_entre, int nombre_neurone_cache, int nombre_neurone_sortie) {
    this->nombre_individus = nombre_individus;
    std::srand(std::time(nullptr)); // Initialize random seed

      for (int i = 0; i < nombre_individus; ++i)
        individus.push_back(NeuralNetwork(nombre_neurone_entre, nombre_neurone_cache, nombre_neurone_sortie));
    //fitness.resize(nombre_individus, -1);
}

int GeneticAlgorithm::most_frequent_element(const std::vector<int>& lst) {
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

int GeneticAlgorithm::individual_action(std::vector<double> input, int i) {
    std::vector<double> action = individus[i].forward_propagation(input); // Example input values
    int index = 0;
    double max_value = std::numeric_limits<double>::lowest();
    for (int i = 0; i < 5; ++i) {
        if (action[i] > max_value) {
            max_value = action[i];
            index = i;
        }
    }
    return index;
}

void GeneticAlgorithm::reproduction(std::vector<int> fitness) {
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

    // Configuration de l'aléatoire
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, individual_list.size() - 1);


    for (size_t i = 1; i < individus.size(); ++i) {
        size_t random_index = dis(gen);
        individus[i] = individual_list[random_index];
    }


    for (int i = 1; i < individus.size() - 2; i += 2) {
        for (size_t y = 0; y < individus[i].first_layer_weights.size(); ++y) {
            for (size_t y2 = 0; y2 < individus[i].first_layer_weights[y].size(); ++y2) {
                individus[i].first_layer_weights[y][y2] = (individus[i].first_layer_weights[y][y2] + individus[i + 1].first_layer_weights[y][y2]) / 2;
                individus[i + 1].first_layer_weights[y][y2] = (individus[i].first_layer_weights[y][y2] + individus[i + 1].first_layer_weights[y][y2]) / 2;
            }
        }

        for (size_t y = 0; y < individus[i].second_layer_weights.size(); ++y) {
            for (size_t y2 = 0; y2 < individus[i].second_layer_weights[y].size(); ++y2) {
                individus[i].second_layer_weights[y][y2] = (individus[i].second_layer_weights[y][y2] + individus[i + 1].second_layer_weights[y][y2]) / 2;
                individus[i + 1].second_layer_weights[y][y2] = (individus[i].second_layer_weights[y][y2] + individus[i + 1].second_layer_weights[y][y2]) / 2;
            }
        }
    }
}


void GeneticAlgorithm::mutation(int prob, int prob_0, int prob_inverse, float noise_value) {
    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(-noise_value, noise_value);

    // Configuration du générateur de nombres aléatoires
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis_prob(1, prob);
    std::uniform_int_distribution<> dis_prob_0(1, prob_0);
    std::uniform_int_distribution<> dis_prob_inverse(1, prob_inverse);

    for (int i = 1; i < individus.size(); i += 1) {
        for (int y = 0; y < individus[i].first_layer_weights.size(); ++y) {
            for (int y2 = 0; y2 < individus[i].first_layer_weights[y].size(); ++y2) {
                if ((dis_prob(gen) == 1)) {
                    individus[i].first_layer_weights[y][y2] += distribution(generator);
                } else if (dis_prob_0(gen) == 1) {  
                    individus[i].first_layer_weights[y][y2] = 0;
                } else if (dis_prob_inverse(gen) == 1) {      
                    individus[i].first_layer_weights[y][y2] = -individus[i].first_layer_weights[y][y2];
                }
            }
        }

        for (size_t y = 0; y < individus[i].second_layer_weights.size(); ++y) {
            for (size_t y2 = 0; y2 < individus[i].second_layer_weights[y].size(); ++y2) {
                if (dis_prob(gen) == 1) {
                    individus[i].second_layer_weights[y][y2] += distribution(generator);
                } else if (dis_prob_0(gen) == 1) {
                    individus[i].second_layer_weights[y][y2] = 0;
                } else if (dis_prob_inverse(gen) == 1) {
                    individus[i].second_layer_weights[y][y2] = -individus[i].second_layer_weights[y][y2];
                }

                
            }
        }
    }
}

/*int main() {

    return 0;
}
*/
