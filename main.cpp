#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include "Headers/MathFunctions.h"
#include "Headers/Collision.h"

double playerX = 4.4;
double playerY = 6.2;
int referenceAngle = 90;
int r;
double rayon;
double size = 4;
double windowHeight = 800;

int main() {
    sf::RenderWindow window(sf::VideoMode(windowHeight, windowHeight), "SFML Raycasting");


    
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        
        float speed = 5.0f;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
            // to move forward
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            // to move backwards
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
            referenceAngle--;
            if (referenceAngle == -1)
                referenceAngle = 359;
            
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            referenceAngle++;
            if (referenceAngle == 360)
                referenceAngle = 0;
        }

        window.clear();
        int i = 0;
        for(r = referenceAngle + 30.0; r >= referenceAngle -31.0; r-= 1.0) {
            rayon = modulo(r, 360);
            std::pair<double, double> pos;

            if (0.0 <= rayon == true && rayon < 90.0 == true) {
                degToRad(3.9);
                pos = collision_0_90(4.4, 6.2, rayon);
            }

            if (90 <= rayon == true && rayon < 180 == true) {
                pos = collision_90_180(4.4, 6.2, rayon);
            }

            if (270 < rayon == true && rayon < 360 == true) {
                pos = collision_270_360(4.4, 6.2, rayon);
            }

            if (180 <= rayon == true && rayon <= 270 == true) {
                pos = collision_180_270(4.4, 6.2, rayon);
            }

            double sideX = 4.4 - pos.first;
            double sideY = 6.2 - pos.second;
            double hyp = std::sqrt(sideX * sideX + sideY * sideY);

            double differenceWithPlayer = std::abs(rayon - referenceAngle);

            // fish eye correction
            double d = hyp * std::cos(degToRad(differenceWithPlayer));
            double distance = (windowHeight * 0.7) / (d + 0.0000000001);
 
            double yWall = ((windowHeight - distance) / 2);

            // Création d'un rectangle
            sf::RectangleShape rectangle;
            rectangle.setSize(sf::Vector2f(size, distance));
            rectangle.setPosition(i * size, yWall);
            rectangle.setFillColor(sf::Color::Green);

            // Dessiner le rectangle
            window.draw(rectangle);
            i++;
        }

        
        window.display();
    }

    return 0;
}
