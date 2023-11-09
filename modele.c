//Faudra pas oublier les commentaires :D  (attendons le cours dessus pour les faire plus propre)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

enum couleur{RIEN, CYAN, JAUNE, VIOLET, ORANGE, BLEU, ROUGE, VERT};
typedef enum couleur couleur;

typedef struct{
    unsigned i : 6; //Coordonnée x de la cellule
    unsigned j : 5; //Coordonnée y de la cellule
    bool estRempli; //Boolean si la cellule est occupé par une piece
    couleur c; //Couleur de la cellule
}cellule;

typedef struct{
    cellule p[4]; //piece (tableau de 4 cellule)
}piece;

const char H = 20;
const char L = 10;

cellule** init_grille() {
    cellule** grille = (cellule **) malloc(H * sizeof(cellule*));
    if(!grille){
        perror("Erreur malloc()\n");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i < H; i++) {
        grille[i] = (cellule *) malloc(L * sizeof(cellule));
        if (!grille[i]) {
            perror("Erreur malloc()\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < L; j++) {
            grille[i][j].i = i;
            grille[i][j].j = j; 
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