#ifndef BALLON_H
#define BALLON_H

#include <stdbool.h>
#include <math.h>

// Structure pour les ballons
typedef struct {
    float x;           // Position X (flottant pour un mouvement fluide)
    float y;           // Position Y
    float speed;       // Vitesse de déplacement
    int pathIndex;     // Index actuel dans le chemin suivi
    int *pathX;        // Tableau des coordonnées X du chemin
    int *pathY;        // Tableau des coordonnées Y du chemin
    int pathLength;    // Longueur du chemin
    bool active;       // Si le ballon est actif (visible à l'écran)
} Ballon;

// Prototypes des fonctions
Ballon* creerBallon(int startX, int startY, int *pathX, int *pathY, int pathLength, float speed);
void freeBalon(Ballon *ballon);
void updateBalon(Ballon *ballon);
void drawBalon(Ballon *ballon, int cellSize);

#endif // BALloN_H