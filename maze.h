#ifndef MAZE_H
#define MAZE_H

#include <stdbool.h>

// Type de cellule dans le labyrinthe
typedef enum {
    WALL,       // Mur
    PATH,       // Chemin
    ENTRY,      // Entrée
    EXIT        // Sortie
} CellType;

// Structure représentant une cellule du labyrinthe
typedef struct {
    CellType type;
    bool visited;      // Pour l'algorithme de génération du labyrinthe
    bool inPath;       // Pour marquer les cellules faisant partie du chemin calculé
} Cell;

// Structure représentant le labyrinthe
typedef struct {
    int width;
    int height;
    Cell **grid;       // Grille 2D de cellules
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
void drawMaze(Maze *maze, int cellSize);

#endif // MAZE_H