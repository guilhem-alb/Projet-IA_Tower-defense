#include <stdlib.h>
#include <stdio.h>
#include "maze.h"
#include "listeCases.h"
#include "raylib.h"


// Création d'un labyrinthe vide
Labyrinthe* initialiserLabyrinthe(int largeur, int hauteur, int numEntrees, int numSorties) {
    Labyrinthe *laby = (Labyrinthe*) malloc(sizeof(Labyrinthe));

    
    // Initialisation des dimensions
    laby->largeur = largeur;
    laby->hauteur = hauteur;
    laby->numEntrees = numEntrees;
    laby->numSorties = numSorties;
    
    // Allocation de la grille
    laby->grille = (Case**) malloc(hauteur * sizeof(Case*));

    
    for (int i = 0; i < hauteur; i++) {
        laby->grille[i] = (Case*) malloc(largeur * sizeof(Case)); 
        // initialise les cellules
        for(int j = 0; j < largeur; j++) {
            laby->grille[i][j].xCoord = i;
            laby->grille[i][j].yCoord = j;
            laby->grille[i][j].visite = false;
            laby->grille[i][j].ajoute = false;
            laby->grille[i][j].dansChemin = false;
            // génération de "quadrillage" pour l'algo de Prim
            if((i % 2 == 1) && (j % 2 == 1)) {
                laby->grille[i][j].type = CHEMIN;
            }
            else {
                laby->grille[i][j].type = MUR;
            }
        }
    }
    // Allocation des tableaux d'entrées et sorties
    laby->entreeX = (int*) malloc(numEntrees * sizeof(int));
    laby->entreeY = (int*) malloc(numEntrees * sizeof(int));
    laby->sortieX = (int*) malloc(numSorties * sizeof(int));
    laby->sortieY = (int*) malloc(numSorties * sizeof(int));
    
    return laby;
}

// fonction auxiliaire à Prim pour ajouter les voisins à la liste
void traiterVoisins(Labyrinthe *laby, Case C, listeCases *L) {
    int x = C.xCoord, y = C.yCoord, cnt = 0;
    int coordVoisins[4] = {(x - 2), (y - 2), (x + 2), (y + 2)}; // Toutes les cases voisines (à deux cases de distance)
    listeCases casesVisitees = creerListeCases();
    Case *currCase, caseAConnecter;
    // On sépare les voisins entre ceux faisant partie du labyrinthe et les autres
    for(int i = 0; i < 4; i++) {
        if ((coordVoisins[i] >= 0) && (coordVoisins[i] < ((i % 2 == 0) ? laby->hauteur : laby->largeur))) { // s'assure que le voisin est dans les bornes du tableau
            currCase = ((i % 2 == 0) ? &laby->grille[coordVoisins[i]][y] : &laby->grille[x][coordVoisins[i]]);
            // si la case ne fait pas encore partie du labyrinthe, on l'ajoute aux cases à traiter
            if(!currCase->visite && !currCase->ajoute) {
                ajouterCase(L, *currCase);
                currCase->ajoute = true;
            }
            // sinon on l'ajoute aux potentielles cases à connecter
            else if(currCase->visite) {
                ajouterCase(&casesVisitees, *currCase);
                cnt++;
            }
        }
    }
    if(!listeCasesVide(casesVisitees)) {
        // pour génerer plusieurs chemins dans le labyrinthe
        for(int i = 0; i <= rand() % cnt; i++) {
            // On le connecte à une voisine faisant partie du labyrinthe au hasard
            caseAConnecter = prendreAuHasard(&casesVisitees);
            laby->grille[(x + caseAConnecter.xCoord) / 2][(y + caseAConnecter.yCoord) / 2].type = CHEMIN;
            laby->grille[x][y].visite = true;
        }
    }
    freeMaillon(casesVisitees.liste);
}

// génère un labyrinthe en utilisant l'algorithme de Prim
void prim(Labyrinthe *laby) {
    listeCases casesATraiter = creerListeCases();
    Case currCase; 
    // Sélection d'une case du quadrillage aléatoire
    int x = rand() % (laby->hauteur / 2) * 2 + 1;
    int y = rand() % (laby->largeur / 2) * 2 + 1;
    laby->grille[x][y].visite = true;
    ajouterCase(&casesATraiter, laby->grille[x][y]);
    while(!listeCasesVide(casesATraiter)) {
        currCase = prendreAuHasard(&casesATraiter);
        traiterVoisins(laby, currCase, &casesATraiter);
    }
}

// Fonction principale de génération du labyrinthe
void genererLabyrinthe(Labyrinthe *laby) {
    // Générer le labyrinthe à partir du point de départ
    prim(laby);
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
            
            if (positionValide(laby, adjX, adjY) && laby->grille[adjY][adjX].type == CHEMIN && laby->grille[y][x].type == MUR) {
                validEntry = true;
            }
        } while (!validEntry);
        
        // Marquer l'entrée
        laby->grille[y][x].type = ENTREE;
        laby->entreeX[i] = x;
        laby->entreeY[i] = y;
    
    }
        // Placer la sortie du côté opposé
    for (int j = 0; j < laby->numSorties; j++) {
            int exitSide, exitX, exitY, adjExitX, adjExitY;
            bool validExit = false;
                    // Déterminer le côté opposé à une entrée aléatoire
        int randomEntry = rand() % laby->numEntrees;
        int entryX = laby->entreeX[randomEntry];
        int entryY = laby->entreeY[randomEntry];
        
        // Déterminer le côté de l'entrée
        int entrySide;
        if (entryY == 0) entrySide = 0; // Haut
        else if (entryX == laby->largeur - 1) entrySide = 1; // Droite
        else if (entryY == laby->hauteur - 1) entrySide = 2; // Bas
        else entrySide = 3; // Gauche
        
        // Déterminer le côté opposé pour la sortie
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
                
                if (positionValide(laby, adjExitX, adjExitY) && laby->grille[adjExitY][adjExitX].type == CHEMIN && laby->grille[exitY][exitX].type == MUR) {
                    validExit = true;
                }
            } while (!validExit);
            
            // Marquer la sortie
            laby->grille[exitY][exitX].type = SORTIE;
            laby->sortieX[j] = exitX;
            laby->sortieY[j] = exitY;   
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