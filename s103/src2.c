/**
 * Ce fichier source contient un jeu de PUISSANCE 4 complet.
 * Jouable uniquement dans le terminal LINUX.
 * Le jeu consiste à aligner 4 jetons d'une même forme.
 * Le jeu se joue dans une grille de 7 * 6.
 * Le joueur1 pose un jeton puis le joueur2 et ainsi de suite jusqu'à que soit:
 * - La grille soit pleine et il sera donc impossible de continuer la partie, et elle sera considérée comme "égalité".
 * - Un joueur aligne 4 jetons de sa forme.
 *
 * Bon jeu à vous !
 */

#include <stdio.h>

// Déclaration des constantes DEFINE
#define LIGNES 6   /** Nombre de lignes présentes dans une grille de puissance 4*/
#define COLONNES 7 /** Nombre de colonnes présentes dans une grille de puissance 4*/

// Déclaration des constantes

const int ID1 = 1;
const int ID2 = 2;

// Variables globales

int victoire1; /** Nombre de victoire du joueur 1*/
int victoire2; /** Nombre de victoire du joueur 2*/
int egalite;   /** Nombre de partie se finissant avec une égalité*/
int partie;    /** Nombre de partie jouée au total */

// Déclaration d'une nouvelle structure
typedef struct
{
    char nom[20];   /** Le nom du joueur*/
    int id;         /**Le numéro du joueur qui permet de l'identifier*/
    char couleur;   /** La couleur des jetons du joueur*/
    int nbvictoire; /** Le nombre de victoire du joueur*/
} str_joueur;       /** Structure d'un joueur*/

typedef struct
{
    int nbjetons;    /** Le nombre de jetons qui ont été posé depuis le début*/
    int nbcoller;    /** Le nombre de jetons maximum qui se colle */
    float moyerreur; /** La moyenne du nombres d'erreurs rencontré par le programme. */
} str_stats;         /** Statistique d'un jeu*/

// Fonction pour afficher le plateau
void afficherPlateau(char plateau[LIGNES][COLONNES]);

// Fonction pour placer un jeton dans la colonne choisie
int placerJeton(char plateau[LIGNES][COLONNES], int colonne, char joueur);

// Fonction pour vérifier s'il y a un gagnant
int verifierVictoire(char plateau[LIGNES][COLONNES], int ligne, int colonne, char joueur);

int main()
{
    char plateau[LIGNES][COLONNES];

    // Initialiser le plateau
    for (int i = 0; i < LIGNES; i++)
    {
        for (int j = 0; j < COLONNES; j++)
        {
            plateau[i][j] = ' ';
        }
    }

    char joueurActuel = 'X'; /**Nom du joueur*/
    int colonne;             /**Le numéro de la colonne choisi par le joueur*/

    // Boucle principale du jeu
    while (1)
    {
        // Afficher le plateau
        afficherPlateau(plateau);

        // Demander au joueur de faire un mouvement
        printf("Joueur %c, veuillez entrer la colonne (1-%d) : ", joueurActuel, COLONNES);
        scanf("%d", &colonne);

        // Vérifier si la colonne est valide
        if (colonne < 1 || colonne > COLONNES)
        {
            printf("Colonne invalide. Veuillez réessayer.\n");
            continue;
        }

        // Placer le jeton dans la colonne choisie
        int ligne = placerJeton(plateau, colonne - 1, joueurActuel);

        // Vérifier s'il y a un gagnant
        if (verifierVictoire(plateau, ligne, colonne - 1, joueurActuel))
        {
            afficherPlateau(plateau);
            printf("Le joueur %c a gagné !\n", joueurActuel);
            break;
        }

        // Passer au joueur suivant
        joueurActuel = (joueurActuel == 'X') ? 'O' : 'X';
    }

    return 0;
}

// Fonction pour afficher le plateau
/**
 * \brief Permet d'afficher la grille actuelle.
 * \details Cette fonction est composé de 4 boucles FOR,
 * il permet d'afficer la présence ou non d'un jeton mais aussi de dessiner le plateau à chaque tour.
 *
 * \param char plateau[LIGNES][COLONNES], c'est le tableau avec lequel les jetons se placent.
 */
void afficherPlateau(char plateau[LIGNES][COLONNES])
{
    printf("\n");

    for (int i = 0; i < LIGNES; i++)
    {
        for (int j = 0; j < COLONNES; j++)
        {
            printf("|%c", plateau[i][j]);
        }
        printf("|\n");
    }

    for (int i = 0; i < COLONNES * 2 + 1; i++)
    {
        printf("-");
    }

    printf("\n");

    for (int i = 1; i <= COLONNES; i++)
    {
        printf(" %d", i);
    }

    printf("\n\n");
}

// Fonction pour placer un jeton dans la colonne choisie

/**
 * \brief Place le jeton dans le plateau.
 * \details Cet fonction permet de savoir si on peut placer le jeton et aussi le met dans le plateau en fonction du plateau.
 *
 * \return int, Numéro de la ligne
 * \param char plateau[LIGNES][COLONNES], c'est le tableau avec lequel les jetons se placent.
 * \param int colonne, numéro de la colonne choisi par le joueur
 * \param char joueur, c'est le nom du joueur.
 */
int placerJeton(char plateau[LIGNES][COLONNES], int colonne, char joueur)
{
    int ligne;

    for (ligne = LIGNES - 1; ligne >= 0; ligne--)
    {
        if (plateau[ligne][colonne] == ' ')
        {
            plateau[ligne][colonne] = joueur;
            break;
        }
    }

    return ligne;
}

// Fonction pour vérifier s'il y a un gagnant
/**
 * \brief Vérifie si une victoire est possible avec le nouveau jeton.
 * \details Cet fonction a 4 étapes:
 * - Vérification horizontale
 * - Vérification verticale
 * - Vérification diagonale vers la droite
 * - Vérification diagonale vers la gauche
 *
 * \return int, si le résultat est égal à 1 alors c'est qu'il y a une victoire, sinon 0.
 * \param char plateau[LIGNES][COLONNES], c'est le tableau avec lequel les jetons se placent.
 * \param int colonne, numéro de la colonne choisi par le joueur
 * \param int ligne, numéro de la colonne renvoyé par la fonction placeJeton.
 * \param char joueur, nom du joueur.
 */
int verifierVictoire(char plateau[LIGNES][COLONNES], int ligne, int colonne, char joueur)
{
    int compte;

    // Vérification horizontale
    compte = 0;
    for (int j = 0; j < COLONNES; j++)
    {
        if (plateau[ligne][j] == joueur)
        {
            compte++;
            if (compte == 4)
                return 1;
        }
        else
        {
            compte = 0;
        }
    }

    // Vérification verticale
    compte = 0;
    for (int i = 0; i < LIGNES; i++)
    {
        if (plateau[i][colonne] == joueur)
        {
            compte++;
            if (compte == 4)
                return 1;
        }
        else
        {
            compte = 0;
        }
    }

    // Vérification diagonale vers la droite
    compte = 0;
    for (int i = ligne, j = colonne; i < LIGNES && j < COLONNES; i++, j++)
    {
        if (plateau[i][j] == joueur)
        {
            compte++;
            if (compte == 4)
                return 1;
        }
        else
        {
            compte = 0;
        }
    }
    for (int i = ligne - 1, j = colonne - 1; i >= 0 && j >= 0; i--, j--)
    {
        if (plateau[i][j] == joueur)
        {
            compte++;
            if (compte == 4)
                return 1;
        }
        else
        {
            compte = 0;
        }
    }

    // Vérification diagonale vers la gauche
    compte = 0;
    for (int i = ligne, j = colonne; i < LIGNES && j >= 0; i++, j--)
    {
        if (plateau[i][j] == joueur)
        {
            compte++;
            if (compte == 4)
                return 1;
        }
        else
        {
            compte = 0;
        }
    }
    for (int i = ligne - 1, j = colonne + 1; i >= 0 && j < COLONNES; i--, j++)
    {
        if (plateau[i][j] == joueur)
        {
            compte++;
            if (compte == 4)
                return 1;
        }
        else
        {
            compte = 0;
        }
    }

    return 0;
}
