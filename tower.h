#ifndef TOWER_H
#define TOWER_H

#include <stdbool.h>
#include "maze.h"
#include "ballon.h"
#include "projectile.h"

// Structure pour les tours
typedef struct {
    int x;              // Position X
    int y;              // Position Y
    int range;          // Portée en nombre de cellules
    float reloadTime;   // Temps entre les tirs en secondes
    float reloadCounter;// Compteur pour le rechargement
    bool actif;        // Si la tour est active
} Tower;

// Prototypes des fonctions
Tower* createTower(int x, int y, int range, float reloadTime);
void freeTower(Tower *tower);
Projectile* updateTower(Tower *tower, Ballon **ballons, int ballonCount);
void drawTower(Tower *tower, int cellSize);
bool canPlaceTower(Labyrinthe *maze, int x, int y);

#endif // TOWER_H