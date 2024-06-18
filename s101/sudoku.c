/**
 * @brief Jeu de sudoku.
*/

#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define NB_LIGNES 9
#define NB_COLONNES 9
typedef int tab2D[NB_LIGNES][NB_COLONNES];

void chargerGrille(tGrille g){
    char nomFichier[30];
    FILE * f;

    printf("Nom du fichier ?\n");
    scanf("%s", nomFichier)
    f = fopen(nomFichier, "rb");
    if (f==NULL){
        printf("\n ERREUR sur le fichier %s\n", nomFichier);
    } else {
        fread(g, sizeof(int), 81, f);
    }
    fclose(f);
}
void grille(tab2D t);
void afficherGrille(*t){
    for (i)
    printf("+");
    printf("-");
    printf("|");
}
int main()
{
    chargerGrille();
    while //grille pas pleine
    {
            afficherGrille();
            printf("Indice de la case ? \n");
            saisir(numLigne);
            saisir(numColonne);
            if (grille [numLigne][numColonne]!=0){
                printf("IMPOSSIBLE, la case n'est pas libre \n")
            }
            else{
                printf("Valuer à insérer ? \n");
                saisir(valeur);
                if (possible (grille1, numLigne, numColonne, valeur)){
                    grille1[numbLigne][numColonne] = valeur;
                }
            }

    }
    printf("Grille pleine, fin de la partie");
    tab2D grilleSudo;
    tab2D grilleSUDOKU=chargerGrille
    grille(grilleSudo);
}
