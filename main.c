#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "raylib.h"
#include "maze.h"
#include "astar.h"
#include "ballon.h"
#include "tower.h"
#include "projectile.h"

#define SCREEN_WIDTH 1000
#define SCREEN_HEIGHT 800
#define CELL_SIZE 40

#define MAX_BALLONS 2
#define MAX_TOWERS 50
#define MAX_PROJECTILES 200
#define MAX_VAGUES 3

int main(void) {
    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));
    // Initialisation de la fenêtre
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Ballon laby Defense");
    SetTargetFPS(60);
    // Calcul de la taille du labyrinthe en fonction de la taille de l'écran
    int labyWidth = SCREEN_WIDTH / CELL_SIZE;
    int labyHeight = SCREEN_HEIGHT / CELL_SIZE;
    
    // Création du labyrinthe
    Labyrinthe *laby = initialiserLabyrinthe(labyWidth, labyHeight, 2, 1);  // 1 entrée, 1 sortie
    
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
    
    int vaguesCount = 0;
    bool waveCompleted = false;
    
    // Boucle principale
    while (!WindowShouldClose()) {
        // Mise à jour
        frameCounter++;

        // Vérifier si tous les ballons ont été créés pour cette vague
        if (ballonCount >= MAX_BALLONS && !waveCompleted) {
            // Vérifier combien de ballons sont encore actifs
            int activeBalloonsCount = 0;
            for (int i = 0; i < ballonCount; i++) {
                if (ballons[i] != NULL && ballons[i]->actif) {
                    activeBalloonsCount++;
                }
            }
            
            // Si tous les ballons sont inactifs, la vague est terminée
            if (activeBalloonsCount == 0) {
                waveCompleted = true;
                printf("Vague %d terminée!\n", vaguesCount + 1);
            }
        }

        // Si la vague est terminée et qu'on n'a pas atteint le nombre maximum de vagues
        if (waveCompleted && vaguesCount < MAX_VAGUES - 1) {
            // Libérer la mémoire des ballons
            for (int i = 0; i < ballonCount; i++) {
                if (ballons[i] != NULL) {
                    freeBallon(ballons[i]);
                    ballons[i] = NULL;
                }
            }
            
            ballonCount = 0;     // Réinitialiser le compteur de ballons
            vaguesCount++;       // Incrémenter le compteur de vagues
            waveCompleted = false; // Réinitialiser l'état de la vague
            
            printf("Nouvelle vague %d commence...\n", vaguesCount + 1);
        }

        // Créer un nouveau ballon toutes les 15 frames (si la vague n'est pas terminée)
        if (frameCounter >= 15 && ballonCount < MAX_BALLONS && !waveCompleted && vaguesCount < MAX_VAGUES) {
            ballons[ballonCount] = createBallon(laby->entreeX[0], laby->entreeY[0], 
                                               path[0], path[1], pathLength, 0.07f);
            if (ballons[ballonCount] != NULL) {
                ballonCount++;
                printf("Ballon %d créé pour la vague %d\n", ballonCount, vaguesCount + 1);
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
        
        // Afficher des informations sur la vague actuelle
        char waveText[50];
        sprintf(waveText, "Vague: %d/%d", vaguesCount + 1, MAX_VAGUES);
        DrawText(waveText, 10, 10, 20, BLACK);
        
        // Afficher un message si toutes les vagues sont terminées
        if (vaguesCount >= MAX_VAGUES - 1 && waveCompleted) {
            DrawText("Toutes les vagues sont terminées!", SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2, 30, RED);
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