#include <stdio.h>
#include <stdlib.h>

const char HAUTEUR_GRILLE = 20;
const char LARGEUR_GRILLE = 10;

char** init_grille(){
    char** grille;
    for(int i=0; i<HAUTEUR_GRILLE; i++){
        for(int j=0; j<LARGEUR_GRILLE; j++){
            grille[i][j]='/';
        }
    }
}

void display_grille(char ** grille){
    for(int i=0; i<HAUTEUR_GRILLE; i++){
        for(int j=0; j<LARGEUR_GRILLE; j++){
            printf("%c", grille[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


int main(int argc, char *argv[]){

    char** grille = init_grille();
    display_grille(grille);



    return EXIT_SUCCESS;
}