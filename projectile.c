#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "projectile.h"
#include "raylib.h"

// Création d'un projectile
Projectile* createProjectile(float x, float y, Ballon *cible, float vitesse) {
    Projectile *projectile = (Projectile*) malloc(sizeof(Projectile));

    
    projectile->x = x;
    projectile->y = y;
    projectile->cible = cible;
    projectile->cibleX = cible->x;
    projectile->cibleY = cible->y;
    projectile->vitesse = vitesse;
    projectile->actif = true;
    
    return projectile;
}

// Libération de la mémoire d'un projectile
void freeProjectile(Projectile *projectile) {
    if (projectile == NULL) return;
    free(projectile);
}

// Mise à jour d'un projectile
void updateProjectile(Projectile *projectile, int *argentJoueur) {
    if (!projectile->actif) return;
    
    // Si la cible n'est plus actif, désactifr le projectile
    if (projectile->cible != NULL && !projectile->cible->actif) {
        projectile->actif = false;
        return;
    }
    
    // Mise à jour de la position cible si elle est encore actif
    if (projectile->cible != NULL && projectile->cible->actif) {
        projectile->cibleX = projectile->cible->x;
        projectile->cibleY = projectile->cible->y;
    }
    
    // Calculer la direction vers la cible
    float dx = projectile->cibleX - projectile->x;
    float dy = projectile->cibleY - projectile->y;
    
    // Normaliser la direction
    float length = sqrt(dx * dx + dy * dy);
    if (length > 0) {
        dx /= length;
        dy /= length;
    }
    
    // Déplacer le projectile
    projectile->x += dx * projectile->vitesse;
    projectile->y += dy * projectile->vitesse;
    
    // Vérifier si on a atteint la cible
    float distanceSquared = (projectile->x - projectile->cibleX) * (projectile->x - projectile->cibleX) +
                           (projectile->y - projectile->cibleY) * (projectile->y - projectile->cibleY);
    
    if (distanceSquared < 0.1f) {
        // Désactifr le projectile et la cible
        projectile->actif = false;
        if (projectile->cible != NULL) {
            projectile->cible->actif = false;
        }
        (*argentJoueur)++;
    }
}

// Dessin d'un projectile
void drawProjectile(Projectile *projectile, int cellSize) {
    if (!projectile->actif) return;
    
    // Calculer les coordonnées à l'écran
    float screenX = projectile->x * cellSize + cellSize / 2;
    float screenY = projectile->y * cellSize + cellSize / 2;
    
    // Dessiner le projectile comme un petit cercle jaune
    DrawCircle((int)screenX, (int)screenY, cellSize / 6, YELLOW);
}