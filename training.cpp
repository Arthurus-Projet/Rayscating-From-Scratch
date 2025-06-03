#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm> 
#include <cmath>
#include <cstdlib>
#include <vector>
#include <utility>
#include "Headers/variables.h"
#include "Headers/MathFunctions.h"
#include "Headers/Controller.h"
#include "Headers/Player.h"
#include "Headers/neuralNetwork.h"
#include "Headers/GeneticAlgorithm.h"



std::pair<std::vector<double>, double> game(int action, Controller controller, double playerX2, double playerY2, int** map) {

    std::vector<double> input;
    double hyp_reference_angle;

    int i = 0;
    for(int r = controller.player.getAngle() + 30.0; r >= controller.player.getAngle() -31.0; r-= 1.0) {
            for (double y = 0.5; y >= 0; y-=0.5){

                double rayon = modulo(r + y, 360);
                //double rayon = modulo(r, 360);
            
                // function return pos
                std::pair<double, double> pos = controller.positionWallToPlayer(rayon, map);

                double sideX = controller.player.getX() - pos.first;
                double sideY = controller.player.getY() - pos.second;
                double hyp = std::sqrt(sideX * sideX + sideY * sideY);

                if (rayon == controller.player.getAngle())
                    hyp_reference_angle = hyp;

                double differenceWithPlayer = std::abs(rayon - controller.player.getAngle());

                // fish eye correction
                double d = hyp * std::cos(degToRad(differenceWithPlayer));
                if (modulo(i, 10) == 0)
                    input.push_back(d);
                
                i++;
            }
        }

    double hypToEnemy = std::sqrt(std::pow((controller.player.getY() - playerY2), 2) + std::pow((controller.player.getX() - playerX2), 2));
    double angleEnemy = controller.angleToEnemy(hypToEnemy, playerX2, playerY2, map);
    input.push_back(angleEnemy);

    return std::make_pair(input, hyp_reference_angle);
}



int main() {


    // Memory allocation for dynamic arrays
    int rows = 13;
    int cols = 13;

    int** map = new int*[rows];
    for (int i = 0; i < rows; i++) {
        map[i] = new int[cols];
    }

    // Copy values from the static table to the dynamic arrays
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            map[i][j] = initial_map[i][j];
        }
    }


    Player player = Player(playerX, playerY, speed, referenceAngle, health);
    Controller controller = Controller(player);

    Player player2 = Player(playerX2, playerY2, speed, referenceAngle2, health);
    Controller controller2 = Controller(player);

    double number_state = game(0, controller, controller2.player.getX(), controller2.player.getY(), map).first.size();
    std::cout << number_state << std::endl;

    

    int nombre_individus = 99;
    int nombre_neurone_entre = static_cast<int>(number_state);
    int nombre_neurone_cache = 100;
    int nombre_neurone_sortie = 5;
    bool debug = false;
    bool bool_fitness = false;
    int number_kill = 0;

    GeneticAlgorithm geneticAlgorithm = GeneticAlgorithm(nombre_individus, nombre_neurone_entre, nombre_neurone_cache, nombre_neurone_sortie);
    GeneticAlgorithm geneticAlgorithmOpponent = GeneticAlgorithm(nombre_individus, nombre_neurone_entre, nombre_neurone_cache, nombre_neurone_sortie);
    // Quand l'ia va tuer son adversaire, le cerveau de l'ia sera dans cette objet

    // start loop
    for (int generation = 0; generation < 10000; generation++) {

        std::cout << "Generation : " << generation << std::endl;
        
        //GeneticAlgorithm geneticAlgorithm = GeneticAlgorithm(nombre_individus, nombre_neurone_entre, nombre_neurone_cache, nombre_neurone_sortie);
        std::vector<int> fitness;
        for (int individual = 0; individual < nombre_individus; ++individual) {
            
            int actionOpponent;
            bool kill_Opponent;

            // get the initial state of the 2 players
            //std::cout << "ref angle " << referenceAngle << std::endl;
            Player player = Player(playerX, playerY, speed, referenceAngle, health);
            Controller controller = Controller(player);
            //std::cout << playerX << " " << playerY << " " << playerX2 << " " << playerY2 <<std::endl;
            Player player2 = Player(playerX2, playerY2, speed, referenceAngle2, health);
            Controller controller2 = Controller(player2);

            std::pair<std::vector<double>, double> result = game(0, controller, controller2.player.getX(), controller2.player.getY(), map);
            std::vector<double> state = result.first;
            double hyp_ref = result.second;

            std::pair<std::vector<double>, double> result2 = game(0, controller2, controller.player.getX(), controller.player.getY(), map);
            std::vector<double> state2 = result2.first;
            double hyp_ref2 = result2.second;

            bool kill = false;

            // nombre de tour dans le jeu (i_)
            for (int i_ = 0; i_ < 1000; ++i_) {

                std::pair<std::vector<double>, double> result = game(0, controller, controller2.player.getX(), controller2.player.getY(), map);
                std::vector<double> state = result.first;
                double hyp_ref = result.second;

                std::pair<std::vector<double>, double> result2 = game(0, controller2, controller.player.getX(), controller.player.getY(), map);
                std::vector<double> state2 = result2.first;
                double hyp_ref2 = result2.second;

                /*std::cout << "Contenu de state: ";
                for (const double& value : state) {
                    std::cout << value << " ";
                }
                std::cout << std::endl;*/

                int action = geneticAlgorithm.individual_action(state, individual);

                
                if (number_kill > 0) {
                    actionOpponent = geneticAlgorithmOpponent.individual_action(state, 0);
                    kill_Opponent = controller2.player.takeAction(actionOpponent, map, controller.player.getX(), controller.player.getY(), hyp_ref2);
                    // true if the old ia kill the genetic algo
                }

                bool kill_ = controller.player.takeAction(action, map, controller2.player.getX(), controller2.player.getY(), hyp_ref);

                if (debug)
                    std::cout << action << " : " << controller.player.getX() << " "<< controller.player.getY() << " angle : " << controller.player.getAngle() << std::endl;

                
                if (kill_) {
                    ++number_kill;
                    std::cout << "nombre de kill total : " << number_kill << " generation "<< generation <<" individual " << individual << " i : "<< i_ <<  std::endl;
                    fitness.push_back(10000);
                    geneticAlgorithm.individus[individual].save("neural_network" + std::to_string(number_kill) + ".dat");
                    geneticAlgorithmOpponent.individus[0].load("neural_network" + std::to_string(number_kill) + ".dat");
                    break;
                } else if (kill_Opponent) {
                    std::cout << "The old IA kill this indivual " << individual << std::endl;
                    break;
                }

            }
            if (!kill) {
                fitness.push_back(static_cast<int>( (std::abs(controller.player.getX() - playerX) + std::abs(controller.player.getY() - playerY) )* 10));
                //fitness.push_back(static_cast<int>( std::abs(controller.player.getX() - playerX) + std::abs(controller.player.getY() - playerY) ));
                //fitness.push_back(1);
            }
        }

        if (bool_fitness) {
            std::cout << "Fitness : ";
            for (int i = 0; i < fitness.size(); ++i) {
                std::cout << fitness[i] << " ";
            }
            std::cout << std::endl;
        }

        geneticAlgorithm.reproduction(fitness);
        int proba = 200;
        geneticAlgorithm.mutation(proba, proba, proba, 0.01);

    }



    return 0;
}