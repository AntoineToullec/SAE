/*  CHANGEMENT PAR RAPPORT AU PRÉCEDENT SUDOKU

------ AJOUTS DE TECHNIQUES ------------

- Singleton caché ne fonctionnait pas, il marche maintenant.

- Paires_nu n'existait pas, sa version ligne et colonne on aussi été ajouté

------- AMELIORATIONS ------------------

- backtracking a maintenant des boucles while pour sortir plus rapidement (puisqu'il faut éviter de mettre plusieurs return,
nous avons opté pour cette options bien que les return soient plus rapide)

- L'affichage de la grille marche maintenant avec n'importe quel taille de grille, la taille du terminal limite cepandant
  la taille maximale que peux prendre la grille.
------- EXPLICATION MAIN -----------------

- Le programme essayera de résoudre la grille sans backtracking,
  Le programme tourne donc en boucle, tant qu'une fonction agît sur la grille
  (a une exception près expliqué en commentaire dans le code)
  une fois que chacune des fonctions n'arrivent plus a agir, backtracking finira la grille
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define N 4
#define TAILLE (N * N)
#define MAX ((N * N) * (N * N))
#define NB_LIGNES (N * N)
#define NB_COLONNES (N * N)

char nomFichier[30];

typedef struct
{
    int valeur;
    int candidats[TAILLE];
    int nbCandidats;
} tCase1;

typedef tCase1 tGrille[NB_LIGNES][NB_COLONNES];
tGrille grille1;

void chargerGrille(tGrille grille1, char nomFichier[30]);
void afficherGrille(tGrille grille1);
bool backtracking(tGrille g, int numeroCase);
bool possible(tGrille g, int numLigne, int numColonne, int valeur);
void txtgrilleInitiale();
void txtgrilleFinale();

void initialiserCandidats(tGrille grille1);
void ajouterCandidat(tCase1 *laCase, int valeur);
void retirerCandidat(tCase1 *laCase, int valeur);
bool estCandidat(tCase1 laCase, int valeur);
bool nbCandidats(tCase1 laCase, int valeur);

bool SingletonNu(tGrille grille1);
void ResetCandidat(tGrille grille1);
bool SingletonCache(tGrille grille1);
bool paires_nu(tGrille grille1);
bool paires_nu_ligne(tGrille grille1);
bool paires_nu_colonne(tGrille grille1);

int main()
{
    int numeroCase = 0;
    chargerGrille(grille1, nomFichier);
    clock_t begin = clock();
    txtgrilleInitiale();
    printf("\n");
    afficherGrille(grille1);

    bool avance = true;
    bool avance2 = true;
    bool avance3 = true;
    bool avance4 = true;
    bool avance5 = true;
    /*
avance correspond a chacune des techniques utilisés
comme elles renvoient toutes un bool, ça nous permet de déterminer
si elles ont eu un impact sur la grille ou non, et dans le cas ou
la fonction n'a pas eu d'impact, elle devient false
donc aucune fonction n'a d'impact, on laisse la grille a backtracking
*/
    while ((avance == true) || (avance2 == true) || (avance3 == true) || (avance4 == true) || (avance5 == true))
    {
        avance = false;
        avance2 = false;
        avance3 = false;
        initialiserCandidats(grille1);
        avance = SingletonNu(grille1);
        avance2 = paires_nu(grille1);
        avance3 = paires_nu_ligne(grille1);
        avance4 = paires_nu_colonne(grille1);
        avance5 = SingletonCache(grille1);
        // situation particulière dans laquel des candidats sont enlevés, mais aucun singleton ne peux insérer de candidat
        if ((avance3 == true) || (avance4 == true) || (avance5 == true))
        {
            avance3 = false;
            avance4 = false;
            avance5 = false;
            // Ici on met tout a false parce que sinon ça va boucler a l'infini
        }
        ResetCandidat(grille1);
    }
    txtgrilleFinale();
    printf("\n");

    backtracking(grille1, numeroCase);
    afficherGrille(grille1);

    clock_t end = clock();
    double tmpsCPU = (end - begin) * 1.0 / CLOCKS_PER_SEC;
    printf("Résolution de la %s, temps CPU = %.6f secondes\n", nomFichier, tmpsCPU);
    return EXIT_SUCCESS;
}

void chargerGrille(tGrille grille, char nomFichier[30])
{
    FILE *f;

    printf("Nom du fichier ? \n");
    scanf("%s", nomFichier);

    f = fopen(nomFichier, "rb");
    if (f == NULL)
    {
        printf("\nERREUR sur le fichier %s\n", nomFichier);
        exit(EXIT_FAILURE);
    }
    else
    {
        for (int ligne = 0; ligne < TAILLE; ligne++)
        {
            for (int col = 0; col < TAILLE; col++)
            {
                fread(&grille[ligne][col], sizeof(int), 1, f);
            }
        }
        fclose(f);
    }
}

void afficherGrille(tGrille g)
{
    // CONSTANTES POUR L'AFFICHAGE
    const char barreVert = '|';
    const char barreHori = '-';
    const char point = '.';
    const char coin = '+';
    const char espace = ' ';
    int indiceL = 1;
    int tailleTIRET = 0;
    printf("    ");
    if (N < 4)
    {
        if (N < 3)
        {
            tailleTIRET = 3 * N;
        }
        else
        {
            tailleTIRET = N * N;
        }
    }
    else
    {
        tailleTIRET = 4 * N;
    }
    for (int indiceC = 1; indiceC <= (N * N); indiceC++) // AFFICHE LES INDICES DES COLONNES
    {
        if (indiceC < 10)
        {
            if (N < 4)
            {
                printf(" %d ", indiceC);
            }
            else
            {
                printf(" %c%d ", espace, indiceC);
            }
        }
        else
        {
            printf("%c%d%c", espace, indiceC, espace);
        }
        if (indiceC % N == 0)
        {
            printf("%c", espace);
        }
    }
    printf("\n");
    for (int i = 0; i < (N * N); i++) // ON AUGMENTE DE 1 LA LIGNE
    {
        if ((i % N == 0))
        {
            printf("%c%c%c%c", espace, espace, espace, coin);
            for (int ligneSep = 0; ligneSep < N; ligneSep++)
            {
                for (int ligneTiret = 0; ligneTiret < tailleTIRET; ligneTiret++)
                {
                    printf("%c", barreHori);
                }
                printf("%c", coin);
            }
            printf("\n");
        }

        if (indiceL <= 9)
        {
            printf("%c%d%c", espace, indiceL, espace);
        }
        else
        {
            printf("%d%c", indiceL, espace);
        }
        indiceL++;
        for (int j = 0; j < (N * N); j++) // ON AUGMENTE DE 1 LA COLONNE.
        {

            if (j % N == 0)
            {
                printf("%c", barreVert);
            }
            if (grille1[i][j].valeur == 0) // LES 0 DEVIENNENT DES . SINON ON AFFICHE LA VALEUR.
            {
                if (N < 4)
                {
                    printf("%c%c%c", espace, point, espace);
                }
                else
                {
                    printf("%c%c%c%c", espace, espace, point, espace);
                }
            }
            // affiche la valeur
            else
            {
                if (grille1[i][j].valeur <= 9)
                {
                    if (N < 4)
                    {
                        printf("%c%d%c", espace, grille1[i][j].valeur, espace);
                    }
                    else
                    {
                        printf("%c%c%d%c", espace, espace, grille1[i][j].valeur, espace);
                    }
                }
                else
                {
                    printf("%c%d%c", espace, grille1[i][j].valeur, espace);
                }
            }
        }
        printf("%c\n", barreVert); // BARRE VERTICALE POUR CHAQUE FIN DE LIGNE
    }
    printf("%c%c%c%c", espace, espace, espace, coin); // DEBUT DE LA BOUCLE POUR LE TRAIT DE SEPARATION HORIZONTALE ENTRE 3 BLOCKS
    for (int ligneSep = 0; ligneSep < N; ligneSep++)
    {
        for (int ligneTiret = 0; ligneTiret < tailleTIRET; ligneTiret++)
        {
            printf("%c", barreHori);
        }
        printf("%c", coin);
    }
    printf("\n"); // RETOUR A LA LIGNE POUR CONTINUER SUR UNE NOUVELLE LIGNE
}

void txtgrilleInitiale()
{
    const char barreHori = '-';
    const char coin = '+';
    const char espace = ' ';
    printf("\n%c%c%c%c", espace, espace, espace, coin);
    for (int i = 0; i < ((TAILLE * N) - 12) / 2; i++)
    {
        printf("%c", barreHori);
    }
    printf("Grille Initiale");
    for (int i = 0; i < ((TAILLE * N) - 12) / 2; i++)
    {
        printf("%c", barreHori);
    }
    printf("%c\n", coin);
}

void txtgrilleFinale()
{
    const char barreHori = '-';
    const char coin = '+';
    const char espace = ' ';
    printf("\n%c%c%c%c", espace, espace, espace, coin);
    for (int i = 0; i < ((TAILLE * N) - 10) / 2; i++)
    {
        printf("%c", barreHori);
    }
    printf("Grille Finale");
    for (int i = 0; i < ((TAILLE * N) - 10) / 2; i++)
    {
        printf("%c", barreHori);
    }
    printf("%c\n", coin);
}

bool backtracking(tGrille g, int numeroCase)
{
    int ligne, colonne, valeur;
    bool resultat = false;
    if (numeroCase == TAILLE * TAILLE)
    {
        resultat = true;
    }
    else
    {
        // c'est censé être la partie ou on récup les coordonnées de la case
        ligne = numeroCase / TAILLE;
        colonne = numeroCase % TAILLE;

        if (g[ligne][colonne].valeur != 0)
        {
            // Si la case est pas vide, on passe a la suivante
            resultat = backtracking(g, numeroCase + 1);
        }
        else
        {
            for (valeur = 1; valeur <= TAILLE; valeur++)
            {
                if (possible(g, ligne, colonne, valeur) == true)
                {
                    g[ligne][colonne].valeur = valeur;
                    if (backtracking(g, numeroCase + 1) == true)
                    {
                        resultat = true;
                        break;
                    }
                    else
                    {
                        g[ligne][colonne].valeur = 0;
                    }
                }
            }
        }
    }
    return resultat;
}

bool possible(tGrille g, int numLigne, int numColonne, int valeur)
{
    bool end = true;
    int boucleL, boucleC;
    // LIGNE
    for (int i = 0; (i < TAILLE) && (end == true); i++)
    {
        if (g[numLigne][i].valeur == valeur)
        {
            end = false;
        }
    }

    // COLONNE
    for (int j = 0; (j < TAILLE) && (end == true); j++)
    {
        if (valeur == g[j][numColonne].valeur)
        {
            end = false;
        }
    }

    //  BLOC
    // on cherche à mettre le curseur dans le carré, en haut à gauche.
    while ((numLigne != 0 * N) && (numLigne != N) && (numLigne != 2 * N) && (numLigne != 3 * N) && (end == true))
    {
        numLigne = numLigne - 1;
    }
    while ((numColonne != 0 * N) && (numColonne != N) && (numColonne != 2 * N) && (numColonne != 3 * N) && (end == true))
    {
        numColonne = numColonne - 1;
    }
    // on regarde si dans le carré c'est bon
    for (boucleL = numLigne; (boucleL < numLigne + N) && (end == true); boucleL++)
    {
        for (boucleC = numColonne; (boucleC < numColonne + N) && (end == true); boucleC++)
        {
            if (g[boucleL][boucleC].valeur == valeur)
            {
                end = false;
            }
        }
    }
    return end;
}

void initialiserCandidats(tGrille g)
{
    // printf("\n\n");
    for (int boucle_ligne = 0; boucle_ligne < TAILLE; boucle_ligne++)
    {
        for (int boucle_colonne = 0; boucle_colonne < TAILLE; boucle_colonne++)
        {
            if (g[boucle_ligne][boucle_colonne].valeur == 0)
            {
                for (int candid_poss = 0; candid_poss <= TAILLE; candid_poss++)
                {
                    if (possible(g, boucle_ligne, boucle_colonne, candid_poss) == true)
                    {
                        ajouterCandidat(&g[boucle_ligne][boucle_colonne], candid_poss);
                    }
                }
            }
        }
    }
}

bool SingletonNu(tGrille g)
{
    bool avance = false;
    int boucle_l = 0;
    for (boucle_l = 0; boucle_l < TAILLE; boucle_l++)
    {
        int boucle_c = 0;
        for (boucle_c = 0; boucle_c < TAILLE; boucle_c++)
        {
            if (g[boucle_l][boucle_c].valeur == 0)
            {
                if (g[boucle_l][boucle_c].nbCandidats == 1)
                {
                    g[boucle_l][boucle_c].valeur = g[boucle_l][boucle_c].candidats[0];
                    avance = true;
                }
            }
        }
    }
    return avance;
}

bool SingletonCache(tGrille g)
{
    int valeur = 0;
    bool retour = false;
    bool unique = true;
    int boucle_l = 0;
    for (boucle_l = 0; boucle_l < TAILLE; boucle_l++)
    {
        int boucle_c = 0;
        for (boucle_c = 0; boucle_c < TAILLE; boucle_c++)
        {
            if (g[boucle_l][boucle_c].valeur == 0)
            {
                if (g[boucle_l][boucle_c].nbCandidats > 1)
                {
                    for (valeur = 0; valeur < TAILLE; valeur++)
                    {
                        unique = true;
                        int boucle_l_square;
                        for (boucle_l_square = 0; boucle_l_square < TAILLE; boucle_l_square++)
                        {
                            int boucle_c_square = 0;
                            for (boucle_c_square = 0; boucle_c_square < TAILLE; boucle_c_square++)
                            {
                                int parcandidat = 0; // c'est pour parcourir les candidats dans une case
                                for (parcandidat = 0; parcandidat < g[boucle_l_square][boucle_c_square].nbCandidats; parcandidat++)
                                {
                                    if ((g[boucle_l_square][boucle_c_square].candidats[parcandidat] == valeur) || (valeur == 0))
                                    {
                                        // Un break ou return nous aurait fais gagner du temps
                                        unique = false;
                                    }
                                }
                            }
                        }
                        if (unique == true)
                        {
                            if (possible(g, boucle_l, boucle_c, valeur) == true)
                            {
                                g[boucle_l][boucle_c].valeur = valeur;
                                retour = true;
                            }
                        }
                    }
                }
            }
        }
    }
    return retour;
}

bool paires_nu(tGrille g)
{
    int valeur = 0;
    bool retour = false;
    int boucle_l_square, boucle_c_square = 0; // c'est pour viser le coin du carré.
    int boucle_l_within, boucle_c_within = 0; // c'est pour parcourir le carré.
    int boucle_l = 0;
    for (boucle_l = 0; boucle_l < TAILLE; boucle_l++)
    {
        int boucle_c = 0;
        for (boucle_c = 0; boucle_c < TAILLE; boucle_c++)
        {
            if (g[boucle_l][boucle_c].nbCandidats == 2)
            {
                boucle_l_square = boucle_l; // tout ça c'est pour que le "viseur" soit dans l'angle du carré
                while ((boucle_l_square != 0 * N) && (boucle_l_square != N) && (boucle_l_square != 2 * N) && (boucle_l_square != 3 * N))
                {
                    boucle_l_square = boucle_l_square - 1;
                }
                while ((boucle_c_square != 0 * N) && (boucle_c_square != N) && (boucle_c_square != 2 * N) && (boucle_c_square != 3 * N))
                {
                    boucle_c_square = boucle_c_square - 1;
                }
                // on regarde si dans le carré c'est bon
                for (boucle_l_within = boucle_l_square; boucle_l_within < boucle_l_square + N; boucle_l_within++) // tdf : abrégeable par possible requete 3
                {
                    for (boucle_c_within = boucle_c_square; boucle_c_within < boucle_c_square + N; boucle_c_within++)
                    {
                        if ((boucle_l_within != boucle_l) && (boucle_c_within != boucle_c))
                        {
                            if (g[boucle_l_within][boucle_c_within].nbCandidats == 2)
                            {
                                int a = g[boucle_l_within][boucle_c_within].candidats[0];
                                int b = g[boucle_l_within][boucle_c_within].candidats[1];
                                int c = g[boucle_l][boucle_c].candidats[0];
                                int d = g[boucle_l][boucle_c].candidats[1];
                                int boucle_l_sup_duo, boucle_c_sup_duo;
                                // a ,b ,c ,d sont des raccourci pour éviter que les commandes qui suivent soient trop longues et difficiles a écrire
                                if ((a == c) && (b == d))
                                {
                                    for (boucle_l_sup_duo = boucle_l_square; boucle_l_sup_duo < boucle_l_square + N; boucle_l_sup_duo++)
                                    {
                                        for (boucle_c_sup_duo = boucle_l_square; boucle_c_sup_duo < boucle_l_square + N; boucle_c_sup_duo++)
                                        {
                                            if (g[boucle_l_sup_duo][boucle_c_sup_duo].nbCandidats > 2)
                                            {
                                                // valeur ça va nous permettre de choisir un candidat, qui nous permettra de viser correctement
                                                // il a été défini au début, car je pensais qu'il m'aurait servi plus tôt
                                                for (valeur = 0; valeur < g[boucle_l_sup_duo][boucle_c_sup_duo].nbCandidats; valeur++)
                                                {
                                                    if (g[boucle_l_sup_duo][boucle_c_sup_duo].candidats[valeur] == a)
                                                    {
                                                        retirerCandidat(&g[boucle_l_sup_duo][boucle_c_sup_duo], a);
                                                        retour = true;
                                                    }
                                                    else if (g[boucle_l_sup_duo][boucle_c_sup_duo].candidats[valeur] == b)
                                                    {
                                                        retirerCandidat(&g[boucle_l_sup_duo][boucle_c_sup_duo], b);
                                                        retour = true;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                                // le else permet de vérifier l'autre égalité
                                else if ((b == c) && (a == d))
                                {
                                    for (boucle_l_sup_duo = 0; boucle_l_sup_duo < TAILLE; boucle_l_sup_duo++)
                                    {
                                        for (boucle_c_sup_duo = 0; boucle_c_sup_duo < TAILLE; boucle_c_sup_duo++)
                                        {
                                            if (g[boucle_l_sup_duo][boucle_c_sup_duo].nbCandidats >= 2)
                                            {
                                                // valeur ça va nous permettre de choisir un candidat, qui nous permettra de viser correctement
                                                // il a été défini au début parce que je pensais qu'il m'aurait servi plus tôt
                                                for (valeur = 0; valeur < g[boucle_l_sup_duo][boucle_c_sup_duo].nbCandidats; valeur++)
                                                {
                                                    if (g[boucle_l_sup_duo][boucle_c_sup_duo].candidats[valeur] == a)
                                                    {
                                                        retirerCandidat(&g[boucle_l_sup_duo][boucle_c_sup_duo], b);
                                                        retour = true;
                                                    }
                                                    else if (g[boucle_l_sup_duo][boucle_c_sup_duo].candidats[valeur] == b)
                                                    {
                                                        retirerCandidat(&g[boucle_l_sup_duo][boucle_c_sup_duo], a);
                                                        retour = true;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return retour;
}

bool paires_nu_ligne(tGrille g)
{
    int valeur = 0;
    bool retour = false;
    int boucle_c_ligne = 0; // c'est pour parcourir le carré.
    int boucle_l = 0;
    for (boucle_l = 0; boucle_l < TAILLE; boucle_l++)
    {
        int boucle_c = 0;
        for (boucle_c = 0; boucle_c < TAILLE; boucle_c++)
        {
            if (g[boucle_l][boucle_c].nbCandidats == 2)
            {
                // on regarde si dans la ligne c'est bon
                for (boucle_c_ligne = 0; boucle_c_ligne < TAILLE; boucle_c_ligne++) // tdf : abrégeable par possible requete 3
                {
                    if ((boucle_c_ligne != boucle_c))
                    {
                        if (g[boucle_l][boucle_c_ligne].nbCandidats == 2)
                        {
                            int a = g[boucle_l][boucle_c_ligne].candidats[0];
                            int b = g[boucle_l][boucle_c_ligne].candidats[1];
                            int c = g[boucle_l][boucle_c].candidats[0];
                            int d = g[boucle_l][boucle_c].candidats[1];
                            int boucle_c_sup_duo;
                            // a ,b ,c ,d sont des raccourci pour éviter que les commandes qui suivent soient trop longues et difficiles a écrire
                            if ((a == c) && (b == d))
                            {
                                for (boucle_c_sup_duo = 0; boucle_c_sup_duo < TAILLE; boucle_c_sup_duo++)
                                {
                                    if (g[boucle_l][boucle_c_sup_duo].nbCandidats > 2)
                                    {
                                        // valeur ça va nous permettre de choisir un candidat, qui nous permettra de viser correctement
                                        // il a été défini au début, car je pensais qu'il m'aurait servi plus tôt
                                        for (valeur = 0; valeur < g[boucle_l][boucle_c_sup_duo].nbCandidats; valeur++)
                                        {
                                            if (g[boucle_l][boucle_c_sup_duo].candidats[valeur] == a)
                                            {
                                                retirerCandidat(&g[boucle_l][boucle_c_sup_duo], a);
                                                retour = true;
                                            }
                                            else if (g[boucle_l][boucle_c_sup_duo].candidats[valeur] == b)
                                            {
                                                retirerCandidat(&g[boucle_l][boucle_c_sup_duo], b);
                                                retour = true;
                                            }
                                        }
                                    }
                                }
                            }
                            // le else permet de vérifier l'autre égalité
                            else if ((b == c) && (a == d))
                            {
                                for (boucle_c_sup_duo = 0; boucle_c_sup_duo < TAILLE; boucle_c_sup_duo++)
                                {
                                    if (g[boucle_l][boucle_c_sup_duo].nbCandidats >= 2)
                                    {
                                        // valeur ça va nous permettre de choisir un candidat, qui nous permettra de viser correctement
                                        // il a été défini au début parce que je pensais qu'il m'aurait servi plus tôt
                                        for (valeur = 0; valeur < g[boucle_l][boucle_c_sup_duo].nbCandidats; valeur++)
                                        {
                                            if (g[boucle_l][boucle_c_sup_duo].candidats[valeur] == a)
                                            {
                                                retirerCandidat(&g[boucle_l][boucle_c_sup_duo], b);
                                                retour = true;
                                            }
                                            else if (g[boucle_l][boucle_c_sup_duo].candidats[valeur] == b)
                                            {
                                                retirerCandidat(&g[boucle_l][boucle_c_sup_duo], a);
                                                retour = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return retour;
}

bool paires_nu_colonne(tGrille g)
{
    int valeur = 0;
    bool retour = false;
    int boucle_l_colonne; // c'est pour parcourir le carré.
    int boucle_c = 0;
    for (boucle_c = 0; boucle_c < TAILLE; boucle_c++)
    {
        int boucle_l = 0;
        for (boucle_l = 0; boucle_c < TAILLE; boucle_c++)
        {
            if (g[boucle_l][boucle_c].nbCandidats == 2)
            {
                for (boucle_l_colonne = 0; boucle_l_colonne < 0 + N; boucle_l_colonne++) // tdf : abrégeable par possible requete 3
                {
                    if ((boucle_l_colonne != boucle_l))
                    {
                        if (g[boucle_l_colonne][boucle_c].nbCandidats == 2)
                        {
                            int a = g[boucle_l_colonne][boucle_c].candidats[0];
                            int b = g[boucle_l_colonne][boucle_c].candidats[1];
                            int c = g[boucle_l][boucle_c].candidats[0];
                            int d = g[boucle_l][boucle_c].candidats[1];
                            int boucle_l_sup_duo;
                            // a ,b ,c ,d sont des raccourci pour éviter que les commandes qui suivent soient trop longues et difficiles a écrire
                            if ((a == c) && (b == d))
                            {
                                for (boucle_l_sup_duo = 0; boucle_l_sup_duo < 0 + N; boucle_l_sup_duo++)
                                {
                                    if (g[boucle_l_sup_duo][boucle_c].nbCandidats > 2)
                                    {
                                        // valeur ça va nous permettre de choisir un candidat, qui nous permettra de viser correctement
                                        // il a été défini au début, car je pensais qu'il m'aurait servi plus tôt
                                        for (valeur = 0; valeur < g[boucle_l_sup_duo][boucle_c].nbCandidats; valeur++)
                                        {
                                            if (g[boucle_l_sup_duo][boucle_c].candidats[valeur] == a)
                                            {
                                                retirerCandidat(&g[boucle_l_sup_duo][boucle_c], a);
                                                retour = true;
                                            }
                                            else if (g[boucle_l_sup_duo][boucle_c].candidats[valeur] == b)
                                            {
                                                retirerCandidat(&g[boucle_l_sup_duo][boucle_c], b);
                                                retour = true;
                                            }
                                        }
                                    }
                                }
                            }
                            // le else permet de vérifier l'autre égalité
                            else if ((b == c) && (a == d))
                            {
                                for (boucle_l_sup_duo = 0; boucle_l_sup_duo < TAILLE; boucle_l_sup_duo++)
                                {
                                    if (g[boucle_l_sup_duo][boucle_c].nbCandidats >= 2)
                                    {
                                        // valeur ça va nous permettre de choisir un candidat, qui nous permettra de viser correctement
                                        // il a été défini au début parce que je pensais qu'il m'aurait servi plus tôt
                                        for (valeur = 0; valeur < g[boucle_l_sup_duo][boucle_c].nbCandidats; valeur++)
                                        {
                                            if (g[boucle_l_sup_duo][boucle_c].candidats[valeur] == a)
                                            {
                                                retirerCandidat(&g[boucle_l_sup_duo][boucle_c], b);
                                                retour = true;
                                            }
                                            else if (g[boucle_l_sup_duo][boucle_c].candidats[valeur] == b)
                                            {
                                                retirerCandidat(&g[boucle_l_sup_duo][boucle_c], a);
                                                retour = true;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    return retour;
}

void retirerCandidat(tCase1 *laCase, int val)
{
    int candidat = 0;
    int decalage = candidat;
    int decalage_case;
    for (candidat = 0; candidat < laCase->nbCandidats; candidat++)
    {
        // printf(" candidat candidat%d\n", laCase->candidats[candidat]);
        if (laCase->candidats[candidat] == val)
        {
            if (laCase->nbCandidats > 1)
            {
                for (decalage_case = 0; decalage_case < laCase->nbCandidats; decalage_case++)
                {
                    // printf("%d\n candidat 0\n", laCase->candidats[0]);
                    decalage = candidat;
                    laCase->candidats[decalage] = laCase->candidats[decalage + 1];
                    laCase->nbCandidats--;
                }
            }
            else
            {
                laCase->nbCandidats = 0;
            }
        }
    }
}

void ajouterCandidat(tCase1 *laCase, int val)
{
    laCase->candidats[laCase->nbCandidats] = val;
    laCase->nbCandidats++;
}

void ResetCandidat(tGrille g)
{
    for (int boucle_ligne_set = 0; boucle_ligne_set < TAILLE; boucle_ligne_set++)
    {
        for (int boucle_colonne_set = 0; boucle_colonne_set < TAILLE; boucle_colonne_set++)
        {
            if (g[boucle_ligne_set][boucle_colonne_set].valeur == 0)
            {
                while (g[boucle_ligne_set][boucle_colonne_set].nbCandidats != 0)
                {
                    retirerCandidat(&g[boucle_ligne_set][boucle_colonne_set], g[boucle_ligne_set][boucle_colonne_set].candidats[g[boucle_ligne_set][boucle_colonne_set].nbCandidats - 1]);
                }
            }
        }
    }
}

/*

- LISTE DE TOUT LES MESSAGES UTILISÉ POUR DÉBOGGUER LE PROGRAMME


printf("Sorti de singleton nu \n\n\n\n\n\n\n") ;
printf("Sorti de paires_nu\n\n\n\n\n\n\n") ;
printf("Sorti de paires_nuligne\n\n\n\n\n\n\n") ;
printf("Sorti de paires_nucolonne\n\n\n\n\n\n\n") ;
printf("Sorti de singleton cache\n\n\n\n\n\n\n") ;
printf("-----------------\nOh, on vient de boucle d'ailleurs !\n-----------------------\n") ;
printf("candid_poss = %d, l = %d c = %d |check case before everth %d, %d, %d \n", candid_poss, boucle_ligne + 1, boucle_colonne + 1, g[boucle_ligne][boucle_colonne].valeur, g[boucle_ligne][boucle_colonne].candidats[g[boucle_ligne][boucle_colonne].nbCandidats - 1], g[boucle_ligne][boucle_colonne].nbCandidats);
printf("nb candid de case[%d][%d] = %d\n",boucle_ligne+1,boucle_colonne+1,g[boucle_ligne][boucle_colonne].nbCandidats) ;
printf("check case wawawa %d, %d, %d \n", g[boucle_ligne][boucle_colonne].valeur, g[boucle_ligne][boucle_colonne].candidats[0], g[boucle_ligne][boucle_colonne].nbCandidats);
printf("l = %d, c = %d, nbcandi = %d, candid[0] = %d\n", boucle_l, boucle_c, g[boucle_l][boucle_c].nbCandidats, g[boucle_l][boucle_c].candidats[0]);
On est en droit de se demander si possible est réellement utile
printf("Une valeur a été assigné par Singleton Nu (case [%d][%d] = %d)\n", boucle_l+1, boucle_c+1, g[boucle_l][boucle_c].valeur);
printf("l = %d, c = %d, nbcandi = %d, candid[0] = %d\n", boucle_l, boucle_c, g[boucle_l][boucle_c].nbCandidats, g[boucle_l][boucle_c].candidats[0]);
printf("Une valeur a été assigné par Singleton Cache (%d)\n",valeur);
printf("1rs Des candidats ont été supprimé suite a une intervention de paires nu (%d et %d)\n", a,b);
printf("btw c'est a la case[%d][%d]\n",boucle_l_sup_duo+1,boucle_c_sup_duo+1) ;
printf("1rs Des candidats ont été supprimé suite a une intervention de paires nu (%d et %d)\n", a,b);
printf("btw c'est a la case[%d][%d]\n",boucle_l_sup_duo+1,boucle_c_sup_duo+1) ;
printf("2nd Des candidats ont été supprimé suite a une intervention de paires nu (%d et %d)\n", a,b);
printf("btw c'est a la case[%d][%d]\n",boucle_l_sup_duo+1,boucle_c_sup_duo+1) ;
printf("2nd Des candidats ont été supprimé suite a une intervention de paires nu (%d et %d)\n", a,b);
printf("btw c'est a la case[%d][%d]\n",boucle_l_sup_duo+1,boucle_c_sup_duo+1) ;
printf("1rs Des candidats ont été supprimé suite a une intervention de paires nu ligne(%d et %d)\n", a,b);
printf("btw c'est a la case[%d][%d]\n",boucle_l+1,boucle_c_sup_duo+1) ;                      
printf("1rs Des candidats ont été supprimé suite a une intervention de paires nu ligne(%d et %d)\n", a,b);
printf("btw c'est a la case[%d][%d]\n",boucle_l+1,boucle_c_sup_duo+1) ;
printf("2nd Des candidats ont été supprimé suite a une intervention de paires nu ligne(%d et %d)\n", a,b);
printf("btw c'est a la case[%d][%d]\n",boucle_l+1,boucle_c_sup_duo+1) ;
printf("2nd Des candidats ont été supprimé suite a une intervention de paires nu ligne(%d et %d)\n", a,b);
printf("btw c'est a la case[%d][%d]\n",boucle_l+1,boucle_c_sup_duo+1) ;
printf("1rs Des candidats ont été supprimé suite a une intervention de paires nu colonne(%d et %d)\n", a,b);
printf("btw c'est a la case[%d][%d]\n",boucle_l_sup_duo+1,boucle_c+1) ;
printf("1rs Des candidats ont été supprimé suite a une intervention de paires nu colonne(%d et %d)\n", a,b);
printf("btw c'est a la case[%d][%d]\n",boucle_l_sup_duo+1,boucle_c+1) ;
printf("2nd Des candidats ont été supprimé suite a une intervention de paires nu (%d et %d)\n", a,b);
printf("btw c'est a la case[%d][%d]\n",boucle_l_sup_duo+1,boucle_c+1) ;
printf("2nd Des candidats ont été supprimé suite a une intervention de paires nu (%d et %d)\n", a,b);
printf("btw c'est a la case[%d][%d]\n",boucle_l_sup_duo+1,boucle_c+1) ;
printf(" candidat candidat%d\n", laCase->candidats[candidat]);
printf("%d\n candidat 0\n", laCase->candidats[0]);


*/