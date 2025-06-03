#include "Headers/MathFunctions.h"
#include <iostream>
#include <cmath>


double degToRad(double degree) {
    if (degree == 0.0) {
        return 0.0;
    } else {
        return (degree * M_PI) / 180.0;
    }
}


double radToDeg(double radians) {
    return radians * (180.0 / M_PI);
}

double round_(double number) {
    return std::floor(number + 0.5);
}

double modulo(double numerateur, double denominateur) {
    if (numerateur < 0)
        return denominateur + numerateur;
    return fmod(numerateur, denominateur);
}


