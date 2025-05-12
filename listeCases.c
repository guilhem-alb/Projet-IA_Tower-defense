#include <stdlib.h>
#include <assert.h>
#include "listeCases.h"


listeCases creerListeCases() {
    listeCases L;
    L.taille = 0;
    L.liste = NULL;
    return L;
}

void ajouterCase(listeCases *L, Case C) {
    maillon *nouveau = malloc(sizeof(maillon));
    nouveau->suiv = L->liste;
    nouveau->voisin = C;
    L->taille++;
    L->liste = nouveau;
}

bool listeCasesVide(listeCases L) {
    return (L.liste == NULL) ||(L.taille == 0);
}

// Attention ne met pas à jour la taille de la liste
// Seulement destiné à être utilisé comme fonction auxiliaire
Case prendreCaseDeTete(maillon **M) {
    assert((M != NULL) && (*M != NULL));
    maillon *tete = *M;
    Case C = tete->voisin;

    *M = tete->suiv; 
    free(tete);           

    return C;
}


Case prendreAuHasard(listeCases *L) {
    assert(!listeCasesVide(*L));
    
    int index = rand() % L->taille;
    L->taille--;
    if (index == 0) {
        return prendreCaseDeTete(&(L->liste));
    }

    maillon *prev = L->liste;
    for (int i = 0; i < index - 1; i++) {
        prev = prev->suiv;
    }

    maillon *toRemove = prev->suiv;


    Case C = toRemove->voisin;
    prev->suiv = toRemove->suiv;
    free(toRemove);
    

    return C;
}