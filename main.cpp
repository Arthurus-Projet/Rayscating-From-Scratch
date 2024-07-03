#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm> 
#include <cmath>
#include <cstdlib>
#include "Headers/MathFunctions.h"
#include "Headers/Collision.h"
#include "Headers/Player.h"

double playerInTheFieldOfVision(double hyp, Collision collision, double posSoliderX, double posSoliderY) {
    for (int rayon = collision.player.getAngle() + 50; rayon >= collision.player.getAngle() - 30; rayon--) {
            int newRayon = modulo(rayon, 360);

            double pos_y = collision.player.getY() - hyp * std::sin(degToRad(newRayon));
            double pos_x = collision.player.getX() + hyp * std::cos(degToRad(newRayon));
           
            if (std::abs(pos_x - posSoliderX) < 0.1 && std::abs(pos_y - posSoliderY) < 0.1)
                return rayon;
        }
        return 100;
    }



int main() {
    double windowHeight = 800;
    double windowWidth = 496;

    double playerX2 = 1.3;
    double playerY2 = 1.353;

    double playerX = 4.4;
    double playerY = 6.2;
    int referenceAngle = 0;
    int r;
    double rayon;
    double size = 4;

    double speed = 0.0025;
    int reference_angle = 90;
    double health = 10;
    bool bool_line = true;
    Player player = Player(playerX, playerY, speed, reference_angle, health);
    Collision collision = Collision(player);
    
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



    int initial_map[13][13] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 2, 0, 0, 0, 0, 0, 1, 0, 0, 1},
        {1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    // Allocation de mémoire pour le tableau dynamique
    int rows = 13;
    int cols = 13;

    int** map = new int*[rows];
    for (int i = 0; i < rows; i++) {
        map[i] = new int[cols];
    }

    // Copie des valeurs du tableau statique vers le tableau dynamique
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            // to move forward
            double pos_y = collision.player.getY() - (collision.player.getSpeed() * 32) * std::sin(degToRad(collision.player.getAngle()));
            double pos_x = collision.player.getX() + (collision.player.getSpeed() * 32) * std::cos(degToRad(collision.player.getAngle()));
            if (map[static_cast<int>(pos_y)][static_cast<int>(pos_x)] == 0)
            {
                collision.player.setX(collision.player.getX() + collision.player.getSpeed() * std::cos(degToRad(collision.player.getAngle())));
                collision.player.setY(collision.player.getY() - collision.player.getSpeed() * std::sin(degToRad(collision.player.getAngle())));
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            // to move backwards
            double pos_y = collision.player.getY() + (collision.player.getSpeed() * 32) * std::sin(degToRad(collision.player.getAngle()));
            double pos_x = collision.player.getX() - (collision.player.getSpeed() * 32) * std::cos(degToRad(collision.player.getAngle()));
            if (map[static_cast<int>(pos_y)][static_cast<int>(pos_x)] == 0) {
            collision.player.setX(collision.player.getX() - collision.player.getSpeed() * std::cos(degToRad(collision.player.getAngle())));
            collision.player.setY(collision.player.getY() + collision.player.getSpeed() * std::sin(degToRad(collision.player.getAngle())));
            }
 
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            collision.player.setAngle(collision.player.getAngle() - 0.1);
            if (collision.player.getAngle() <= 0)
                collision.player.setAngle(360);  
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            collision.player.setAngle(collision.player.getAngle() + 0.1);
            if (collision.player.getAngle() >= 360)
                collision.player.setAngle(0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
            bool_line = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
            bool_line = false;
        }

        window.clear();

        window.draw(greyRectangle);
        window.draw(redRectangle);
        
        int i = -1;
        
        for(r = collision.player.getAngle() + 30.0; r >= collision.player.getAngle() -31.0; r-= 1.0) {
            for (double y = 0.5; y >= 0; y-=0.5){
                i++;

                rayon = modulo(r + y, 360);
                std::pair<double, double> pos;
                

                if (0.0 <= rayon == true && rayon < 90.0 == true) {
                    pos = collision.collision_0_90(map, rayon);
                }

                if (90 <= rayon == true && rayon < 180 == true) {
                    pos = collision.collision_90_180(map, rayon);
                }

                if (270 < rayon == true && rayon < 360 == true) {
                    pos = collision.collision_270_360(map, rayon);
                }

                if (180 <= rayon == true && rayon <= 270 == true) {
                    pos = collision.collision_180_270(map, rayon);
                }

                double sideX = collision.player.getX() - pos.first;
                double sideY = collision.player.getY() - pos.second;
                double hyp = std::sqrt(sideX * sideX + sideY * sideY);

                double differenceWithPlayer = std::abs(rayon - collision.player.getAngle());

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

                window.draw(sprite);

                if (bool_line) {
                    sf::RectangleShape rectangle;
                    rectangle.setSize(sf::Vector2f(size, distance));
                    rectangle.setPosition(i * size, yWall);
                    rectangle.setFillColor(sf::Color::White);
                    window.draw(rectangle);
                }
                
            }
        }

        // Print other player 

        
        

        double hyp = std::sqrt(std::pow((collision.player.getY() - playerY2), 2) + std::pow((collision.player.getX() - playerX2), 2));
        double cos_ = std::abs(collision.player.getX() - playerX2) / hyp;
        double angle_degrees = radToDeg(std::acos(cos_));

        double rayon = playerInTheFieldOfVision(hyp, collision, playerX2, playerY2);
        if (rayon != 100) {
            //std::cout << "je suis la 1" << std::endl;
            if (90 <= rayon && rayon <= 180) 
                angle_degrees = 90 + 90 - angle_degrees;
            if (270 <= rayon && rayon <= 360)
                angle_degrees = 270 + 270 - angle_degrees;
            if (180 <= rayon && rayon <= 270)
                angle_degrees = 180 + angle_degrees;

            double angle = collision.player.getAngle() - angle_degrees;

            double difference_with_reference_angle = std::abs(rayon - collision.player.getAngle());

            // Fish eye correction
            double d = hyp * std::cos(degToRad(difference_with_reference_angle));
            double distance = (windowHeight * 0.7) / (d + 0.00000001);

            double y_wall = ((windowHeight - distance) / 2);

            double ratio = 110. / 76.;

            double i_ = 61 + ((collision.player.getAngle() - rayon) * 2);

            // Créer un sprite avec la texture chargée
            sf::Sprite spriteSoldier;
            spriteSoldier.setTexture(textureSoldier);

            // Redimensionner le sprite
            double scaleX = distance / ratio; // Mettre à l'échelle à 50% de la largeur originale
            double scaleY = distance; // Mettre à l'échelle à 50% de la hauteur originale
            spriteSoldier.setScale(scaleX / 76, scaleY / 110);
            spriteSoldier.setPosition(i_ * size, y_wall);

            window.draw(spriteSoldier);
            //std::cout << ratio << "je suis la " << scaleX << " " << scaleY << " " << i_ * size << " " << y_wall << std::endl;

        }


        window.display();
    }

    return 0;
}
