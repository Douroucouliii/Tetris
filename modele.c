//Faudra pas oublier les commentaires :D  (attendons le cours dessus pour les faire plus propre)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
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
    printf("+");
    for(int i=0; i<LARGEUR*2; i++){
        printf("-");
    }
    printf("+");
    printf("\n");
    for(int i=0; i<HAUTEUR; i++){
        printf("|");
        for(int j=0; j<LARGEUR; j++){
            if(!c[i][j].isFull) printf("  ");
            else printf("[]");
        }
        printf("|\n");
    }
    printf("+");
    for(int i=0; i<LARGEUR*2; i++){
        printf("-");
    }
    printf("+\n");
}

piece** init_tmpPiece(){
    //Ici on crée les 7 pieces du jeu et on les stock dans le tableau
    char noms_pieces[] = {'I', 'O', 'T', 'L', 'J', 'Z', 'S'};
    //On alloue le tableau
    piece** tmpPiece = (piece**) malloc(7 * sizeof(piece *));
    if(!tmpPiece){
        perror("Erreur malloc()\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 7; i++) {
        tmpPiece[i] = (piece *)malloc(sizeof(piece));
        if (!tmpPiece[i]) {
            perror("Erreur malloc()\n");
            exit(EXIT_FAILURE);
        }
        //On initialise à vide
        tmpPiece[i]->nom = noms_pieces[i];
        for (int j = 0; j < 4; j++) {
            tmpPiece[i]->p[j].i = 0;
            tmpPiece[i]->p[j].j = 0;
            tmpPiece[i]->p[j].isFull = false;
            tmpPiece[i]->p[j].c = NOTHING;
        }
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

piece* get_piece(piece** tmpPiece){
    //On crée une pièce qu'on a memcpy sur une piece de tmpPiece aléatoirement
    //On retourne la pièce
    int randomIndex = rand() % 7;
    piece* newPiece = (piece*)malloc(sizeof(piece));
    if (!newPiece) {
        perror("Erreur malloc()\n");
        exit(EXIT_FAILURE);
    }
    memcpy(newPiece, tmpPiece[randomIndex], sizeof(piece));

    return newPiece;
}

void set_piece(piece* p, piece*** boardPiece, cell** board, int *nbBoardPiece){
    *nbBoardPiece = *nbBoardPiece + 1;
    if (!*boardPiece) {
        *boardPiece = (piece **) malloc(sizeof(piece *));
    } else {
        *boardPiece = (piece **) realloc(*boardPiece, *nbBoardPiece * sizeof(piece *));
    }
    if (!*boardPiece) {
        perror("Erreur malloc()/realloc()\n");
        exit(EXIT_FAILURE);
    }

    //Allouer dynamiquement de la mémoire pour la nouvelle pièce
    (*boardPiece)[*nbBoardPiece - 1] = (piece *) malloc(sizeof(piece));
    if (!(*boardPiece)[*nbBoardPiece - 1]) {
        perror("Erreur malloc()\n");
        exit(EXIT_FAILURE);
    }

    //Copier les données de la pièce dans la nouvelle mémoire allouée
    memcpy((*boardPiece)[*nbBoardPiece - 1], p, sizeof(piece));

    //Ici on met à jour la grille
    for(int ind = 0; ind < 4; ind++){
        if(board[p->p[ind].i][p->p[ind].j].isFull == true){
            //Ca veut dire qu'on a perdu car les pieces sont tout en haut :(
            printf("\nTu as perdu la partie, dommage !\n\n");
            return;
        } else {
            board[p->p[ind].i][p->p[ind].j].isFull = true;
        }
        board[p->p[ind].i][p->p[ind].j].c = p->p[0].c;
    }
}

void moveDownPiece(piece *p){
    //vérif que c'est pas tout en bas (pas encore fait)
    for(int i=0; i<4; i++){
        p->p[i].i++;
    }
    //Attention il faut reload le board apres l'appel de cette fonction
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

void reload_board(cell** grille, piece **lesPieces, int nbBoardPiece){
    //On efface tout le board
    for(int i=0; i<HAUTEUR; i++){
        for(int j=0; j<LARGEUR; j++){
            grille[i][j].isFull=false;
        }
    }
    //On parcourt nos pieces et met à true les case où y'a des pieces
    for(int i=0; i<nbBoardPiece; i++){
        for(int j=0; j<4; j++){
            grille[lesPieces[i]->p[j].i][lesPieces[i]->p[j].j].isFull=true;
        }
    }
}

void clear_board(cell** c){
    if(!c) return;
    for(int i = 0; i < HAUTEUR; i++) {
        free(c[i]);
    }
    free(c);
}

void clear_boardPiece(piece** p, int nbBoardPiece){
    if(!p) return;
    for (int i=0; i<nbBoardPiece; i++) {
        free(p[i]);
    }
    free(p);
}

void clear_tmpPiece(piece** p){
    if(!p) return;
    for (int i=0; i<7; i++) {
        free(p[i]);
    }
    free(p);
}


int main(int argc, char *argv[]){
    argc--;
    argv++;

    srand(time(NULL));

    //Tableau de pointeur de piece (les 7 pieces du jeu), qui serviront pour memcpy. Evite de devoir regénérer une piece à chaque fois.
    piece** tmpPiece = init_tmpPiece();

    //Tableau de pointeur de piece qui stocke toutes les pièces qui ont été dans le jeu.
    piece** lesPieces = NULL;
    int nbLesPieces=0;

    //Tableau de cellule, c'est notre jeu
    cell** board = init_board();


    //Pour l'instant krol je prend une piece aléatoire avec le get (memcpy etc), et je la met dans la grille avec le set (qui refait un memcpy jspa si c'est opti):)
    piece* piece = get_piece(tmpPiece);
    set_piece(piece, &lesPieces, board, &nbLesPieces);
    free(piece);

    //Et dcp quand j'affiche mon tableau, ploup
    display_board(board);

    moveDownPiece(lesPieces[0]);
    reload_board(board, lesPieces, nbLesPieces);


    clear_board(board);
    clear_boardPiece(lesPieces, nbLesPieces);
    clear_tmpPiece(tmpPiece);

    return EXIT_SUCCESS;
}


//A l'aide krol, est ce qu'on ajoute la piece au tableau lesPieces une fois que c'est arrivé tout en bas ou non ?