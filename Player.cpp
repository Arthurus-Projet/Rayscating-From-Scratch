#include "Headers/Player.h"
#include "Headers/MathFunctions.h"
#include <iostream>
#include <cmath>
#include <cstdlib>


Player::Player() : x(0.0), y(0.0), speed(0.0), angle(0.0), health(100) {}

Player::Player(double x, double y, double speed, double angle, int health)
    : x(x), y(y), speed(speed), angle(angle), health(health) {}

bool Player::moveForward(int** map) {

    double pos_y = y - (speed * 32) * std::sin(degToRad(angle));
    double pos_x = x + (speed * 32) * std::cos(degToRad(angle));
    if (map[static_cast<int>(pos_y)][static_cast<int>(pos_x)] == 0)
    {
        setX(x + speed * std::cos(degToRad(angle)));
        setY(y - speed * std::sin(degToRad(angle)));
        return true;
    }
    return false;
}

bool Player::moveBack(int** map) {

    double pos_y = y + (speed * 32) * std::sin(degToRad(angle));
    double pos_x = x - (speed * 32) * std::cos(degToRad(angle));
    if (map[static_cast<int>(pos_y)][static_cast<int>(pos_x)] == 0) {
        setX(x - speed * std::cos(degToRad(angle)));
        setY(y + speed * std::sin(degToRad(angle)));
        return true;
    }
    return false;
}

void Player::newAngle(double value) {
    setAngle(angle + value);
    if (angle <= 0)
        setAngle(360);
    else if (angle >= 360)
        setAngle(0);
}

bool Player::shoot(double XOtherPlayer, double YOtherPlayer, double hyp_reference_angle) {
    double hyp = std::sqrt( std::pow(x - XOtherPlayer, 2) + std::pow(y - YOtherPlayer, 2));
    double pos_y = y - hyp * std::sin(degToRad(angle));
    double pos_x = x + hyp * std::cos(degToRad(angle));
    if (hyp < hyp_reference_angle) {
        if (std::abs(XOtherPlayer - pos_x) < 0.2 && std::abs(YOtherPlayer - pos_y) < 0.2)
            return true;
    }
    return false;
}

double Player::getX() const {
    return x;
}

double Player::getY() const {
    return y;
}

double Player::getSpeed() const {
    return speed;
}

double Player::getAngle() const {
    return angle;
}

int Player::getHealth() const {
    return health;
}

// Méthodes pour modifier les membres privés
void Player::setX(double newX) {
    x = newX;
}

void Player::setY(double newY) {
    y = newY;
}

void Player::setSpeed(double newSpeed) {
    speed = newSpeed;
}

void Player::setAngle(double newAngle) {
    angle = newAngle;
}

void Player::setHealth(int newHealth) {
    health = newHealth;
}
