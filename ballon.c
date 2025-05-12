#include <stdlib.h>
#include <stdio.h>
#include "ballon.h"
#include "raylib.h"

// Création d'un ballon
Ballon* createBallon(int startX, int startY, int *cheminX, int *cheminY, int cheminLength, float vitesse) {
    Ballon *ballon = (Ballon*) malloc(sizeof(Ballon));

    
    // Position initiale (au centre de la cellule)
    ballon->x = (float)startX;
    ballon->y = (float)startY;
    ballon->vitesse = vitesse;
    ballon->cheminIndex = 0;
    ballon->cheminX = cheminX;
    ballon->cheminY = cheminY;
    ballon->cheminLength = cheminLength;
    ballon->actif = true;
    
    return ballon;
}

// Libération de la mémoire d'un ballon
void freeBallon(Ballon *ballon) {
    if (ballon == NULL) return;
    
    // Ne pas libérer cheminX et cheminY car ils sont partagés entre plusieurs ballons
    free(ballon);
}

// Mise à jour de la position d'un ballon
void updateBallon(Ballon *ballon) {
    if (!ballon->actif || ballon->cheminIndex >= ballon->cheminLength - 1) {
        ballon->actif = false;
        return;
    }
    
    // Coordonnées cibles (où le ballon se dirige)
    float targetX = (float)ballon->cheminX[ballon->cheminIndex + 1];
    float targetY = (float)ballon->cheminY[ballon->cheminIndex + 1];
    
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
    ballon->x += dx * ballon->vitesse;
    ballon->y += dy * ballon->vitesse;
    
    // Vérifier si on a atteint la cible
    float distanceSquared = (ballon->x - targetX) * (ballon->x - targetX) +
                           (ballon->y - targetY) * (ballon->y - targetY);
    
    if (distanceSquared < ballon->vitesse * ballon->vitesse) {
        // Passer au prochain point du chemin
        ballon->cheminIndex++;
        
        // Vérifier si on a atteint la fin du chemin
        if (ballon->cheminIndex >= ballon->cheminLength - 1) {
            ballon->actif = false;
        }
    }
}

// Dessin d'un ballon
void drawBallon(Ballon *ballon, int cellSize) {
    if (!ballon->actif) return;
    
    // Calculer les coordonnées à l'écran
    float screenX = ballon->x * cellSize + cellSize / 2;
    float screenY = ballon->y * cellSize + cellSize / 2;
    
    // Dessiner le ballon comme un cercle
    DrawCircle((int)screenX, (int)screenY, cellSize / 3, RED);
}