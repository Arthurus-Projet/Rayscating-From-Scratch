#ifndef GeneticAlgorithm_H
#define GeneticAlgorithm_H

#include "neuralNetwork.h"
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <random>

class GeneticAlgorithm {
private:
    std::vector<int> fitness;

    // Helper function to find the most frequent element in a vector
    int most_frequent_element(const std::vector<int>& lst);

public:
    int nombre_individus;
    std::vector<NeuralNetwork> individus;

    GeneticAlgorithm(int nombre_individus, int nombre_neurone_entre, int nombre_neurone_cache, int nombre_neurone_sortie);

    int individual_action(int i);
    void reproduction();
    void mutation(int prob, int prob_0, int prob_inverse, float noise_value);
};

#endif // GeneticAlgorithm_H
