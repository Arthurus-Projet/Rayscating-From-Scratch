#ifndef COLLISION_H
#define COLLISION_H

#include "Player.h"

class Collision {
public:
    Player player;

public:
	Collision();

	Collision(Player player);

    std::pair<double, double> collision_0_90(int** map, double angle) const;
	std::pair<double, double> collision_90_180(int** map, double angle) const;
	std::pair<double, double> collision_270_360(int** map, double angle) const;
	std::pair<double, double> collision_180_270(int** map, double angle) const;
};

#endif 

