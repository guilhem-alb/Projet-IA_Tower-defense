#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <stdbool.h>
#include "ballon.h"

// Structure pour les projectiles
typedef struct {
    float x;           // Position X (flottant pour un mouvement fluide)
    float y;           // Position Y
    float cibleX;     // Position X de la cible
    float cibleY;     // Position Y de la cible
    float vitesse;       // Vitesse de déplacement
    bool actif;       // Si le projectile est actif (visible à l'écran)
    Ballon *cible;   // Pointeur vers le ballon ciblé
} Projectile;

// Prototypes des fonctions
Projectile* createProjectile(float x, float y, Ballon *cible, float vitesse);
void freeProjectile(Projectile *projectile);
void updateProjectile(Projectile *projectile, int *argentJoueur);
void drawProjectile(Projectile *projectile, int cellSize);

#endif // PROJECTILE_H