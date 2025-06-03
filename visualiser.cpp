#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm> 
#include <cmath>
#include <cstdlib>
#include <vector>

#include "Headers/MathFunctions.h"
#include "Headers/Controller.h"
#include "Headers/Player.h"
#include "Headers/variables.h"
#include "Headers/Controller.h"
#include "Headers/neuralNetwork.h"
#include "Headers/GeneticAlgorithm.h"


std::pair<std::vector<double>, double> game(int action, Controller controller, double playerX2, double playerY2, int** map) {

    std::vector<double> input;
    double hyp_reference_angle;
    double hyp_reference_angle2;

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

    
    
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML Raycasting");
    window.setFramerateLimit(60);

    sf::Texture texture;
    if (!texture.loadFromFile("ressources/textures/bric.png")) {
        std::cerr << "Erreur lors du chargement de l'image" << std::endl;
        return -1;
    }

    sf::Texture textureSoldier;
    if (!textureSoldier.loadFromFile("ressources/sprites/soldier/0.png")) {
        // Afficher un message d'erreur si le fichier ne peut pas être chargé
        return -1;
    }


    sf::Vector2f rectangleSize(windowWidth, windowHeight / 2.f);

    sf::RectangleShape greyRectangle(rectangleSize);
    greyRectangle.setFillColor(sf::Color(113, 113, 113)); // grey
    greyRectangle.setPosition(0.f, windowHeight / 2.f); 

    sf::RectangleShape redRectangle(rectangleSize);
    redRectangle.setFillColor(sf::Color(97, 9, 0)); // red
    redRectangle.setPosition(0.f, 0.f); 

    // Memory allocation for dynamic arrays
    int rows = 13;
    int cols = 13;

    int** map = new int*[rows];
    for (int i = 0; i < rows; ++i) {
        map[i] = new int[cols];
    }

    // Copy values from the static table to the dynamic arrays
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            map[i][j] = initial_map[i][j];
        }
    }

    bool otherPlayerAlive = true;
    double hyp_reference_angle;

    

    

    for (size_t number_neural = 1; number_neural < 20; ++number_neural) {
        std::cout << "new one " << std::endl;
        std::cout << playerX2 << " " << playerY2 << std::endl;

        Player player = Player(playerX, playerY, speed, referenceAngle, health);
        Controller controller = Controller(player);

        // pour l'ia 
        Player player2 = Player(playerX2, playerY2, speed, referenceAngle2, health);
        Controller controller2 = Controller(player2);

        double number_state = game(0, controller, controller2.player.getX(), controller2.player.getY(), map).first.size();
        // permet de connaitre le nombre de neurone en entré (pour faire des changements dynamiques)
        std::cout << number_state << std::endl;

        int nombre_individus = 99;
        int nombre_neurone_entre = static_cast<int>(number_state);
        int nombre_neurone_cache = 100;
        int nombre_neurone_sortie = 5;
        bool debug = false;
        bool bool_fitness = false;
        int number_kill = 0;

        GeneticAlgorithm geneticAlgorithm = GeneticAlgorithm(nombre_individus, nombre_neurone_entre, nombre_neurone_cache, nombre_neurone_sortie);
        geneticAlgorithm.individus[0].load("neural_network" + std::to_string(number_neural) + ".dat");
        GeneticAlgorithm geneticAlgorithmOpponent = GeneticAlgorithm(nombre_individus, nombre_neurone_entre, nombre_neurone_cache, nombre_neurone_sortie);

        // quand l'ia joue contre son précédant réseau de neurone :
        if (number_neural > 1) {
            geneticAlgorithmOpponent.individus[0].load("neural_network" + std::to_string(number_neural - 1) + ".dat");
        }
        // fin de pour l'ia

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            
            float speed = 5.0f;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) 
                controller.player.moveForward(map);
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) 
                controller.player.moveBack(map);
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
            controller.player.newAngle(-2);
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) 
                controller.player.newAngle(2);
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
                bool_line = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
                bool_line = false;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
                if (controller.player.shoot(playerX2, playerY2, hyp_reference_angle))
                    otherPlayerAlive = false;

            window.clear();

            window.draw(greyRectangle);
            window.draw(redRectangle);
            
            int i = -1;

            // other player view

            double hyp = std::sqrt(std::pow((controller.player.getY() - playerY2), 2) + std::pow((controller.player.getX() - playerX2), 2));
            double cos_ = std::abs(controller.player.getX() - playerX2) / hyp;
            double angle_degrees = radToDeg(std::acos(cos_));
            double distancePlayer = 0.;
            sf::Sprite spriteSoldier;
            spriteSoldier.setTexture(textureSoldier);

            // for the right screen

            double hyp2 = std::sqrt(std::pow((controller2.player.getY() - playerY), 2) + std::pow((controller2.player.getX() - playerX), 2));
            double cos_2 = std::abs(controller2.player.getX() - playerX2) / hyp2;
            double angle_degrees2 = radToDeg(std::acos(cos_2));
            double distancePlayer2 = 0.;

            double rayonPlayer = controller.playerInTheFieldOfVision(hyp, playerX2, playerY2);
            if (rayonPlayer != 100) {
                //std::cout << "je suis la 1" << std::endl;
                if (90 <= rayonPlayer && rayonPlayer <= 180) 
                    angle_degrees = 90 + 90 - angle_degrees;
                if (270 <= rayonPlayer && rayonPlayer <= 360)
                    angle_degrees = 270 + 270 - angle_degrees;
                if (180 <= rayonPlayer && rayonPlayer <= 270)
                    angle_degrees = 180 + angle_degrees;

                double angle = controller.player.getAngle() - angle_degrees;

                double difference_with_reference_angle = std::abs(rayonPlayer - controller.player.getAngle());

                // Fish eye correction
                double d = hyp * std::cos(degToRad(difference_with_reference_angle));
                distancePlayer = (windowHeight * 0.7) / (d + 0.00000001);

                double y_wall = ((windowHeight - distancePlayer) / 2);
                double ratio = 110. / 76.;
                double i_ = 61 + ((controller.player.getAngle() - rayonPlayer) * 2);

                // Créer un sprite avec la texture chargée
                
                // Redimensionner le sprite
                double scaleX = distancePlayer / ratio; // Mettre à l'échelle à 50% de la largeur originale
                double scaleY = distancePlayer; // Mettre à l'échelle à 50% de la hauteur originale
                spriteSoldier.setScale(scaleX / 76, scaleY / 110);
                spriteSoldier.setPosition(i_ * size, y_wall);        
            }

            // for the right screen

            double rayonPlayer2 = controller2.playerInTheFieldOfVision(hyp2, playerX, playerY);
            if (rayonPlayer2 != 100) {
                //std::cout << "je suis la 1" << std::endl;
                if (90 <= rayonPlayer2 && rayonPlayer <= 180) 
                    angle_degrees2 = 90 + 90 - angle_degrees2;
                if (270 <= rayonPlayer2 && rayonPlayer2 <= 360)
                    angle_degrees2 = 270 + 270 - angle_degrees2;
                if (180 <= rayonPlayer2 && rayonPlayer2 <= 270)
                    angle_degrees2 = 180 + angle_degrees2;

                double angle2 = controller2.player.getAngle() - angle_degrees2;

                double difference_with_reference_angle2 = std::abs(rayonPlayer2 - controller2.player.getAngle());

                // Fish eye correction
                double d2 = hyp2 * std::cos(degToRad(difference_with_reference_angle2));
                distancePlayer2 = (windowHeight * 0.7) / (d2 + 0.00000001);

                double y_wall2 = ((windowHeight - distancePlayer2) / 2);
                double ratio2 = 110. / 76.;
                double i_2 = 61 + ((controller2.player.getAngle() - rayonPlayer2) * 2);

                // Créer un sprite avec la texture chargée
                
                // Redimensionner le sprite
                double scaleX2 = distancePlayer2 / ratio2; // Mettre à l'échelle à 50% de la largeur originale
                double scaleY2 = distancePlayer2; // Mettre à l'échelle à 50% de la hauteur originale
                spriteSoldier.setScale(scaleX2 / 76, scaleY2 / 110);
                spriteSoldier.setPosition(i_2 * size, y_wall2);        
            }
            
          

            // action of the ia

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

            int action = geneticAlgorithm.individual_action(state, 0);
            bool kill_ = controller.player.takeAction(action, map, controller2.player.getX(), controller2.player.getY(), hyp_ref);

            // l'adversaire de l'ia

            bool kill_Opponent = false;
           
            if (number_neural > 1) {
                std::cout << "ee" << std::endl;
                int actionOpponent = geneticAlgorithmOpponent.individual_action(state, 0);
                kill_Opponent = controller2.player.takeAction(actionOpponent, map, controller.player.getX(), controller.player.getY(), hyp_ref2);
            }

            if (kill_) {
                std::cout << "le mec est mort " << std::endl;
                break;
            }

            if (kill_Opponent) 
                std::cout << "mon gars l'autre ia a tué le mec " << std::endl;

            // end action of the ia

            
            std::vector<sf::Sprite> spriteList;
            std::vector<double> distanceList;
            for(int r = controller.player.getAngle() + 30.0; r >= controller.player.getAngle() -31.0; --r) {
                for (double y = 0.5; y >= 0; y-=0.5){
                    ++i;

                    double rayon = modulo(r + y, 360);
                
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
                    double distance = (windowHeight * 0.7) / (d + 0.0000000001);
        
                    double yWall = ((windowHeight - distance) / 2);

                    // Obtenir les dimensions de l'image
                    sf::Vector2u textureSize = texture.getSize();
                    double rest;
                    if (modulo(pos.first, 1) == 0) {
                        rest = modulo(pos.second, 1);
                    }
                    else {
                        rest = modulo(pos.first, 1);
                    }

                    double startX = rest * textureSize.x;
                

                    sf::IntRect textureRect(startX, 0, size, textureSize.y);

                    // create spirite
                    sf::Sprite sprite;
                    sprite.setTexture(texture);
                    sprite.setTextureRect(textureRect); 

                    long double scaleY = distance / textureSize.y;
                    sprite.setScale(1, scaleY);
                    sprite.setPosition(i * size, yWall);

                    //window.draw(sprite);
                    spriteList.push_back(sprite);
                    distanceList.push_back(distance);

                    if (bool_line) {
                        sf::RectangleShape rectangle;
                        rectangle.setSize(sf::Vector2f(size, distance));
                        rectangle.setPosition(i * size, yWall);
                        rectangle.setFillColor(sf::Color::White);
                        window.draw(rectangle);
                    }
                    
                }
            }

            // for the right screen
            i = 123;
            for(int r2 = controller2.player.getAngle() + 30.0; r2 >= controller2.player.getAngle() -31.0; --r2) {
                for (double y = 0.5; y >= 0; y-=0.5){
                    ++i;

                    double rayon2 = modulo(r2 + y, 360);
                
                    // function return pos
                    std::pair<double, double> pos2 = controller2.positionWallToPlayer(rayon2, map);

                    double sideX2 = controller2.player.getX() - pos2.first;
                    double sideY2 = controller2.player.getY() - pos2.second;
                    double hyp2 = std::sqrt(sideX2 * sideX2 + sideY2 * sideY2);

                    double hyp_reference_angle2;
                    if (rayon2 == controller2.player.getAngle())
                        hyp_reference_angle2 = hyp2;

                    double differenceWithPlayer2 = std::abs(rayon2 - controller2.player.getAngle());

                    // fish eye correction
                    double d2 = hyp2 * std::cos(degToRad(differenceWithPlayer2));
                    double distance2 = (windowHeight * 0.7) / (d2 + 0.0000000001);
        
                    double yWall2 = ((windowHeight - distance2) / 2);

                    // Obtenir les dimensions de l'image
                    sf::Vector2u textureSize = texture.getSize();
                    double rest;
                    if (modulo(pos2.first, 1) == 0) {
                        rest = modulo(pos2.second, 1);
                    }
                    else {
                        rest = modulo(pos2.first, 1);
                    }

                    double startX2 = rest * textureSize.x;
                

                    sf::IntRect textureRect(startX2, 0, size, textureSize.y);

                    // create spirite
                    sf::Sprite sprite;
                    sprite.setTexture(texture);
                    sprite.setTextureRect(textureRect); 

                    long double scaleY2 = distance2 / textureSize.y;
                    sprite.setScale(1, scaleY2);
                    sprite.setPosition(i * size, yWall2);

                    //window.draw(sprite);
                    spriteList.push_back(sprite);
                    distanceList.push_back(distance2);

                    if (bool_line) {
                        sf::RectangleShape rectangle;
                        rectangle.setSize(sf::Vector2f(size, distance2));
                        rectangle.setPosition(i * size, yWall2);
                        rectangle.setFillColor(sf::Color::White);
                        window.draw(rectangle);
                    }
                    
                }
            }


            for (int i = 0; i < spriteList.size(); ++i) {
                if (distanceList[i] < distancePlayer)
                    window.draw(spriteList[i]);
            }

            if (rayonPlayer != 100 && otherPlayerAlive)
                window.draw(spriteSoldier);

            for (int i = 0; i < spriteList.size(); ++i) {
                if (distanceList[i] > distancePlayer)
                    window.draw(spriteList[i]);
            }


            //std::cout  << controller.angleToEnemy(hyp, playerX2, playerY2, map) << " angle  : "  <<  controller.player.getAngle() << "  map :"<<  lenMap<< std::endl;
            window.display();
        }


    }

    return 0;
}
