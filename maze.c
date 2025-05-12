#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "maze.h"
#include "raylib.h"

// Création d'un labyrinthe vide
Labyrinthe* creerLabyrinthe(int largeur, int hauteur, int numEntrees, int numSorties) {
    Labyrinthe *laby = (Labyrinthe*) malloc(sizeof(Labyrinthe));

    
    // Initialisation des dimensions
    laby->largeur = largeur;
    laby->hauteur = hauteur;
    laby->numEntrees = numEntrees;
    laby->numSorties = numSorties;
    
    // Allocation de la grille
    laby->grille = (Case**) malloc(hauteur * sizeof(Case*));

    
    for (int i = 0; i < hauteur; i++) 
        laby->grille[i] = (Case*) calloc(largeur, sizeof(Case)); // initialise les cellules avec tous leurs attributs à 0
    
    // Allocation des tableaux d'entrées et sorties
    laby->entreeX = (int*) malloc(numEntrees * sizeof(int));
    laby->entreeY = (int*) malloc(numEntrees * sizeof(int));
    laby->sortieX = (int*) malloc(numSorties * sizeof(int));
    laby->sortieY = (int*) malloc(numSorties * sizeof(int));
    
    return laby;
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
void carvePath(Labyrinthe *laby, int x, int y) {
    // Marquer la cellule comme visitée et comme chemin
    laby->grille[y][x].visite = true;
    laby->grille[y][x].type = CHEMIN;
    
    // Tableau d'indices de directions
    int directions[4] = {0, 1, 2, 3}; // NORTH, EAST, SOUTH, WEST
    shuffleArray(directions, 4);
    
    // Explorer dans toutes les directions
    for (int i = 0; i < 4; i++) {
        int dir = directions[i];
        int nx = x + DX[dir] * 2; // Sauter une cellule pour laisser de l'espace entre les chemins
        int ny = y + DY[dir] * 2;
        
        // Vérifier si la nouvelle position est valide et non visitée
        if (positionValide(laby, nx, ny) && !laby->grille[ny][nx].visite) {
            // Creuser un passage entre la cellule actuelle et la nouvelle cellule
            laby->grille[y + DY[dir]][x + DX[dir]].type = CHEMIN;
            
            // Continuer depuis la nouvelle cellule
            carvePath(laby, nx, ny);
        }
    }
}

// Fonction principale de génération du labyrinthe
void genererLabyrinthe(Labyrinthe *laby) {
    // Initialiser le générateur de nombres aléatoires
    srand(time(NULL));
    
    // Choisir un point de départ aléatoire (doit être impair pour laisser des murs)
    int startX = 1 + 2 * (rand() % ((laby->largeur - 1) / 2));
    int startY = 1 + 2 * (rand() % ((laby->hauteur - 1) / 2));
    
    // S'assurer que le point de départ est dans les limites
    if (startX >= laby->largeur) startX = laby->largeur - 2;
    if (startY >= laby->hauteur) startY = laby->hauteur - 2;
    
    // Générer le labyrinthe à partir du point de départ
    carvePath(laby, startX, startY);
    
    // Placer les entrées aux bords du labyrinthe
    for (int i = 0; i < laby->numEntrees; i++) {
        int entrySide, x, y, adjX, adjY;
        bool validEntry = false;
        
        do {
            entrySide = rand() % 4; // Côté du labyrinthe (0=haut, 1=droite, 2=bas, 3=gauche)
            
            switch (entrySide) {
                case 0: // Haut
                    x = 1 + 2 * (rand() % ((laby->largeur - 1) / 2));
                    y = 0;
                    break;
                case 1: // Droite
                    x = laby->largeur - 1;
                    y = 1 + 2 * (rand() % ((laby->hauteur - 1) / 2));
                    break;
                case 2: // Bas
                    x = 1 + 2 * (rand() % ((laby->largeur - 1) / 2));
                    y = laby->hauteur - 1;
                    break;
                case 3: // Gauche
                    x = 0;
                    y = 1 + 2 * (rand() % ((laby->hauteur - 1) / 2));
                    break;
            }
            
            // S'assurer que les coordonnées sont valides
            if (x >= laby->largeur) x = laby->largeur - 1;
            if (y >= laby->hauteur) y = laby->hauteur - 1;
            
            // Vérifier que la cellule adjacente est un chemin
            adjX = x + (entrySide == 3 ? 1 : (entrySide == 1 ? -1 : 0));
            adjY = y + (entrySide == 0 ? 1 : (entrySide == 2 ? -1 : 0));
            
            if (positionValide(laby, adjX, adjY) && laby->grille[adjY][adjX].type == CHEMIN) {
                validEntry = true;
            }
        } while (!validEntry);
        
        // Marquer l'entrée
        laby->grille[y][x].type = ENTREE;
        laby->entreeX[i] = x;
        laby->entreeY[i] = y;
        
        // Placer la sortie du côté opposé
        for (int j = 0; j < laby->numSorties; j++) {
            int exitSide, exitX, exitY, adjExitX, adjExitY;
            bool validExit = false;
            
            // Déterminer le côté opposé
            exitSide = (entrySide + 2) % 4;
            
            do {
                switch (exitSide) {
                    case 0: // Haut
                        exitX = 1 + 2 * (rand() % ((laby->largeur - 1) / 2));
                        exitY = 0;
                        break;
                    case 1: // Droite
                        exitX = laby->largeur - 1;
                        exitY = 1 + 2 * (rand() % ((laby->hauteur - 1) / 2));
                        break;
                    case 2: // Bas
                        exitX = 1 + 2 * (rand() % ((laby->largeur - 1) / 2));
                        exitY = laby->hauteur - 1;
                        break;
                    case 3: // Gauche
                        exitX = 0;
                        exitY = 1 + 2 * (rand() % ((laby->hauteur - 1) / 2));
                        break;
                }
                
                // S'assurer que les coordonnées sont valides
                if (exitX >= laby->largeur) exitX = laby->largeur - 1;
                if (exitY >= laby->hauteur) exitY = laby->hauteur - 1;
                
                // Vérifier que la cellule adjacente est un chemin
                adjExitX = exitX + (exitSide == 3 ? 1 : (exitSide == 1 ? -1 : 0));
                adjExitY = exitY + (exitSide == 0 ? 1 : (exitSide == 2 ? -1 : 0));
                
                if (positionValide(laby, adjExitX, adjExitY) && laby->grille[adjExitY][adjExitX].type == CHEMIN) {
                    validExit = true;
                }
            } while (!validExit);
            
            // Marquer la sortie
            laby->grille[exitY][exitX].type = SORTIE;
            laby->sortieX[j] = exitX;
            laby->sortieY[j] = exitY;
        }
    }
}

// Libération de la mémoire du labyrinthe
void freeLabyrinthe(Labyrinthe *laby) {
    if (laby == NULL) return;
    
    if (laby->grille != NULL) {
        for (int i = 0; i < laby->hauteur; i++) {
            if (laby->grille[i] != NULL) {
                free(laby->grille[i]);
            }
        }
        free(laby->grille);
    }
    
    if (laby->entreeX != NULL) free(laby->entreeX);
    if (laby->entreeY != NULL) free(laby->entreeY);
    if (laby->sortieX != NULL) free(laby->sortieX);
    if (laby->sortieY != NULL) free(laby->sortieY);
    
    free(laby);
}

// Fonction pour vérifier si une position est valide
bool positionValide(Labyrinthe *laby, int x, int y) {
    return (x >= 0 && x < laby->largeur && y >= 0 && y < laby->hauteur);
}

// Fonction pour afficher le labyrinthe
void dessinerLabyrinthe(Labyrinthe *laby, int tailleCase) {
    for (int y = 0; y < laby->hauteur; y++) {
        for (int x = 0; x < laby->largeur; x++) {
            Rectangle rect = {x * tailleCase, y * tailleCase, tailleCase, tailleCase};
            
            switch (laby->grille[y][x].type) {
                case MUR:
                    DrawRectangleRec(rect, DARKGRAY);
                    break;
                case CHEMIN:
                    if (laby->grille[y][x].dansChemin) {
                        DrawRectangleRec(rect, LIGHTGRAY);
                    } else {
                        DrawRectangleRec(rect, WHITE);
                    }
                    break;
                case ENTREE:
                    DrawRectangleRec(rect, GREEN);
                    break;
                case SORTIE:
                    DrawRectangleRec(rect, RED);
                    break;
            }
            
            // Dessin de la bordure des cellules
            DrawRectangleLinesEx(rect, 1, BLACK);
        }
    }
}