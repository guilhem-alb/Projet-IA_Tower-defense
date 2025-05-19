#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "raylib.h"
#include "maze.h"
#include "astar.h"
#include "ballon.h"
#include "tower.h"
#include "projectile.h"


#define CELL_SIZE 40
// Doivent toujours être un multiple impair de CELL_SIZE pour la bonne génération du labyrinthe
#define SCREEN_WIDTH 1000   
#define SCREEN_HEIGHT 840   


#define MAX_BALLONS 10
#define MAX_TOWERS 50
#define MAX_PROJECTILES 200
#define MAX_VAGUES 5

#define NOMBRE_ENTREES 2
#define NOMBRE_SORTIES 2

#define VIE_DE_DEPART 10
#define ARGENT_DE_DEPART 9
#define COUT_TOUR 3

// Prototypes
void resetGame();

// Globals
Labyrinthe *laby;
int **path;
int pathLength;

Ballon *ballons[MAX_BALLONS];
int ballonCount   = 0;
int frameCounter  = 0;

Tower *towers[MAX_TOWERS];
int towerCount = 0;

Projectile *projectiles[MAX_PROJECTILES];
int projectileCount = 0;

int vaguesCount   = 0;
bool waveCompleted = false;
bool waveStarted = false;
bool gameEnded = false;

int vieJoueur = VIE_DE_DEPART;
int argentJoueur = ARGENT_DE_DEPART;

int main(void) {
    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));
    // Initialisation de la fenêtre
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "bloons maze battle");
    SetTargetFPS(60);
    // Calcul de la taille du labyrinthe en fonction de la taille de l'écran
    int labyWidth = SCREEN_WIDTH / CELL_SIZE;
    int labyHeight = (SCREEN_HEIGHT / CELL_SIZE);
    if(labyWidth % 2 != 1 || labyHeight % 2 != 1) {
        perror("taille de labyrinthe incorrecte");
        return 1;
    }
    
    // Création du labyrinthe
    laby = initialiserLabyrinthe(labyWidth, labyHeight, NOMBRE_ENTREES, NOMBRE_SORTIES);  // 1 entrée, 1 sortie
    
    // Génération du labyrinthe
    genererLabyrinthe(laby);
    
     // Calcul du chemin avec A*
            pathLength = 0;
            path = recherche_astar(laby, &pathLength, towers, towerCount);
    
    
    // Boucle principale
    while (!WindowShouldClose()) {
        if(!gameEnded){
            // Mise à jour
            frameCounter++;

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
                
                // effacer le chemin précédent
                for(int i = 0; i < pathLength; i++) {
                    laby->grille[path[1][i]][path[0][i]].dansChemin = false;
                }
                // Libération du chemin
                if (path) {
                    free(path[0]);
                    free(path[1]);
                    free(path);
                }
                // Calcul du nouveau chemin avec A*
                pathLength = 0;
                path = recherche_astar(laby, &pathLength, towers, towerCount);
                // redessiner le chemin
                printf("Nouvelle vague %d commence...\n", vaguesCount + 1);
            }

            // Créer un nouveau ballon toutes les 15 frames (si la vague n'est pas terminée)
            if (frameCounter >= 15 && ballonCount < MAX_BALLONS && !waveCompleted && vaguesCount < MAX_VAGUES) {
                ballons[ballonCount] = createBallon(path[0], path[1], pathLength, 0.07f);
                if (ballons[ballonCount] != NULL) {
                    ballonCount++;
                    printf("Ballon %d créé pour la vague %d\n", ballonCount, vaguesCount + 1);
                }
                frameCounter = 0;
                
            }
            else if(frameCounter >= 15)
                waveStarted = true;
            
            // Mise à jour de tous les ballons
            for (int i = 0; i < ballonCount; i++) {
                if (ballons[i] != NULL && ballons[i]->actif) {
                    updateBallon(ballons[i], &vieJoueur);
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
                    updateProjectile(projectiles[i], &argentJoueur);
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
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && argentJoueur >= COUT_TOUR) {
                Vector2 mousePos = GetMousePosition();
                int gridX = (int)(mousePos.x / CELL_SIZE);
                int gridY = (int)(mousePos.y / CELL_SIZE);
                
                if (canPlaceTower(laby, gridX, gridY) && towerCount < MAX_TOWERS) {
                    towers[towerCount] = createTower(gridX, gridY, 3, 1.0f);
                    if (towers[towerCount] != NULL) {
                        towerCount++;
                    }
                    argentJoueur -= 3;
                }
            }

            // Vérifier si tous les ballons ont été créés pour cette vague
            if (!waveCompleted && waveStarted) {
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
        }
        // Afficher des informations sur la vague actuelle
        char waveText[50];
        sprintf(waveText, "Vague: %d/%d", vaguesCount + 1, MAX_VAGUES);
        DrawText(waveText, 10, 10, 20, BLACK);

        // afficeh les points de vie
        char healthText[10];
        sprintf(healthText, "PV: %d", vieJoueur);
        DrawText(healthText, 150, 10, 20, RED);

        // afficeh les points de vie
        char argentText[10];
        sprintf(argentText, "$: %d", argentJoueur);
        DrawText(argentText, 230, 10, 20, YELLOW);

        // Bouton Retry quand fini
        if (vaguesCount >= MAX_VAGUES-1 && waveCompleted) {
            gameEnded = true;
            DrawText("Toutes les vagues sont terminées!",
                     SCREEN_WIDTH/2 - 200, SCREEN_HEIGHT/2, 30, RED);

            Rectangle btn = {
                SCREEN_WIDTH/2 - 60,
                SCREEN_HEIGHT/2 + 40,
                120, 40
            };
            bool hover = CheckCollisionPointRec(GetMousePosition(), btn);
            DrawRectangleRec(btn, hover ? LIGHTGRAY : DARKGRAY);
            DrawText("Retry", btn.x + 30, btn.y + 10, 20, BLACK);

            if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                resetGame();
            }
        }

        // écran de game over
        if (vieJoueur == 0) {
            gameEnded = true;
            DrawText("GAME OVER !",
                     SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2, 30, RED);

            Rectangle btn = {
                SCREEN_WIDTH/2 - 60,
                SCREEN_HEIGHT/2 + 40,
                120, 40
            };
            bool hover = CheckCollisionPointRec(GetMousePosition(), btn);
            DrawRectangleRec(btn, hover ? LIGHTGRAY : DARKGRAY);
            DrawText("Retry", btn.x + 30, btn.y + 10, 20, BLACK);

            if (hover && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                resetGame();
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

void resetGame() {
    // Réinitialisation des compteurs/verrous
    vaguesCount = 0;
    waveCompleted = false;
    waveStarted = false;
    gameEnded = false;
    vieJoueur = VIE_DE_DEPART;
    argentJoueur = ARGENT_DE_DEPART;

    // Libération des ballons
    for (int i = 0; i < ballonCount; i++) {
        if (ballons[i]) { freeBallon(ballons[i]); ballons[i] = NULL; }
    }
    ballonCount = 0;

    // Libération des tours
    for (int i = 0; i < towerCount; i++) {
        if (towers[i]) { freeTower(towers[i]); towers[i] = NULL; }
    }
    towerCount = 0;

    // Libération des projectiles
    for (int i = 0; i < projectileCount; i++) {
        if (projectiles[i]) {
            freeProjectile(projectiles[i]);
            projectiles[i] = NULL;
        }
    }
    projectileCount = 0;

    // Libération du chemin
    if (path) {
        free(path[0]);
        free(path[1]);
        free(path);
    }

    // Libération et régénération du labyrinthe (entrée/sortie aléatoires)
    freeLabyrinthe(laby);
    int labyWidth  = SCREEN_WIDTH  / CELL_SIZE;
    int labyHeight = SCREEN_HEIGHT / CELL_SIZE;
    laby = initialiserLabyrinthe(labyWidth, labyHeight, NOMBRE_ENTREES, NOMBRE_SORTIES);
    genererLabyrinthe(laby);
    // Calcul du chemin avec A*
            pathLength = 0;
            path = recherche_astar(laby, &pathLength, towers, towerCount);

    frameCounter = 0;
}
