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



int main() {

    bool otherPlayerAlive = true;
    double hyp_reference_angle;

    std::cout << playerX2 << " " << playerY2 << std::endl;

    Player player = Player(playerX, playerY, speed, reference_angle, health);
    Controller controller = Controller(player);
    
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML Raycasting");

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
    for (int i = 0; i < rows; i++) {
        map[i] = new int[cols];
    }

    // Copy values from the static table to the dynamic arrays
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            map[i][j] = initial_map[i][j];
        }
    }





    

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
           controller.player.newAngle(-0.1);
        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) 
            controller.player.newAngle(0.1);
        
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
        
        std::cout << otherPlayerAlive << std::endl;
        //end other player view

        
        std::vector<sf::Sprite> spriteList;
        std::vector<double> distanceList;
        for(int r = controller.player.getAngle() + 30.0; r >= controller.player.getAngle() -31.0; r-= 1.0) {
            for (double y = 0.5; y >= 0; y-=0.5){
                i++;

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


        

        
        

        

        //std::cout  << controller.angleToEnemy(hyp, playerX2, playerY2, map) << playerY2 << "  map :"<<  lenMap<< std::endl;
        window.display();
    }

    return 0;
}
