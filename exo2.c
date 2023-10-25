#include <stdio.h>
#include <stdlib.h>
#include "gnuplot_i/src/gnuplot_i.h"

#define N 4

int getMaxNotes (int notes[]){
    int max = notes[0];
    for (int i = 1; i < N; i++)
    {
        if (notes[i] > max)
        {
            max = notes[i];
        }
        
    }
    return max;
}

int getMinNotes (int notes[]){
    int min = notes[0];
    for (int i = 1; i < N; i++)
    {
        if (notes[i] < min)
        {
            min = notes[i];
        }
        
    }
    return min;
}

int getMoyenneNotes (int notes[]){
    int moyenne = 0;
    for (int i = 0; i < N; i++)
    {
        moyenne += notes[i];
    }
    return moyenne/N;
}

int getNumberOfNotesBetween (int notes[], int min, int max){
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (notes[i] >= min && notes[i] <= max)
        {
            count++;
        }
        
    }
    return count;
}
int getNumberOfThisNote (int notes[], int note){
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        if (notes[i] == note)
        {
            count++;
        }
        
    }
    return count;
}

int main (){
    int points[N];
    for (int i = 0; i < N; i++)
    {
        printf("Entrez les points de l'etudiant %d: ", i+1);
        scanf("%d", &points[i]);
    }
    printf("La note maximale est: %d\n", getMaxNotes(points));
    printf("La note minimale est: %d\n", getMinNotes(points));
    printf("La moyenne est: %d\n", getMoyenneNotes(points));

    // Plotting the notes as a point cloud
    gnuplot_ctrl *h;
    h = gnuplot_init();
    //gnuplot_setstyle(h, "points");
    //gnuplot_set_xlabel(h, "Student");
    //gnuplot_set_ylabel(h, "Points");
    //gnuplot_cmd(h, "set title 'Notes des etudiants'");
    //gnuplot_plot_xy(h, points, N, "Notes", "points");
    gnuplot_plot_equation(h, "sin(x)", "sine on first session");
    printf("Press enter to continue...");
    getchar();
    gnuplot_close(h);

    return 0;
}