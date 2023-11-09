//Faudra pas oublier les commentaires :D  (attendons le cours dessus pour les faire plus propre)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

enum color{NOTHING, CYAN, YELLOW, PURPLE, ORANGE, BLUE, RED, GREEN};
typedef enum color color;

typedef struct{
    unsigned i : 12; //Coordonnée x de la cellule
    unsigned j : 12; //Coordonnée y de la cellule
    bool isFull; //Boolean si la cell est occupé par une piece
    color c; //Couleur de la cellule
}cell;

typedef struct{
    char nom; //Nom de la pièce
    cell p[4]; //Piece (tableau de 4 cellules)
}piece;

const char HAUTEUR = 20;
const char LARGEUR = 10;

cell** init_board() {
    cell** board = (cell **) malloc(HAUTEUR * sizeof(cell*));
    if(!board){
        perror("Erreur malloc()\n");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i < HAUTEUR; i++) {
        board[i] = (cell *) malloc(LARGEUR * sizeof(cell));
        if (!board[i]) {
            perror("Erreur malloc()\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < LARGEUR; j++) {
            board[i][j].i = i;
            board[i][j].j = j; 
            board[i][j].isFull = false;
            board[i][j].c = NOTHING;
        }
    }
    return board;
}

void display_board(cell** c){
    for(int i=0; i<HAUTEUR; i++){
        for(int j=0; j<LARGEUR; j++){
            if(!c[i][j].isFull) printf(' ');
            else printf('#');
        }
        printf("\n");
    }
    printf("\n");
}

piece** init_tmpPiece(){
    //Ici on crée les 7 pieces du jeu et on les stock dans le tableau
    char noms_pieces[] = {'I', 'O', 'T', 'L', 'J', 'Z', 'S'};
    piece[7][] tmpPiece;

    for (int i = 0; i < 7; i++) {
        tmpPiece[i] = (piece *)malloc(sizeof(piece));
        if (!tmpPiece[i]) {
            perror("Erreur malloc()\n");
            exit(EXIT_FAILURE);
        }
        tmpPiece[i]->nom = noms_pieces[i];
        tmpPiece[i]->p = (cell*) malloc(4 * sizeof(cell));
        //On crée toutes les pièces
        if(tmpPiece[i]->nom == 'I'){
            tmpPiece[i]->p[0].i = 0;
            tmpPiece[i]->p[0].j = 3;
            tmpPiece[i]->p[0].isFull=true;
            tmpPiece[i]->p[0].c=RED;

            tmpPiece[i]->p[1].i = 0;
            tmpPiece[i]->p[1].j = 4;
            tmpPiece[i]->p[1].isFull=true;
            tmpPiece[i]->p[1].c=RED;

            tmpPiece[i]->p[2].i = 0;
            tmpPiece[i]->p[2].j = 5;
            tmpPiece[i]->p[2].isFull=true;
            tmpPiece[i]->p[2].c=RED;

            tmpPiece[i]->p[3].i = 0;
            tmpPiece[i]->p[3].j = 6;
            tmpPiece[i]->p[3].isFull=true;
            tmpPiece[i]->p[3].c=RED;
        }
        else if(tmpPiece[i]->nom == 'O'){
            //On crée la pièce
            tmpPiece[i]->p[0].i = 0;
            tmpPiece[i]->p[0].j = 4;
            tmpPiece[i]->p[0].isFull=true;
            tmpPiece[i]->p[0].c=RED;

            tmpPiece[i]->p[1].i = 0;
            tmpPiece[i]->p[1].j = 5;
            tmpPiece[i]->p[1].isFull=true;
            tmpPiece[i]->p[1].c=RED;

            tmpPiece[i]->p[2].i = 1;
            tmpPiece[i]->p[2].j = 4;
            tmpPiece[i]->p[2].isFull=true;
            tmpPiece[i]->p[2].c=RED;

            tmpPiece[i]->p[3].i = 1;
            tmpPiece[i]->p[3].j = 5;
            tmpPiece[i]->p[3].isFull=true;
            tmpPiece[i]->p[3].c=RED;
        }
        else if(tmpPiece[i]->nom == 'T'){
            //On crée la pièce
            tmpPiece[i]->p[0].i = 0;
            tmpPiece[i]->p[0].j = 4;
            tmpPiece[i]->p[0].isFull=true;
            tmpPiece[i]->p[0].c=RED;

            tmpPiece[i]->p[1].i = 0;
            tmpPiece[i]->p[1].j = 5;
            tmpPiece[i]->p[1].isFull=true;
            tmpPiece[i]->p[1].c=RED;

            tmpPiece[i]->p[2].i = 0;
            tmpPiece[i]->p[2].j = 6;
            tmpPiece[i]->p[2].isFull=true;
            tmpPiece[i]->p[2].c=RED;

            tmpPiece[i]->p[3].i = 1;
            tmpPiece[i]->p[3].j = 5;
            tmpPiece[i]->p[3].isFull=true;
            tmpPiece[i]->p[3].c=RED;
        }
        else if(tmpPiece[i]->nom == 'L'){
            //On crée la pièce
            tmpPiece[i]->p[0].i = 0;
            tmpPiece[i]->p[0].j = 4;
            tmpPiece[i]->p[0].isFull=true;
            tmpPiece[i]->p[0].c=RED;

            tmpPiece[i]->p[1].i = 0;
            tmpPiece[i]->p[1].j = 5;
            tmpPiece[i]->p[1].isFull=true;
            tmpPiece[i]->p[1].c=RED;

            tmpPiece[i]->p[2].i = 0;
            tmpPiece[i]->p[2].j = 6;
            tmpPiece[i]->p[2].isFull=true;
            tmpPiece[i]->p[2].c=RED;

            tmpPiece[i]->p[3].i = 1;
            tmpPiece[i]->p[3].j = 4;
            tmpPiece[i]->p[3].isFull=true;
            tmpPiece[i]->p[3].c=RED;
        }
        else if(tmpPiece[i]->nom == 'J'){
            //On crée la pièce
            tmpPiece[i]->p[0].i = 0;
            tmpPiece[i]->p[0].j = 4;
            tmpPiece[i]->p[0].isFull=true;
            tmpPiece[i]->p[0].c=RED;

            tmpPiece[i]->p[1].i = 0;
            tmpPiece[i]->p[1].j = 5;
            tmpPiece[i]->p[1].isFull=true;
            tmpPiece[i]->p[1].c=RED;

            tmpPiece[i]->p[2].i = 0;
            tmpPiece[i]->p[2].j = 6;
            tmpPiece[i]->p[2].isFull=true;
            tmpPiece[i]->p[2].c=RED;

            tmpPiece[i]->p[3].i = 1;
            tmpPiece[i]->p[3].j = 6;
            tmpPiece[i]->p[3].isFull=true;
            tmpPiece[i]->p[3].c=RED;
        }
        else if(tmpPiece[i]->nom == 'Z'){
            //On crée la pièce
            tmpPiece[i]->p[0].i = 0;
            tmpPiece[i]->p[0].j = 4;
            tmpPiece[i]->p[0].isFull=true;
            tmpPiece[i]->p[0].c=RED;

            tmpPiece[i]->p[1].i = 0;
            tmpPiece[i]->p[1].j = 5;
            tmpPiece[i]->p[1].isFull=true;
            tmpPiece[i]->p[1].c=RED;

            tmpPiece[i]->p[2].i = 1;
            tmpPiece[i]->p[2].j = 5;
            tmpPiece[i]->p[2].isFull=true;
            tmpPiece[i]->p[2].c=RED;

            tmpPiece[i]->p[3].i = 1;
            tmpPiece[i]->p[3].j = 6;
            tmpPiece[i]->p[3].isFull=true;
            tmpPiece[i]->p[3].c=RED;
        }
        else if(tmpPiece[i]->nom == 'S'){
            //On crée la pièce
            tmpPiece[i]->p[0].i = 0;
            tmpPiece[i]->p[0].j = 5;
            tmpPiece[i]->p[0].isFull=true;
            tmpPiece[i]->p[0].c=RED;

            tmpPiece[i]->p[1].i = 0;
            tmpPiece[i]->p[1].j = 6;
            tmpPiece[i]->p[1].isFull=true;
            tmpPiece[i]->p[1].c=RED;

            tmpPiece[i]->p[2].i = 1;
            tmpPiece[i]->p[2].j = 4;
            tmpPiece[i]->p[2].isFull=true;
            tmpPiece[i]->p[2].c=RED;

            tmpPiece[i]->p[3].i = 1;
            tmpPiece[i]->p[3].j = 5;
            tmpPiece[i]->p[3].isFull=true;
            tmpPiece[i]->p[3].c=RED;
        }
    }

    return tmpPiece;
}

piece* getPiece(){
    //On crée une pièce qu'on a memcpy sur une piece de tmpPiece aléatoirement
    //Ajouter la piece au tableau de pieces (si c'est vide, on malloc, sinon realloc)
    //On retourne la pièce
}

void moveDownPiece(cell** c, piece *p){
    //Et vérif que c'est pas tout en bas
}
void moveLeftPiece(cell** c, piece *p){
    //Et vérif que c'est pas tout à gauche
}
void moveRightPiece(cell** c, piece *p){
    //Et vérif que c'est pas tout à droite
}

void rotateLeft(cell** c, piece *p){
    //Si possible
}
void rotateRight(cell** c, piece *p){
    //Si possible
}




void clear_board(cell** c){
    for(int i = 0; i < HAUTEUR; i++) {
        free(c[i]);
    }
    free(c);
}

void clear_boardPiece(piece** p, int nbBoardPiece){
    for (int i=0; i<nbBoardPiece; i++) {
        free(p[i]);
    }
    free(p);
}

void clear_tmpPiece(piece** p){
    for (int i=0; i<7; i++) {
        free(p[i]);
    }
}




int main(int argc, char *argv[]){
    argc--;
    argv++;

    srand(time(NULL));

    //Tableau de pointeur de piece (les 7 pieces du jeu), qui serviront pour memcpy. Evite de devoir regénérer une piece à chaque fois.
    piece[7][] tmpPiece = init_tmpPiece();

    //Tableau de pointeur de piece qui stocke toutes les pièces qui ont été dans le jeu.
    piece** boardPiece = NULL;
    int nbBoardPiece=0;

    //Tableau de cellule, c'est notre jeu
    cell** board = init_board();

    display_board(board);




    clear_board(board);
    clear_boardPiece(boardPiece, nbBoardPiece);
    clear_tmpPiece(tmpPiece);

    return EXIT_SUCCESS;
}
