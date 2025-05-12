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
    int xCoord;        
    int yCoord;         
    bool visite;      // sert à savoir si la case fait partie du labyrinthe dans prim
    bool ajoute;       // pour ne pas traiter une case deux fois dans prim
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
Labyrinthe* initialiserLabyrinthe(int largeur, int hauteur, int numEntrees, int numSorties);
void genererLabyrinthe(Labyrinthe *laby);
void freeLabyrinthe(Labyrinthe *laby);
bool positionValide(Labyrinthe *laby, int x, int y);
void dessinerLabyrinthe(Labyrinthe *laby, int tailleCase);

#endif // MAZE_H