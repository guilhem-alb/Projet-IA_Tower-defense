#ifndef MAZE_H
#define MAZE_H

#include <stdbool.h>

// Type de cellule dans le labyrinthe
typedef enum {
    MUR,       // Mur
    CHEMIN,       // Chemin
    ENTREE,      // Entrée
    SORTIE        // Sortie
} Typecase;

// Structure représentant une cellule du labyrinthe
typedef struct {
    Typecase type;
    bool visite;      // Pour l'algorithme de génération du labyrinthe
    bool dansChemin;       // Pour marquer les cellules faisant partie du chemin calculé
} Case;

// Structure représentant le labyrinthe
typedef struct {
    int largeur;
    int hauteur;
    Case **grille;       // Grille 2D de cellules
    int numEntrees;
    int numSorties;
    int *entreeX;       // Coordonnées X des entrées
    int *entreeY;       // Coordonnées Y des entrées
    int *sortieX;        // Coordonnées X des sorties
    int *sortieY;        // Coordonnées Y des sorties
} Labyrinthe;

// Prototypes des fonctions
Labyrinthe* createMaze(int width, int height, int numEntries, int numExits);
void generateMaze(Labyrinthe *maze);
void freeMaze(Labyrinthe *maze);
bool isValid(Labyrinthe *maze, int x, int y);
void drawMaze(Labyrinthe *maze, int tailleCase);

#endif // MAZE_H