#ifndef MAZE_H
#define MAZE_H

#include <stdbool.h>

// Type de cellule dans le labyrinthe
typedef enum {
    MUR,       // Mur
    CHEMIN,       // Chemin
    ENTREE,      // Entrée
    SORTIE        // Sortie
} CaseType;

// Structure représentant une cellule du labyrinthe
typedef struct {
    CaseType type;
    bool visited;      // Pour l'algorithme de génération du labyrinthe
    bool inPath;       // Pour marquer les cellules faisant partie du chemin calculé
} Case;

// Structure représentant le labyrinthe
typedef struct {
    int width;
    int height;
    Case **grid;       // Grille 2D de cellules
    int numEntries;
    int numExits;
    int *entryX;       // Coordonnées X des entrées
    int *entryY;       // Coordonnées Y des entrées
    int *exitX;        // Coordonnées X des sorties
    int *exitY;        // Coordonnées Y des sorties
} Labyrinthe;

// Prototypes des fonctions
Labyrinthe* createMaze(int width, int height, int numEntries, int numExits);
void generateMaze(Labyrinthe *maze);
void freeMaze(Labyrinthe *maze);
bool isValid(Labyrinthe *maze, int x, int y);
void drawMaze(Labyrinthe *maze, int tailleCase);

#endif // MAZE_H