#ifndef MAZE_H
#define MAZE_H

#include <stdbool.h>

// Type de Caseule dans le labyrinthe
typedef enum {
    WALL,       // Mur
    PATH,       // Chemin
    ENTRY,      // Entrée
    EXIT        // Sortie
} CaseType;

// Structure représentant une Caseule du labyrinthe
typedef struct {
    CaseType type;
    bool visited;      // Pour l'algorithme de génération du labyrinthe
    bool inPath;       // Pour marquer les Caseules faisant partie du chemin calculé
} Case;

// Structure représentant le labyrinthe
typedef struct {
    int width;
    int height;
    Case **grid;       // Grille 2D de Caseules
    int numEntries;
    int numExits;
    int *entryX;       // Coordonnées X des entrées
    int *entryY;       // Coordonnées Y des entrées
    int *exitX;        // Coordonnées X des sorties
    int *exitY;        // Coordonnées Y des sorties
} Maze;

// Prototypes des fonctions
Maze* createMaze(int width, int height, int numEntries, int numExits);
void generateMaze(Maze *maze);
void freeMaze(Maze *maze);
bool isValid(Maze *maze, int x, int y);
void drawMaze(Maze *maze, int CaseSize);

#endif // MAZE_H