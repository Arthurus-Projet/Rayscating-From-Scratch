#include "Headers/Player.h"


Player::Player() : x(0.0), y(0.0), speed(0.0), angle(0.0), health(100) {}

Player::Player(double x, double y, double speed, double angle, int health)
    : x(x), y(y), speed(speed), angle(angle), health(health) {}


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
