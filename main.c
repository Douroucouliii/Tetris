#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum couleur{RIEN, ROUGE, BLEU, JAUNE, VERT};
typedef enum couleur couleur;

typedef struct{
    bool estRempli;
    couleur c;
}cellule;

const char H = 20;
const char L = 10;

cellule** init_grille() {
    cellule** grille = (cellule **) malloc(H * sizeof(cellule*));
    if(!grille){
        perror("Erreur malloc()\n");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i < H; i++) {
        grille[i] = malloc(L * sizeof(cellule));
        if (!grille[i]) {
            perror("Erreur malloc()\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < L; j++) {
            grille[i][j].estRempli = false;
            grille[i][j].c = RIEN;
        }
    }
    return grille;
}

void display_grille(cellule** c){
    for(int i=0; i<H; i++){
        for(int j=0; j<L; j++){
            if(!c[i][j].estRempli) printf(" ");
            else printf("#");
        }
        printf("\n");
    }
    printf("\n");
}

void clear(cellule** c){
    for(int i = 0; i < H; i++) {
        free(c[i]);
    }
    free(c);
}

int main(int argc, char *argv[]){
    argc--;
    argv++;

    cellule** grille = init_grille();
    display_grille(grille);



    clear(grille);

    return EXIT_SUCCESS;
}