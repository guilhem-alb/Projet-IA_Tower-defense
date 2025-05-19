#ifndef BALLON_H
#define BALLON_H

#include <stdbool.h>
#include <math.h>

// Structure pour les ballons
typedef struct {
    float x;           // Position X (flottant pour un mouvement fluide)
    float y;           // Position Y
    float vitesse;       // Vitesse de déplacement
    int cheminIndex;     // Index actuel dans le chemin suivi
    int *cheminX;        // Tableau des coordonnées X du chemin
    int *cheminY;        // Tableau des coordonnées Y du chemin
    int cheminLength;    // Longueur du chemin
    bool actif;       // Si le ballon est actif (visible à l'écran)
} Ballon;

// Prototypes des fonctions
Ballon* createBallon(int *cheminX, int *cheminY, int cheminLength, float speed);
void freeBallon(Ballon *ballon);
void updateBallon(Ballon *ballon, int *vieJoueur);
void drawBallon(Ballon *ballon, int cellSize);

#endif // BALLON_H