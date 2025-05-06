#ifndef BALLOON_H
#define BALLOON_H

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
} Balloon;

// Prototypes des fonctions
Balloon* createBalloon(int startX, int startY, int *pathX, int *pathY, int pathLength, float speed);
void freeBalloon(Balloon *balloon);
void updateBalloon(Balloon *balloon);
void drawBalloon(Balloon *balloon, int cellSize);

#endif // BALLOON_H