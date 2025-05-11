#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "maze.h"
#include "raylib.h"

// Création d'un labyrinthe vide
Labyrinthe* createMaze(int width, int height, int numEntries, int numExits) {
    Labyrinthe *maze = (Labyrinthe*) malloc(sizeof(Labyrinthe));

    
    // Initialisation des dimensions
    maze->width = width;
    maze->height = height;
    maze->numEntries = numEntries;
    maze->numExits = numExits;
    
    // Allocation de la grille
    maze->grid = (Case**) malloc(height * sizeof(Case*));

    
    for (int i = 0; i < height; i++) {
        maze->grid[i] = (Case*) calloc(width, sizeof(Case)); // initialise les cellules avec tous leurs attributs à 0
        
    }
    
    // Allocation des tableaux d'entrées et sorties
    maze->entryX = (int*) malloc(numEntries * sizeof(int));
    maze->entryY = (int*) malloc(numEntries * sizeof(int));
    maze->exitX = (int*) malloc(numExits * sizeof(int));
    maze->exitY = (int*) malloc(numExits * sizeof(int));
    
    return maze;
}

// Libération de la mémoire du labyrinthe
void freeMaze(Labyrinthe *maze) {
    if (maze == NULL) return;
    
    if (maze->grid != NULL) {
        for (int i = 0; i < maze->height; i++) {
            if (maze->grid[i] != NULL) {
                free(maze->grid[i]);
            }
        }
        free(maze->grid);
    }
    
    if (maze->entryX != NULL) free(maze->entryX);
    if (maze->entryY != NULL) free(maze->entryY);
    if (maze->exitX != NULL) free(maze->exitX);
    if (maze->exitY != NULL) free(maze->exitY);
    
    free(maze);
}

// Fonction pour vérifier si une position est valide
bool isValid(Labyrinthe *maze, int x, int y) {
    return (x >= 0 && x < maze->width && y >= 0 && y < maze->height);
}

// directions pour la génération du labyrinthe
const int DX[4] = {0, 1, 0, -1};  // NORTH, EAST, SOUTH, WEST
const int DY[4] = {-1, 0, 1, 0};

// Ajout d'une fonction pour mélanger un tableau
void shuffleArray(int *array, int size) {
    for (int i = size - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

// Implémentation récursive de l'algorithme de creusement du labyrinthe
void carvePath(Labyrinthe *maze, int x, int y) {
    // Marquer la cellule comme visitée et comme chemin
    maze->grid[y][x].visited = true;
    maze->grid[y][x].type = PATH;
    
    // Tableau d'indices de directions
    int directions[4] = {0, 1, 2, 3}; // NORTH, EAST, SOUTH, WEST
    shuffleArray(directions, 4);
    
    // Explorer dans toutes les directions
    for (int i = 0; i < 4; i++) {
        int dir = directions[i];
        int nx = x + DX[dir] * 2; // Sauter une cellule pour laisser de l'espace entre les chemins
        int ny = y + DY[dir] * 2;
        
        // Vérifier si la nouvelle position est valide et non visitée
        if (isValid(maze, nx, ny) && !maze->grid[ny][nx].visited) {
            // Creuser un passage entre la cellule actuelle et la nouvelle cellule
            maze->grid[y + DY[dir]][x + DX[dir]].type = PATH;
            
            // Continuer depuis la nouvelle cellule
            carvePath(maze, nx, ny);
        }
    }
}

// Fonction principale de génération du labyrinthe
void generateMaze(Labyrinthe *maze) {
    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));
    
    // Choisir un point de départ aléatoire (doit être impair pour laisser des murs)
    int startX = 1 + 2 * (rand() % ((maze->width - 1) / 2));
    int startY = 1 + 2 * (rand() % ((maze->height - 1) / 2));
    
    // S'assurer que le point de départ est dans les limites
    if (startX >= maze->width) startX = maze->width - 2;
    if (startY >= maze->height) startY = maze->height - 2;
    
    // Générer le labyrinthe à partir du point de départ
    carvePath(maze, startX, startY);
    
    // Placer les entrées aux bords du labyrinthe
    for (int i = 0; i < maze->numEntries; i++) {
        int entrySide, x, y, adjX, adjY;
        bool validEntry = false;
        
        do {
            entrySide = rand() % 4; // Côté du labyrinthe (0=haut, 1=droite, 2=bas, 3=gauche)
            
            switch (entrySide) {
                case 0: // Haut
                    x = 1 + 2 * (rand() % ((maze->width - 1) / 2));
                    y = 0;
                    break;
                case 1: // Droite
                    x = maze->width - 1;
                    y = 1 + 2 * (rand() % ((maze->height - 1) / 2));
                    break;
                case 2: // Bas
                    x = 1 + 2 * (rand() % ((maze->width - 1) / 2));
                    y = maze->height - 1;
                    break;
                case 3: // Gauche
                    x = 0;
                    y = 1 + 2 * (rand() % ((maze->height - 1) / 2));
                    break;
            }
            
            // S'assurer que les coordonnées sont valides
            if (x >= maze->width) x = maze->width - 1;
            if (y >= maze->height) y = maze->height - 1;
            
            // Vérifier que la cellule adjacente est un chemin
            adjX = x + (entrySide == 3 ? 1 : (entrySide == 1 ? -1 : 0));
            adjY = y + (entrySide == 0 ? 1 : (entrySide == 2 ? -1 : 0));
            
            if (isValid(maze, adjX, adjY) && maze->grid[adjY][adjX].type == PATH) {
                validEntry = true;
            }
        } while (!validEntry);
        
        // Marquer l'entrée
        maze->grid[y][x].type = ENTRY;
        maze->entryX[i] = x;
        maze->entryY[i] = y;
        
        // Placer la sortie du côté opposé
        for (int j = 0; j < maze->numExits; j++) {
            int exitSide, exitX, exitY, adjExitX, adjExitY;
            bool validExit = false;
            
            // Déterminer le côté opposé
            exitSide = (entrySide + 2) % 4;
            
            do {
                switch (exitSide) {
                    case 0: // Haut
                        exitX = 1 + 2 * (rand() % ((maze->width - 1) / 2));
                        exitY = 0;
                        break;
                    case 1: // Droite
                        exitX = maze->width - 1;
                        exitY = 1 + 2 * (rand() % ((maze->height - 1) / 2));
                        break;
                    case 2: // Bas
                        exitX = 1 + 2 * (rand() % ((maze->width - 1) / 2));
                        exitY = maze->height - 1;
                        break;
                    case 3: // Gauche
                        exitX = 0;
                        exitY = 1 + 2 * (rand() % ((maze->height - 1) / 2));
                        break;
                }
                
                // S'assurer que les coordonnées sont valides
                if (exitX >= maze->width) exitX = maze->width - 1;
                if (exitY >= maze->height) exitY = maze->height - 1;
                
                // Vérifier que la cellule adjacente est un chemin
                adjExitX = exitX + (exitSide == 3 ? 1 : (exitSide == 1 ? -1 : 0));
                adjExitY = exitY + (exitSide == 0 ? 1 : (exitSide == 2 ? -1 : 0));
                
                if (isValid(maze, adjExitX, adjExitY) && maze->grid[adjExitY][adjExitX].type == PATH) {
                    validExit = true;
                }
            } while (!validExit);
            
            // Marquer la sortie
            maze->grid[exitY][exitX].type = EXIT;
            maze->exitX[j] = exitX;
            maze->exitY[j] = exitY;
        }
    }
}

// Fonction pour afficher le labyrinthe
void drawMaze(Labyrinthe *maze, int tailleCase) {
    for (int y = 0; y < maze->height; y++) {
        for (int x = 0; x < maze->width; x++) {
            Rectangle rect = {x * tailleCase, y * tailleCase, tailleCase, tailleCase};
            
            switch (maze->grid[y][x].type) {
                case WALL:
                    DrawRectangleRec(rect, DARKGRAY);
                    break;
                case PATH:
                    if (maze->grid[y][x].inPath) {
                        DrawRectangleRec(rect, LIGHTGRAY);
                    } else {
                        DrawRectangleRec(rect, WHITE);
                    }
                    break;
                case ENTRY:
                    DrawRectangleRec(rect, GREEN);
                    break;
                case EXIT:
                    DrawRectangleRec(rect, RED);
                    break;
            }
            
            // Dessin de la bordure des cellules
            DrawRectangleLinesEx(rect, 1, BLACK);
        }
    }
}