#include "Headers/MathFunctions.h"
#include <iostream>
#include <cmath>


double degToRad(double degree) {
    // Si l'angle est égal à zéro, le résultat est également zéro
    if (degree == 0.0) {
        return 0.0;
    }
    // Sinon, convertir les degrés en radians en utilisant la formule : radians = (degrés * PI) / 180
    else {
        return (degree * M_PI) / 180.0;
    }
}

double round_(double number) {
    return std::floor(number + 0.5);
}

double modulo(double numerateur, double denominateur) {
    if (numerateur < 0)
        return denominateur + numerateur;
    return fmod(numerateur, denominateur);
}

/*double modulo(double dividend, double divisor) {
    if (divisor == 0) {
        // Gérer le cas où le diviseur est zéro
        std::cerr << "Erreur : Division par zéro !" << std::endl;
        return 0; // Ou tout autre traitement souhaité
    }
    
    int quotient = dividend / divisor;
    int remainder = dividend - quotient * divisor;
    
    if (remainder < 0) {
        // Ajouter le diviseur au reste si le reste est négatif
        remainder += divisor;
    }
    
    return remainder;
}*/
