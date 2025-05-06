#include <stdlib.h>
#include <stdio.h>
#include "balloon.h"
#include "raylib.h"

// Création d'un ballon
Balloon* createBalloon(int startX, int startY, int *pathX, int *pathY, int pathLength, float speed) {
    Balloon *balloon = (Balloon*) malloc(sizeof(Balloon));

    
    // Position initiale (au centre de la cellule)
    balloon->x = (float)startX;
    balloon->y = (float)startY;
    balloon->speed = speed;
    balloon->pathIndex = 0;
    balloon->pathX = pathX;
    balloon->pathY = pathY;
    balloon->pathLength = pathLength;
    balloon->active = true;
    
    return balloon;
}

// Libération de la mémoire d'un ballon
void freeBalloon(Balloon *balloon) {
    if (balloon == NULL) return;
    
    // Ne pas libérer pathX et pathY car ils sont partagés entre plusieurs ballons
    free(balloon);
}

// Mise à jour de la position d'un ballon
void updateBalloon(Balloon *balloon) {
    if (!balloon->active || balloon->pathIndex >= balloon->pathLength - 1) {
        balloon->active = false;
        return;
    }
    
    // Coordonnées cibles (où le ballon se dirige)
    float targetX = (float)balloon->pathX[balloon->pathIndex + 1];
    float targetY = (float)balloon->pathY[balloon->pathIndex + 1];
    
    // Direction vers la cible
    float dx = targetX - balloon->x;
    float dy = targetY - balloon->y;
    
    // Normaliser la direction
    float length = sqrt(dx * dx + dy * dy);
    if (length > 0) {
        dx /= length;
        dy /= length;
    }
    
    // Déplacer le ballon
    balloon->x += dx * balloon->speed;
    balloon->y += dy * balloon->speed;
    
    // Vérifier si on a atteint la cible
    float distanceSquared = (balloon->x - targetX) * (balloon->x - targetX) +
                           (balloon->y - targetY) * (balloon->y - targetY);
    
    if (distanceSquared < balloon->speed * balloon->speed) {
        // Passer au prochain point du chemin
        balloon->pathIndex++;
        
        // Vérifier si on a atteint la fin du chemin
        if (balloon->pathIndex >= balloon->pathLength - 1) {
            balloon->active = false;
        }
    }
}

// Dessin d'un ballon
void drawBalloon(Balloon *balloon, int cellSize) {
    if (!balloon->active) return;
    
    // Calculer les coordonnées à l'écran
    float screenX = balloon->x * cellSize + cellSize / 2;
    float screenY = balloon->y * cellSize + cellSize / 2;
    
    // Dessiner le ballon comme un cercle
    DrawCircle((int)screenX, (int)screenY, cellSize / 3, RED);
}