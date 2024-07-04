#ifndef COLLISION_H
#define COLLISION_H

#include "Player.h"

class Controller {
public:
    Player player;

public:
	Controller();

	Controller(Player player);

    std::pair<double, double> controller_0_90(int** map, double angle) const;
	std::pair<double, double> controller_90_180(int** map, double angle) const;
	std::pair<double, double> controller_270_360(int** map, double angle) const;
	std::pair<double, double> controller_180_270(int** map, double angle) const;
};

#endif 

