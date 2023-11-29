#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct {
    double pos_x;
    double pos_y;
} Position;

typedef struct {
    char* nom_restaurant;
    char* adresse_restaurant;
    Position position_restaurant;
    char* specialite;
} Restaurant;

int lire_restaurant(char* chemin, Restaurant **restaurantsPtr) {
    FILE *fp;
    char* line = NULL;
    size_t len = 0;
    size_t read;

    fp=fopen(chemin, "r");
    if (fp == NULL) {
        return -1;
    }

    // Obtenir le nombre de lignes avant d'allouer
    size_t nbLines=0;
    for (; (read = getline(&line, &len, fp)) != -1; ++nbLines);
    *restaurantsPtr = malloc(nbLines * sizeof(Restaurant));
    Restaurant* restaurants = *restaurantsPtr;
    if (!restaurants)
        return -1;

    // Remplir les restaurants
    rewind(fp);
    int index=0;
    while ((read = getline(&line, &len, fp)) != -1) {
        const char delimiter[2]=";";

        const char* nom = strtok(line, delimiter);
        const char* adresse = strtok(NULL, delimiter);
        const char* position = strtok(NULL, delimiter);
        const char* specialite = strtok(NULL, delimiter);

        if (nom == NULL || adresse == NULL || position == NULL || specialite == NULL) {
            continue; // pour passer les lignes vides ou incorrectes
        }

        restaurants[index].nom_restaurant = malloc(sizeof(char) * (strlen(nom) + 1));
        restaurants[index].adresse_restaurant = malloc(sizeof(char) * (strlen(adresse) + 1));
        restaurants[index].specialite = malloc(sizeof(char) * (strlen(specialite) + 1)); 

        strcpy(restaurants[index].nom_restaurant, nom);
        strcpy(restaurants[index].adresse_restaurant, adresse);
        strcpy(restaurants[index].specialite, specialite);

        sscanf(position, " (x=%lf, y=%lf)", 
            &restaurants[index].position_restaurant.pos_x, 
            &restaurants[index].position_restaurant.pos_y);

        sscanf(specialite, " {%[^}]}", restaurants[index].specialite);

        ++index;
    }

    fclose(fp);
    if (line) {
        free(line);
    }

    int nbRestaurants=index;
    return nbRestaurants;
}

int inserer_restaurant(char* chemin, Restaurant restaurant) {
    FILE *fp;
    fp = fopen(chemin, "a");
    if (fp == NULL)
        return 0;

    fprintf(fp, "\n");
    fprintf(fp, "%s; ", restaurant.nom_restaurant);
    fprintf(fp, "%s; ", restaurant.adresse_restaurant);
    fprintf(fp, "(x=%lf, y=%lf); ", restaurant.position_restaurant.pos_x, restaurant.position_restaurant.pos_y);
    fprintf(fp, "{%s};", restaurant.specialite);

    fclose(fp);
    return 1;
}

int cherche_restaurant(char* chemin, double x, double y, double  rayon_recherche, Restaurant** resultsPtr) {
    Restaurant *restaurants;
    int len = lire_restaurant("restau.txt", &restaurants);
    *resultsPtr = malloc(len * sizeof(Restaurant));
    Restaurant* results = *resultsPtr;

    int resultsIndex = 0;
    for (int i = 0; i < len; ++i) {
        Restaurant restaurant = restaurants[i];
        Position restauPos = restaurant.position_restaurant;
        
        int xInRange = fabs(restauPos.pos_x - x) < rayon_recherche,
            yInRange = fabs(restauPos.pos_y - y) < rayon_recherche;
        if (xInRange && yInRange) {
            results[resultsIndex] = restaurant;
            ++resultsIndex;
        }
    }

    return resultsIndex;
}

int cherche_par_specialite(char* chemin, double x, double y, char* specialite[], size_t nbSpecialites, Restaurant** resultsPtr) {
    Restaurant *restaurants;
    int len = lire_restaurant("restau.txt", &restaurants);
    *resultsPtr = malloc(len * sizeof(Restaurant));
    Restaurant* results = *resultsPtr;
    double* resultDistances = malloc(len * sizeof(double));
    int resultsIndex=0;

    if (nbSpecialites == 0)
        return 0;

    for (int i = 0; i < len; ++i) {
        Restaurant restaurant = restaurants[i];
        
        for (int specIndex = 0; specIndex < nbSpecialites; ++specIndex) {
            if (strcmp(restaurant.specialite, specialite[specIndex]) == 0) { // égalité trouvée
                results[resultsIndex] = restaurant;

                // Calcul de la distance avec la position de l'utilisateur
                double diffX = restaurant.position_restaurant.pos_x - x,
                    diffY = restaurant.position_restaurant.pos_y - y;
                double distance = sqrt(diffX*diffX + diffY*diffY);
                resultDistances[resultsIndex] = distance;

                ++resultsIndex;
                break;
            }
        }
    }

    if (resultsIndex == 0)
        return 0;

    // Tri par sélection
    size_t resultsLength = resultsIndex;
    for (int i = 0; i < resultsLength - 1; i++) {
        int minIndex = i;
        for (int j = i + 1; j < resultsLength; j++) {
            if (resultDistances[j] < resultDistances[minIndex]) {
                minIndex = j;
            }
        }
        // swap distances
        double tmpDist = resultDistances[minIndex];
        resultDistances[minIndex] = resultDistances[i];
        resultDistances[i] = tmpDist;
        // swap restaurants
        Restaurant tmpRest = results[minIndex];
        results[minIndex] = results[i];
        results[i] = tmpRest;
    }

    free(resultDistances);
    return resultsLength;
}

void afficher_restaurants(Restaurant* restaurants, int len) {
    printf("[Nom | Adresse | Position | Specialité]\n");
    for (int i = 0; i < len; ++i) {
        Restaurant restaurant = restaurants[i];
        printf("%s | %s | POS(X: %lf, Y: %lf) | %s\n", restaurant.nom_restaurant, restaurant.adresse_restaurant, 
            restaurant.position_restaurant.pos_x, restaurant.position_restaurant.pos_y, restaurant.specialite);
    }
}

int consoleReadline(char* line, int len, int flush) {
    if (flush) { // alternative à fflush
        int ch=0;
        while((ch = getchar()) != '\n' && ch != EOF);
    }

    // fgets
    if (!fgets(line, len, stdin)) {
        return 0;
    }

    line[strcspn(line, "\n")] = 0;
    return 1;
}

void prompt_position(double* x, double* y) {
    printf("Position (x,y) : ");
    scanf("%lf,%lf", x, y);
}

int prompt_specialites(char** specialitesPtr[]) {
    const size_t maxSpecialites=10, specialiteMaxLength=100;
    *specialitesPtr=malloc(maxSpecialites*sizeof(char*));
    char** specialites=*specialitesPtr;
    printf("Specialités (s1,s2,s3...) : ");

    const size_t inputMaxLength = maxSpecialites * specialiteMaxLength 
        + maxSpecialites + 1; // inclure les virgules et le \0
    char* input=malloc(sizeof(char)*inputMaxLength); 
    consoleReadline(input, specialiteMaxLength, 1);

    const char delimiter[2]=",";
    char* specialite = strtok(input, delimiter);
    int index=0;
    for (; specialite != NULL; index++) {
        specialites[index] = malloc(strlen(specialite) + 1);
        strcpy(specialites[index], specialite);
        specialite=strtok(NULL, delimiter);
    }
    int nbSpecialites = index;

    free(input);
    return nbSpecialites;
}

int main() {
    int choice = -1;
    char* chemin="restau.txt";

    while (choice < 0 || choice > 4) {
        printf("Quel action effectuer ?\n"
            "   1. Lire les restaurants du fichier\n"
            "   2. Ajouter un restaurant au fichier\n"
            "   3. Chercher un restaurant par position\n"
            "   4. Chercher un restaurant par spécialité\n"
            "   0. Quitter\n"
            "\nChoix: ");
        
        scanf("%d", &choice);

        switch (choice) {
            case 0:
                return 0;
            
            case 1: {
                Restaurant* restaurants;
                int len = lire_restaurant(chemin, &restaurants);
                afficher_restaurants(restaurants, len);
                free(restaurants);
            }
            break;

            case 2: {
                // inputs
                char inputNom[100], inputAdresse[100], inputSpecialite[100];
                double pos_x, pos_y;

                printf("Nom du restaurant: ");
                consoleReadline(inputNom, sizeof inputNom, 1);
                printf("Adresse du restaurant: ");
                consoleReadline(inputAdresse, sizeof inputAdresse, 0);

                prompt_position(&pos_x, &pos_y);

                printf("Spécialité du restaurant: ");
                consoleReadline(inputSpecialite, sizeof inputSpecialite, 1);

                // construction structure restaurant
                Restaurant restaurant;
                restaurant.nom_restaurant = inputNom;
                restaurant.adresse_restaurant = inputAdresse;
                restaurant.specialite = inputSpecialite; // TODO insérer automatiquement les {} au sein de la fonction
                Position restauPos;
                restauPos.pos_x = pos_x;
                restauPos.pos_y = pos_y;
                restaurant.position_restaurant = restauPos;

                printf("Entrée : %s | %s | POS(X: %f, Y: %f) | %s\n", restaurant.nom_restaurant, restaurant.adresse_restaurant, 
                    restaurant.position_restaurant.pos_x, restaurant.position_restaurant.pos_y, restaurant.specialite);

                // insertion
                int success = inserer_restaurant(chemin, restaurant);
                if (success) {
                    printf("Insertion effectuée.");
                } else {
                    printf("L'insertion a echoué.");
                }
            }
            break;

            case 3: {
                Restaurant* results;
                double x, y, rayon;
                prompt_position(&x, &y);
                printf("Rayon : ");
                scanf("%lf", &rayon);
                int len = cherche_restaurant(chemin, x, y, rayon, &results);
                afficher_restaurants(results, len);
                free(results);
            }
            break;

            case 4: {
                Restaurant* results;
                double x, y;
                prompt_position(&x, &y);
                char** specialites;
                int nbSpecialites = prompt_specialites(&specialites);
                int len = cherche_par_specialite(chemin, x, y, specialites, nbSpecialites, &results);
                afficher_restaurants(results, len);
                free(results);
            }
            break;

            default:
                printf("\nAction incorrecte.\n");
            break;
        }
    }

    return 0;
}
