// Ici est une implémentation d'une liste pour l'algorithme de Prim servant à la génération du labyrinthe
#ifndef LISTE_CASES_H
#define LISTE_CASES_H

#include "maze.h"

// structure de liste chainée
typedef struct zmaillon{
    Case voisin;
    struct zmaillon *suiv;
} maillon;

// utilisée dans l'algorithme de Prim pour stocker les cases non visitées
typedef struct {
    int taille; // essentiel pour le choix aléatoire
    maillon *liste;
} listeCases;

// prototypes
listeCases creerListeCases(); // crée une liste vide
void ajouterCase(listeCases *, Case); // ajoute la case dans la liste
bool listeCasesVide(listeCases);    // teste si la liste est vide
Case prendreAuHasard(listeCases *); // choisis une case au hasard est l'enlève de la liste
void freeMaillon(maillon *);


#endif