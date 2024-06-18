/**
 * Ce programme en langage C implémente le jeu classique du Tic-Tac-Toe (Morpion).
 * Il utilise une grille 3x3 pour représenter le plateau de jeu.
 * Deux joueurs, symbolisés par "X" et "O", alternent pour effectuer leurs mouvements.
 * Le programme vérifie constamment s'il y a un gagnant ou un match nul à chaque tour.
 * La partie se termine dès qu'un joueur remporte la partie ou si un match nul est atteint.
 * Des commentaires détaillés sont ajoutés pour faciliter la compréhension du code source.
*/

#include <stdio.h>
#include <stdlib.h>

#define WIN 1 /** Les victoires sont égales à 1 */
#define LOST 0 /** Les pertes sont égales à 0 */

int nbvictoire; /**Nombre total des victoires*/
int nbnul; /** Nombre total des match nul*/

typedef struct {
    char symbole;    /** Symbole du joueur ('X' ou 'O') */
    int victoires;    /** Nombre de victoires du joueur */
} Joueur; /** Structure victoire d'un symbole */

typedef struct {
    Joueur joueur1;  /** Joueur 1 */
    Joueur joueur2;  /** Joueur 2 */
} InfosJoueurs; /** Structure d'un joueur */

/*
 * Déclaration des fonctions
 */
void afficherPlateau(char plateau[3][3]);
int verifierVictoire(char plateau[3][3], char joueur);
int verifierMatchNul(char plateau[3][3]);

/**
 * int main()
 * 
 * \brief Fonction principale du programme, elle permet de 
 * jouer en exécutant plusieurs fonctions telles que :
 * - afficherPlateau()
 * - verifierVictoire()
 * - verifierMatchNul()
 * 
 * \return 0
 */

int main() {
    /* Initialisation du plateau */
    char plateau[3][3] = {{' ', ' ', ' '}, {' ', ' ', ' '}, {' ', ' ', ' '}};

    /* Variables pour les joueurs */
    char joueur1 = 'X';
    char joueur2 = 'O';

    /* Variable pour le joueur en cours */
    char joueurActuel = joueur1;

    /* Boucle principale du jeu */
    int tour;
    for (tour = 0; tour < 9; tour++) {
        /* Affichage du plateau */
        afficherPlateau(plateau);

        /* Demander au joueur de faire un mouvement */
        int ligne, colonne;
        printf("Joueur %c, veuillez entrer la ligne et la colonne (1-3) séparées par un espace : ", joueurActuel);
        scanf("%d %d", &ligne, &colonne);

        /* Vérifier si la case est valide */
        if (ligne < 1 || ligne > 3 || colonne < 1 || colonne > 3 || plateau[ligne - 1][colonne - 1] != ' ') {
            printf("Mouvement invalide. Veuillez réessayer.\n");
            tour--; /* Réduire le compteur de tour pour permettre au même joueur de rejouer */
            continue;
        }

        /* Mettre à jour le plateau avec le mouvement du joueur */
        plateau[ligne - 1][colonne - 1] = joueurActuel;

        /* Vérifier s'il y a un gagnant */
        if (verifierVictoire(plateau, joueurActuel)) {
            afficherPlateau(plateau);
            printf("Le joueur %c a gagné !\n", joueurActuel);
            break;
        }

        /* Vérifier s'il y a un match nul */
        if (verifierMatchNul(plateau)) {
            afficherPlateau(plateau);
            printf("Match nul !\n");
            break;
        }

        /* Passer au joueur suivant */
        joueurActuel = (joueurActuel == joueur1) ? joueur2 : joueur1;
    }

    return 0;
}

/**
 * void afficherPlateau(char plateau[3][3])
 * 
 * \brief Procédure qui affiche le plateau.
 * 
 * \param plateau[3][3] : La grille à afficher.
 */
void afficherPlateau(char plateau[3][3]) {
    printf("\nPlateau de jeu :\n");

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            /* Utilisation de caractères Unicode pour une meilleure représentation graphique */
            if (j < 2)
                printf(" %c |", plateau[i][j]);
            else
                printf(" %c ", plateau[i][j]);
        }

        if (i < 2)
            printf("\n-----------\n");
    }

    printf("\n");
}

/**
 * int verifierVictoire(char plateau[3][3], char joueur)
 * 
 * \brief Fonction qui vérifie s'il y a un gagnant.
 * 
 * \param plateau[3][3] : La grille à vérifier. 
 * 
 * \param joueur : Vérifie si le joueur a gagné. 
 * 
 * \return 0 si le joueur n'est pas gagnant, 1 si le joueur est gagnant.
 */
int verifierVictoire(char plateau[3][3], char joueur) {
   
