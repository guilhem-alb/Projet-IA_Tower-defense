#include <stdlib.h>
#include <stdio.h>
#include "ballon.h"
#include "raylib.h"

// Création d'un ballon
Ballon* createBallon(int startX, int startY, int *pathX, int *pathY, int pathLength, float speed) {
    Ballon *ballon = (Ballon*) malloc(sizeof(Ballon));

    
    // Position initiale (au centre de la cellule)
    ballon->x = (float)startX;
    ballon->y = (float)startY;
    ballon->speed = speed;
    ballon->pathIndex = 0;
    ballon->pathX = pathX;
    ballon->pathY = pathY;
    ballon->pathLength = pathLength;
    ballon->active = true;
    
    return ballon;
}

// Libération de la mémoire d'un ballon
void freeBallon(Ballon *ballon) {
    if (ballon == NULL) return;
    
    // Ne pas libérer pathX et pathY car ils sont partagés entre plusieurs ballons
    free(ballon);
}

// Mise à jour de la position d'un ballon
void updateBallon(Ballon *ballon) {
    if (!ballon->active || ballon->pathIndex >= ballon->pathLength - 1) {
        ballon->active = false;
        return;
    }
    
    // Coordonnées cibles (où le ballon se dirige)
    float targetX = (float)ballon->pathX[ballon->pathIndex + 1];
    float targetY = (float)ballon->pathY[ballon->pathIndex + 1];
    
    // Direction vers la cible 
    float dx = targetX - ballon->x;
    float dy = targetY - ballon->y;
    
    // Normaliser la direction
    float length = sqrt(dx * dx + dy * dy);
    if (length > 0) {
        dx /= length;
        dy /= length;
    }
    
    // Déplacer le ballon
    ballon->x += dx * ballon->speed;
    ballon->y += dy * ballon->speed;
    
    // Vérifier si on a atteint la cible
    float distanceSquared = (ballon->x - targetX) * (ballon->x - targetX) +
                           (ballon->y - targetY) * (ballon->y - targetY);
    
    if (distanceSquared < ballon->speed * ballon->speed) {
        // Passer au prochain point du chemin
        ballon->pathIndex++;
        
        // Vérifier si on a atteint la fin du chemin
        if (ballon->pathIndex >= ballon->pathLength - 1) {
            ballon->active = false;
        }
    }
}

// Dessin d'un ballon
void drawBallon(Ballon *ballon, int cellSize) {
    if (!ballon->active) return;
    
    // Calculer les coordonnées à l'écran
    float screenX = ballon->x * cellSize + cellSize / 2;
    float screenY = ballon->y * cellSize + cellSize / 2;
    
    // Dessiner le ballon comme un cercle
    DrawCircle((int)screenX, (int)screenY, cellSize / 3, RED);
}