#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <stdbool.h>
#include "balloon.h"

// Structure pour les projectiles
typedef struct {
    float x;           // Position X (flottant pour un mouvement fluide)
    float y;           // Position Y
    float targetX;     // Position X de la cible
    float targetY;     // Position Y de la cible
    float speed;       // Vitesse de déplacement
    bool active;       // Si le projectile est actif (visible à l'écran)
    Balloon *target;   // Pointeur vers le ballon ciblé
} Projectile;

// Prototypes des fonctions
Projectile* createProjectile(float x, float y, Balloon *target, float speed);
void freeProjectile(Projectile *projectile);
void updateProjectile(Projectile *projectile);
void drawProjectile(Projectile *projectile, int cellSize);

#endif // PROJECTILE_H