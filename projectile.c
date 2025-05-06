#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "projectile.h"
#include "raylib.h"

// Création d'un projectile
Projectile* createProjectile(float x, float y, Balloon *target, float speed) {
    Projectile *projectile = (Projectile*) malloc(sizeof(Projectile));

    
    projectile->x = x;
    projectile->y = y;
    projectile->target = target;
    projectile->targetX = target->x;
    projectile->targetY = target->y;
    projectile->speed = speed;
    projectile->active = true;
    
    return projectile;
}

// Libération de la mémoire d'un projectile
void freeProjectile(Projectile *projectile) {
    if (projectile == NULL) return;
    free(projectile);
}

// Mise à jour d'un projectile
void updateProjectile(Projectile *projectile) {
    if (!projectile->active) return;
    
    // Si la cible n'est plus active, désactiver le projectile
    if (projectile->target != NULL && !projectile->target->active) {
        projectile->active = false;
        return;
    }
    
    // Mise à jour de la position cible si elle est encore active
    if (projectile->target != NULL && projectile->target->active) {
        projectile->targetX = projectile->target->x;
        projectile->targetY = projectile->target->y;
    }
    
    // Calculer la direction vers la cible
    float dx = projectile->targetX - projectile->x;
    float dy = projectile->targetY - projectile->y;
    
    // Normaliser la direction
    float length = sqrt(dx * dx + dy * dy);
    if (length > 0) {
        dx /= length;
        dy /= length;
    }
    
    // Déplacer le projectile
    projectile->x += dx * projectile->speed;
    projectile->y += dy * projectile->speed;
    
    // Vérifier si on a atteint la cible
    float distanceSquared = (projectile->x - projectile->targetX) * (projectile->x - projectile->targetX) +
                           (projectile->y - projectile->targetY) * (projectile->y - projectile->targetY);
    
    if (distanceSquared < 0.1f) {
        // Désactiver le projectile et la cible
        projectile->active = false;
        if (projectile->target != NULL) {
            projectile->target->active = false;
        }
    }
}

// Dessin d'un projectile
void drawProjectile(Projectile *projectile, int cellSize) {
    if (!projectile->active) return;
    
    // Calculer les coordonnées à l'écran
    float screenX = projectile->x * cellSize + cellSize / 2;
    float screenY = projectile->y * cellSize + cellSize / 2;
    
    // Dessiner le projectile comme un petit cercle jaune
    DrawCircle((int)screenX, (int)screenY, cellSize / 6, YELLOW);
}