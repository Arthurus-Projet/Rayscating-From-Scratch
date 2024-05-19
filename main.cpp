#include <SFML/Graphics.hpp>
#include <iostream>
#include <algorithm> 
#include <cmath>
#include "Headers/MathFunctions.h"
#include "Headers/Collision.h"
#include "Headers/Player.h"


int main() {

    double playerX = 4.4;
    double playerY = 6.2;
    int referenceAngle = 90;
    int r;
    double rayon;
    double size = 4;
    double windowHeight = 800;
    double windowWidth = 700;
    double speed = 0.01;
    int reference_angle = 90;
    double health = 10;
    bool bool_line = true;
    Player player = Player(playerX, playerY, speed, reference_angle, health);
    Collision collision = Collision(player);
    
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML Raycasting");

    sf::Texture texture;
    if (!texture.loadFromFile("ressources/bric.png")) {
        std::cerr << "Erreur lors du chargement de l'image" << std::endl;
        return -1;
    }


    int map[13][13] = {
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
    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        
        float speed = 5.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            // to move forward
            collision.player.setX(collision.player.getX() + collision.player.getSpeed() * std::cos(degToRad(collision.player.getAngle())));
            collision.player.setY(collision.player.getY() - collision.player.getSpeed() * std::sin(degToRad(collision.player.getAngle())));
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            // to move backwards
            collision.player.setX(collision.player.getX() - collision.player.getSpeed() * std::cos(degToRad(collision.player.getAngle())));
            collision.player.setY(collision.player.getY() + collision.player.getSpeed() * std::sin(degToRad(collision.player.getAngle())));
 
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            collision.player.setAngle(collision.player.getAngle() - 1);
            if (collision.player.getAngle() == -1)
                collision.player.setAngle(359);
            
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            collision.player.setAngle(collision.player.getAngle() + 1);
            if (collision.player.getAngle() == 360)
                collision.player.setAngle(0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::N)) {
            bool_line = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::B)) {
            bool_line = false;
        }

        window.clear();

        
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
                    rectangle.setFillColor(sf::Color::Blue);
                    window.draw(rectangle);
                }
                
            }
        }
    
        window.display();
    }

    return 0;
}
