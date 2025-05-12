#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "tower.h"
#include "projectile.h"
#include "raylib.h"

// Création d'une tour
Tower* createTower(int x, int y, int range, float reloadTime) {
    Tower *tower = (Tower*) malloc(sizeof(Tower));

    
    tower->x = x;
    tower->y = y;
    tower->range = range;
    tower->reloadTime = reloadTime;
    tower->reloadCounter = 0;
    tower->active = true;
    
    return tower;
}

// Libération de la mémoire d'une tour
void freeTower(Tower *tower) {
    if (tower == NULL) return;
    free(tower);
}

// Mise à jour de la tour et tir sur les ballons
Projectile* updateTower(Tower *tower, Balloon **balloons, int balloonCount) {
    if (!tower->active) return NULL;
    
    // Diminuer le compteur de rechargement
    if (tower->reloadCounter > 0) {
        tower->reloadCounter -= GetFrameTime();
    }
    
    // Si prêt à tirer, trouver le ballon le plus proche dans la portée
    if (tower->reloadCounter <= 0) {
        Balloon *targetBalloon = NULL;
        float minDistance = (float)(tower->range * tower->range);
        
        for (int i = 0; i < balloonCount; i++) {
            if (balloons[i] != NULL && balloons[i]->active) {
                // Calculer la distance au ballon
                float dx = tower->x - balloons[i]->x;
                float dy = tower->y - balloons[i]->y;
                float distSquared = dx * dx + dy * dy;
                
                // Vérifier si dans la portée et plus proche que la cible actuelle
                if (distSquared <= minDistance) {
                    minDistance = distSquared;
                    targetBalloon = balloons[i];
                }
            }
        }
        
        // Si trouvé une cible, créer un projectile
        if (targetBalloon != NULL) {
            // Réinitialiser le compteur de rechargement
            tower->reloadCounter = tower->reloadTime;
            
            // Créer et retourner un nouveau projectile
            return createProjectile((float)tower->x, (float)tower->y, targetBalloon, 0.2f);
        }
    }
    
    return NULL;
}

// Dessin de la tour
void drawTower(Tower *tower, int cellSize) {
    if (!tower->active) return;
    
    // Calculer les coordonnées à l'écran
    float screenX = tower->x * cellSize + cellSize / 2;
    float screenY = tower->y * cellSize + cellSize / 2;
    
    // Dessiner la tour comme un carré bleu
    DrawRectangle((int)screenX - cellSize/4, (int)screenY - cellSize/4, 
                 cellSize/2, cellSize/2, BLUE);
    
    // Dessiner la portée de la tour (comme un cercle) quand elle recharge
    if (tower->reloadCounter > 0) {
        DrawCircleLines((int)screenX, (int)screenY, 
                       tower->range * cellSize, SKYBLUE);
    }
}

// Vérifier si une tour peut être placée à une position spécifique
bool canPlaceTower(Labyrinthe *maze, int x, int y) {
    // Vérifier si la position est valide
    if (!isValid(maze, x, y)) return false;
    
    // Vérifier si la cellule est un mur
    if (maze->grille[y][x].type != MUR) return false;
    
    // Vérifier si au moins une cellule adjacente est un chemin
    // Cela garantit que les tours sont uniquement placées sur des murs adjacents aux chemins
    const int dx[4] = {0, 1, 0, -1};  // Haut, Droite, Bas, Gauche
    const int dy[4] = {-1, 0, 1, 0};
    
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        if (isValid(maze, nx, ny) && maze->grille[ny][nx].type == CHEMIN) {
            return true;
        }
    }
    
    return false;
}