#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <stdbool.h>
#include "balloon.h"

// Structure pour les projectiles
typedef struct {
    float x;           // Position X (flottant pour un mouvement fluide)
    float y;           // Position Y
    float cibleX;     // Position X de la cible
    float cibleY;     // Position Y de la cible
    float speed;       // Vitesse de déplacement
    bool active;       // Si le projectile est actif (visible à l'écran)
    Balloon *cible;   // Pointeur vers le ballon ciblé
} Projectile;

// Prototypes des fonctions
Projectile* createProjectile(float x, float y, Balloon *cible, float speed);
void freeProjectile(Projectile *projectile);
void updateProjectile(Projectile *projectile);
void drawProjectile(Projectile *projectile, int cellSize);

#endif // PROJECTILE_H