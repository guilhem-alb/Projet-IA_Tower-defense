#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "astar.h"

// Création d'une liste de nœuds
NodeList* createNodeList() {
    NodeList *list = (NodeList*) malloc(sizeof(NodeList));
    
    list->nodes = (AStarNode**) malloc(100 * sizeof(AStarNode*));
    
    list->taille = 0;    
    return list;
}

// Libération de la mémoire d'une liste de nœuds
void freeNodeList(NodeList *list) {
    if (list == NULL) return;
    
    if (list->nodes != NULL) {
        // Libération de chaque nœud
        for (int i = 0; i < list->taille; i++) {
            if (list->nodes[i] != NULL) {
                free(list->nodes[i]);
            }
        }
        free(list->nodes);
    }
    
    free(list);
}

// Ajout d'un nœud à une liste
void ajoutNode(NodeList *list, AStarNode *node) {
    // Ajouter le nœud
    list->nodes[list->taille] = node;
    list->taille++;
}

// Retirer le nœud avec la valeur f la plus basse
AStarNode* suppNodeF(NodeList *list) {
    if (list->taille == 0) return NULL;
    
    // Trouver l'index du nœud avec le f minimum
    int minIndex = 0;
    for (int i = 1; i < list->taille; i++) {
        if (list->nodes[i]->f < list->nodes[minIndex]->f) {
            minIndex = i;
        }
    }
    
    // Sauvegarder le nœud à retourner
    AStarNode *node = list->nodes[minIndex];
    
    // Déplacer le dernier nœud à la position du nœud retiré
    list->taille--;
    if (minIndex < list->taille) {
        list->nodes[minIndex] = list->nodes[list->taille];
    }
    
    return node;
}

// Vérifier si un nœud avec les coordonnées (x, y) est dans la liste
bool existeNode(NodeList *list, int x, int y) {
    for (int i = 0; i < list->taille; i++) {
        if (list->nodes[i]->x == x && list->nodes[i]->y == y) {
            return true;
        }
    }
    return false;
}

// Récupérer un nœud avec les coordonnées (x, y) depuis la liste
AStarNode* recupNode(NodeList *list, int x, int y) {
    for (int i = 0; i < list->taille; i++) {
        if (list->nodes[i]->x == x && list->nodes[i]->y == y) {
            return list->nodes[i];
        }
    }
    return NULL;
}

// Retirer un nœud spécifique de la liste
void suppNode(NodeList *list, AStarNode *node) {
    for (int i = 0; i < list->taille; i++) {
        if (list->nodes[i] == node) {
            // Déplacer le dernier nœud à la position du nœud retiré
            list->taille--;
            if (i < list->taille) {
                list->nodes[i] = list->nodes[list->taille];
            }
            return;
        }
    }
}

// Calcul de la distance de Manhattan entre deux points
int distance(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

// Implémentation de l'algorithme A*
int** recherche_astar(Labyrinthe *laby, int arrivX, int arrivY, int goalX, int goalY, int *tailleChemin) {
    // Créer les listes ouverte et fermée
    NodeList *openList = createNodeList();
    NodeList *closedList = createNodeList();
    
    // Créer le nœud de départ
    AStarNode *arrivNode = (AStarNode*) malloc(sizeof(AStarNode));

    
    arrivNode->x = arrivX;
    arrivNode->y = arrivY;
    arrivNode->g = 0;
    arrivNode->h = distance(arrivX, arrivY, goalX, goalY);
    arrivNode->f = arrivNode->g + arrivNode->h;
    arrivNode->parent = NULL;
    
    // Ajouter le nœud de départ à la liste ouverte
    ajoutNode(openList, arrivNode);
    
    // Directions pour les voisins
    const int dx[4] = {0, 1, 0, -1};  // Haut, Droite, Bas, Gauche
    const int dy[4] = {-1, 0, 1, 0};
    
    // Boucle principale de A*
    while (openList->taille > 0) {
        // Prendre le nœud avec le f minimum de la liste ouverte
        AStarNode *currentNode = suppNodeF(openList);
        
        // Vérifier si on a atteint l'objectif
        if (currentNode->x == goalX && currentNode->y == goalY) {
            // Construire le chemin en remontant les parents
            int count = 0;
            AStarNode *node = currentNode;
            while (node != NULL) {
                count++;
                node = node->parent;
            }
            
            // Allouer la mémoire pour le chemin
            int **path = (int**) malloc(2 * sizeof(int*));
            path[0] = (int*) malloc(count * sizeof(int));  // coordonnées X
            path[1] = (int*) malloc(count * sizeof(int));  // coordonnées Y
            
            
            // Remplir le chemin
            node = currentNode;
            int i = count - 1;
            while (node != NULL) {
                path[0][i] = node->x;
                path[1][i] = node->y;
                i--;
                node = node->parent;
            }
            
            // Mettre à jour la longueur du chemin
            *tailleChemin = count;
            
            // Marquer les cellules du chemin
            for (int i = 0; i < count; i++) {
                if (maze->grille[path[1][i]][path[0][i]].type == CHEMIN) {
                    maze->grille[path[1][i]][path[0][i]].dansChemin = true;
                }
            }
            
            // Nettoyer
            freeNodeList(openList);
            freeNodeList(closedList);
            
            return path;
        }
        
        // Ajouter le nœud courant à la liste fermée
        ajoutNode(closedList, currentNode);
        
        // Explorer les voisins
        for (int i = 0; i < 4; i++) {
            int nx = currentNode->x + dx[i];
            int ny = currentNode->y + dy[i];
            
            // Vérifier si la position est valide
            if (!isValid(laby, nx, ny)) continue;
            
            // Vérifier si c'est un mur
            if (maze->grid[ny][nx].type == WALL) continue;
            
            // Vérifier si le nœud est déjà dans la liste fermée
            if (existeNode(closedList, nx, ny)) continue;
            
            // Calculer le nouveau coût g
            int newG = currentNode->g + 1;
            
            // Vérifier si le nœud est dans la liste ouverte
            if (existeNode(openList, nx, ny)) {
                AStarNode *existingNode = recupNode(openList, nx, ny);
                
                // Vérifier si le nouveau chemin est meilleur
                if (newG < existingNode->g) {
                    existingNode->g = newG;
                    existingNode->f = existingNode->g + existingNode->h;
                    existingNode->parent = currentNode;
                }
            } else {
                // Créer un nouveau nœud
                AStarNode *newNode = (AStarNode*) malloc(sizeof(AStarNode));

                
                newNode->x = nx;
                newNode->y = ny;
                newNode->g = newG;
                newNode->h = distance(nx, ny, goalX, goalY);
                newNode->f = newNode->g + newNode->h;
                newNode->parent = currentNode;
                
                // Ajouter à la liste ouverte
                ajoutNode(openList, newNode);
            }
        }
    }
    
    // Si on arrive ici, c'est qu'aucun chemin n'a été trouvé
    printf("Aucun chemin trouvé\n");
    freeNodeList(openList);
    freeNodeList(closedList);
    return NULL;
}
