#ifndef COLLISION_H
#define COLLISION_H

std::pair<double, double> collision_0_90(double x, double y, double angle);
std::pair<double, double> collision_90_180(double x, double y, double angle);
std::pair<double, double> collision_270_360(double x, double y, double angle);
std::pair<double, double> collision_180_270(double x, double y, double angle);

#endif
