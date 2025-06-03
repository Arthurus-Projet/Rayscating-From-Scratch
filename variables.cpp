#include "Headers/variables.h"
#include "Headers/MathFunctions.h"



int lenMap = 13;

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

double windowHeight = 550;
double windowWidth = 488 * 2;

double playerX = 5.5;
double playerY = 4.3;

double playerX2 = lenMap / 2. + (lenMap / 2. - playerX);
double playerY2 = lenMap / 2. + (lenMap / 2. - playerY);

int referenceAngle = 0;

int referenceAngle2 = modulo(reference_angle + 180, 360);

double size = 4;

//double speed = 0.0025;
double speed = 0.08;
int reference_angle = 90;
double health = 10;
bool bool_line = true;
