#ifndef ASTAR_H
#define ASTAR_H

#include "maze.h"
#include "tower.h"

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
    int taille;           // Nombre d'éléments dans la liste
} NodeList;


// Toolbox NodeList
NodeList* createNodeList();
void freeNodeList(NodeList *list);
void ajoutNode(NodeList *list, AStarNode *node);
AStarNode* suppNodeF(NodeList *list);
bool existeNode(NodeList *list, int x, int y);
AStarNode* recupNode(NodeList *list, int x, int y);
void suppNode(NodeList *list, AStarNode *node);

// Fonctions A*
int distance(int x1, int y1, int x2, int y2);
int** recherche_astar(Labyrinthe *laby, int *tailleChemin, Tower **towers, int towersSize);

#endif // ASTAR_H