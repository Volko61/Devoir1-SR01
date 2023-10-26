#include <stdio.h>  // Librarie standard d'entrée sortie
#include <stdlib.h> // Librarie standard pour l'allocation de mémoire

#define N 4             // Nombres d'etudiants
#define NUMBER_RANGES 8 // Nombre de tranches de notes (max 8 car après ca augmente le nombre de caractères pour afficher les tranches de notes, provoquerait des bugs d'affichages et serait compliqué à patcher)

char *returnRepeatedChar(char c, int n) //Utile pour faire des strings avec des caractères répétés plus simplement et plus lisiblement. Prend un caractère et un nombre et retourne un string avec le caractère répété n fois
{
    char *str = malloc((n + 1) * sizeof(char)); // On augmente la taille de 1 pour laisser de la place pour le null terminator
    for (int i = 0; i < n; i++) // On remplit le tableau avec le caractère c n fois
    {
        str[i] = c;
    }
    str[n] = '\0'; // On ajoute le null terminator
    return str; // Et on retourne le string généré
}


int getMaxNotes(int notes[])    // Prend un tableau de notes et retourne la valeur max du tableau notes (Utile pour le rendu responsive du graphique)
{
    int max = notes[0]; // fonctionne comme getMaxPoints mais avec NUMBER_RANGEE au lieu de N
    for (int i = 1; i < NUMBER_RANGES; i++)
    {
        if (notes[i] > max)
        {
            max = notes[i];
        }
    }
    return max;
}

int getMaxPoints(int points[])  // Prend un tableau de points et retourne le maximum de tous les points
{
    int max = points[0];    // On initialise le maximum à la première valeur du tableau (si on initialisait à 0 ça ne marcherait pas si tous les points sont négatifs même si ca ne devrait normalement pas arriver)
    for (int i = 1; i < N; i++) // On parcourt le tableau
    {
        if (points[i] > max) // Si la valeur actuelle est plus grande que le maximum actuel on remplace le maximum par la valeur actuelle
        {
            max = points[i];
        }
    }
    return max; // On retourne le maximum
}

int getMinPoints(int points[])  // Prend un tableau de points et retourne le minimum de tous les points
{
    int min = points[0];    // Comme pour getMaxPoints mais avec < au lieu de >
    for (int i = 1; i < N; i++)
    {
        if (points[i] < min)
        {
            min = points[i];
        }
    }
    return min;
}

int getMoyenneNotes(int points[])   // Prend un tableau de points et retourne la moyenne
{
    int sum = 0;
    for (int i = 0; i < N; i++) // On fait la somme de tous les points
    {
        sum += points[i];
    }
    return sum / N; //et on divise par le nombre de points (donc d'étudiants)
}

int *pointsToNotes(int points[])    // Prend un tableau de points et retourne un tableau de notes avec le nombre de points par tranche
{
    // A TERMINER
    int *notes = malloc(NUMBER_RANGES * sizeof(int)); // On alloue de la mémoire pour les notes (j'ai fait un tableau dynamique parce que je ne sais pas pourquoi je n'arrivais pas à faire une fonction qui retourne un tableau statique)
    for (int i = 0; i < NUMBER_RANGES; i++) // On initialise le tableau à 0
    {
        notes[i] = 0; 
    }
    for (int i = 0; i < N; i++) // Ca ne marchait pas avec un switch donc j'ai fait ça avec des if
    {
        notes[(int)points[i]/10]++; // On divise par 10 pour avoir le numéro de la tranche de notes. On applique un int pour arrondir au nombre entier inférieur (ex: 19/10 = 1.9 -> 1)
    }
    return notes;       // On retourne le tableau de notes
}

void graphNuagePoint(int notes[])
{
    for (int i = getMaxNotes(notes); 0 <= i; i--)  // Pour chaque ligne (y a autant de ligne que la valeur max de notes pour faire un graphique responsive)
    {
        if (i == 0)     // On fait un affichage différent pour l'étage 0 (avec des +--+)
        {
            printf("   ");  // On affiche 3 espaces pour aligner le graphique vu que normalement on affiche "0 |"
            for (int j = 0; j < NUMBER_RANGES; j++) // Pour chaque tranche de notes
            {
                if (notes[j] == i)  // Si il y a une note à l'étage 0 on affiche un +-o-+ sinon on affiche +-----
                {
                    printf("+-o---");
                }
                else
                {
                    printf("+-----");
                }
            }
        }
        else    // Si on est pas à l'étage 0 on affiche normalement
        {
            printf("%d >", i);      // On affiche le numéro de l'étage de manière jolie
            for (int j = 0; j < (NUMBER_RANGES + 1); j++)   // Pour chaque tranche de notes (responsive en fonction de NUMBER_RANGES afin de pouvoir facilement modifier le code pour avoir des tranches différentes)
            {
                if (notes[j] == i) // Si il y a une note à l'étage on affiche un o sinon on affiche des espaces
                {
                    printf("  o   ");
                }
                else
                {
                    printf("      ");
                }
            }
            printf("\n");   // Nouvelle ligne pour passer à l'étage suivant (plus bas) 
        }
    }
    printf("+\n   | 0-9 ");     // On fait un affichage spécifique pour la tranche 0-9 parce que c'est la seule qui a qu'un chifre pour chaque nombre (0 a un chiffre et 10 a deux chiffres)
    for (int i = 1; i < NUMBER_RANGES - 1; i++)     // Pour les autres tranches on affiche |10-19 |20-29 etc... C'est fait automatiquement en fonction de NUMBER_RANGES
    {
        printf("|%d-%d", i * 10, i * 10 + 9);
    }
    printf("| %d  |\n", (NUMBER_RANGES-1)*10);        // 60 (ou autre) est un peu particulier donc on le met en dehors de la boucle
}

void stats(int points[], int *notes)
{
    printf("------------------------------\n");
    for (int i = 0; i < NUMBER_RANGES - 1; i++) // On affiche le nombre de notes dans chaque tranche
    {
        printf("Nombre de notes entre %d et %d: %d\n", i * 10, i * 10 + 9, notes[i]);
    }
    printf("Nombre de notes a %d: %d\n", (NUMBER_RANGES-1)*10, notes[6]);
    printf("------------------------------\n");
    printf("La note maximale est: %d\n", getMaxPoints(points));
    printf("La note minimale est: %d\n", getMinPoints(points));
    printf("La moyenne est: %d\n", getMoyenneNotes(points));
    printf("------------------------------\n");
}

int main()
{
    if(NUMBER_RANGES < 0 || 8 < NUMBER_RANGES) { // On vérifie que le nombre de tranches de notes est inférieur ou égal à 8 et positif car sinon ça va provoquer des bugs d'affichage
        printf("Le nombre de tranches de notes doit etre inferieur ou égal a 8 et positif\n");
        return 1;
    }
    int points[N];  // On initialise le tableau des points
    int isGood = 0; // On initialise la variable qui va nous permettre de vérifier que les points sont bien entre 0 et 60
    for (int i = 0; i < N; i++) // On demande les points pour chaque étudiant
    {
        while (isGood != 1) // On répete la question tant que les points ne sont pas entre 0 et 60
        {
            printf("Enter the points for student %d: ", i + 1); 
            scanf("%d", &points[i]);
            //On vérifie que les points sont bien entre 0 et 60. Sinon on répete la question
            if(points[i] >= 0 && points[i] <= (NUMBER_RANGES-1)*10) {
                isGood = 1; // On passe la variable à 1 pour sortir de la boucle
            }
            else {
                printf("The points must be between 0 and %d\n", (NUMBER_RANGES-1)*10); // Sinon on affiche un message d'erreur
            }
        }
        isGood = 0; // On remet la variable à 0 pour la prochaine question
    }
    int *notes = pointsToNotes(points); // On mets les points dans des tranches de notes
    for (int i = 0; i < N; i++)
    {
        printf("%d\n", points[i]);
    }
    
    for (int i = 0; i < NUMBER_RANGES; i++) // On affiche les points et les notes pour chaque étudiant
    {
        printf("tranche %d : %d | %d-%d\n", i + 1, notes[i], i * 10, i * 10 + 9);
    }
    stats(points, notes);   // On fait apparaitre quelques stats
    graphNuagePoint(notes); // On lance l'affichage du graphique
    free(notes); // On libere la mémoire
    return 0;
}