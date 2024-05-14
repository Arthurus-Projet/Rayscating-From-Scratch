#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
    double x;
    double y;
    double speed;
    double angle;
    int health;

public:
    // Constructeur par défaut
    Player();

    // Constructeur surchargé
    Player(double x, double y, double speed, double angle, int health);

    // Méthodes pour accéder aux membres privés
    double getX() const;
    double getY() const;
    double getSpeed() const;
    double getAngle() const;
    int getHealth() const;

    // Méthodes pour modifier les membres privés
    void setX(double newX);
    void setY(double newY);
    void setSpeed(double newSpeed);
    void setAngle(double newAngle);
    void setHealth(int newHealth);
};

#endif // PLAYER_H
