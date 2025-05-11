#ifndef ASTAR_H
#define ASTAR_H

#include "maze.h"

// Structure pour un nœud dans l'algorithme A*
typedef struct AStarNode {
    int x, y;           // Position
    int g;              // Coût du chemin depuis le départ
    int h;              // Heuristique (estimation du coût jusqu'à l'arrivée)
    int f;              // f = g + h (coût total estimé)
    struct AStarNode *parent;  // Parent pour reconstruire le chemin
} AStarNode;

// Structure pour une liste de nœuds
typedef struct {
    AStarNode **nodes;  // Tableau dynamique de pointeurs vers des nœuds
    int size;           // Nombre d'éléments dans la liste
    int capacity;       // Capacité actuelle du tableau
} NodeList;

// Prototypes des fonctions
NodeList* createNodeList(int initialCapacity);
void freeNodeList(NodeList *list);
void addToNodeList(NodeList *list, AStarNode *node);
AStarNode* removeNodeWithLowestF(NodeList *list);
bool isInNodeList(NodeList *list, int x, int y);
AStarNode* getNodeFromList(NodeList *list, int x, int y);
void removeFromNodeList(NodeList *list, AStarNode *node);
int manhattanDistance(int x1, int y1, int x2, int y2);
int** aStarSearch(Labyrinthe *maze, int startX, int startY, int goalX, int goalY, int *pathLength);

#endif // ASTAR_H