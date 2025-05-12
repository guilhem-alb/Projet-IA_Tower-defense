#include <stdlib.h>
#include <stdio.h>
#include "raylib.h"
#include "maze.h"
#include "astar.h"
#include "ballon.h"
#include "tower.h"
#include "projectile.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define CELL_SIZE 40

#define MAX_BALLONS 100
#define MAX_TOWERS 50
#define MAX_PROJECTILES 200

int main(void) {
    // Initialisation de la fenêtre
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ballon laby Defense");
    SetTargetFPS(60);
    
    // Calcul de la taille du labyrinthe en fonction de la taille de l'écran
    int labyWidth = SCREEN_WIDTH / CELL_SIZE;
    int labyHeight = SCREEN_HEIGHT / CELL_SIZE;
    
    // Création du labyrinthe
    Labyrinthe *laby = creerLabyrinthe(labyWidth, labyHeight, 2, 1);  // 1 entrée, 1 sortie

    
    // Génération du labyrinthe
    genererLabyrinthe(laby);
    
    // Calcul du chemin avec A*
    int pathLength = 0;
    int **path = recherche_astar(laby, laby->entreeX[0], laby->entreeY[0], 
                             laby->sortieX[0], laby->sortieY[0], &pathLength);
    

    // Création des ballons
    Ballon *ballons[MAX_BALLONS];
    int ballonCount = 0;
    int frameCounter = 0;
    
    // Création des tours
    Tower *towers[MAX_TOWERS];
    int towerCount = 0;
    
    // Création des projectiles
    Projectile *projectiles[MAX_PROJECTILES];
    int projectileCount = 0;
    
    // Boucle principale
    while (!WindowShouldClose()) {
        // Mise à jour
        frameCounter++;
        
        // Créer un nouveau ballon toutes les 15 frames
        if (frameCounter >= 15 && ballonCount < MAX_BALLONS) {
            ballons[ballonCount] = createBallon(laby->entreeX[0], laby->entreeY[0], 
                                                 path[0], path[1], pathLength, 0.07f);
            if (ballons[ballonCount] != NULL) {
                ballonCount++;
            }
            frameCounter = 0;
        }
        
        // Mise à jour de tous les ballons
        for (int i = 0; i < ballonCount; i++) {
            if (ballons[i] != NULL && ballons[i]->actif) {
                updateBallon(ballons[i]);
            }
        }
        
        // Mise à jour de toutes les tours et création de projectiles
        for (int i = 0; i < towerCount; i++) {
            if (towers[i] != NULL && towers[i]->actif) {
                Projectile *newProjectile = updateTower(towers[i], ballons, ballonCount);
                if (newProjectile != NULL && projectileCount < MAX_PROJECTILES) {
                    projectiles[projectileCount++] = newProjectile;
                }
            }
        }
        
        // Mise à jour de tous les projectiles
        for (int i = 0; i < projectileCount; i++) {
            if (projectiles[i] != NULL && projectiles[i]->actif) {
                updateProjectile(projectiles[i]);
            }
        }
        
        // Nettoyage des projectiles inactifs
        for (int i = 0; i < projectileCount; i++) {
            if (projectiles[i] != NULL && !projectiles[i]->actif) {
                freeProjectile(projectiles[i]);
                projectiles[i] = NULL;
                
                // Décaler les projectiles restants
                for (int j = i; j < projectileCount - 1; j++) {
                    projectiles[j] = projectiles[j + 1];
                }
                projectileCount--;
                i--; // Pour ne pas sauter un élément après décalage
            }
        }
        
        // Gestion des clics pour placer des tours
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            int gridX = (int)(mousePos.x / CELL_SIZE);
            int gridY = (int)(mousePos.y / CELL_SIZE);
            
            if (canPlaceTower(laby, gridX, gridY) && towerCount < MAX_TOWERS) {
                towers[towerCount] = createTower(gridX, gridY, 3, 1.0f);
                if (towers[towerCount] != NULL) {
                    towerCount++;
                }
            }
        }
        
        // Dessin
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Dessiner le labyrinthe
        dessinerLabyrinthe(laby, CELL_SIZE);
        
        // Dessiner les ballons
        for (int i = 0; i < ballonCount; i++) {
            if (ballons[i] != NULL && ballons[i]->actif) {
                drawBallon(ballons[i], CELL_SIZE);
            }
        }
        
        // Dessiner les tours
        for (int i = 0; i < towerCount; i++) {
            if (towers[i] != NULL && towers[i]->actif) {
                drawTower(towers[i], CELL_SIZE);
            }
        }
        
        // Dessiner les projectiles
        for (int i = 0; i < projectileCount; i++) {
            if (projectiles[i] != NULL && projectiles[i]->actif) {
                drawProjectile(projectiles[i], CELL_SIZE);
            }
        }
                
        EndDrawing();
    }
    
    // Libération de la mémoire
    for (int i = 0; i < ballonCount; i++) {
        if (ballons[i] != NULL) {
            freeBallon(ballons[i]);
        }
    }
    
    for (int i = 0; i < towerCount; i++) {
        if (towers[i] != NULL) {
            freeTower(towers[i]);
        }
    }
    
    for (int i = 0; i < projectileCount; i++) {
        if (projectiles[i] != NULL) {
            freeProjectile(projectiles[i]);
        }
    }
    
    if (path != NULL) {
        free(path[0]);
        free(path[1]);
        free(path);
    }
    
    freeLabyrinthe(laby);
    
    // Fermeture de la fenêtre
    CloseWindow();
    
    return 0;
}