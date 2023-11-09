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
    char* nom; //Nom de la pièce
    cell p[4]; //Piece (tableau de 4 cellules)
    color c; //Couleur de la pièce
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
            if(!c[i][j].isFull) printf(" ");
            else printf("#");
        }
        printf("\n");
    }
    printf("\n");
}

piece** init_tmp_piece(){
    //Ici on crée les 7 pieces du jeu et on les stock dans le tableau
    for(int i=0; i<7; i++){
        tmpPiece[i] = (piece *) malloc(sizeof(piece));
    }
    tmpPiece[0]->nom = "I";
    tmpPiece[1]->nom = "O";
    tmpPiece[2]->nom = "T";
    tmpPiece[3]->nom = "L";
    tmpPiece[4]->nom = "J";
    tmpPiece[5]->nom = "Z";
    tmpPiece[6]->nom = "S";
}
piece* getPiece(){
    //Ajouter la piece au tableau de pieces
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
void clear(cell** c){
    for(int i = 0; i < HAUTEUR; i++) {
        free(c[i]);
    }
    free(c);
}
int main(int argc, char *argv[]){
    argc--;
    argv++;
    srand(time(NULL));
    //Tableau de pointeur de piece (les 7 pieces du jeu), qui serviront pour memcpy. Evite de devoir regénérer une piece à chaque fois.
    piece[7][] tmpPiece = init_tmp_piece();
    //Tableau de pointeur de piece qui stocke toutes les pièces qui ont été dans le jeu.
    piece** boardPiece = NULL;
    //Tableau de cellule, c'est notre jeu
    cell** board = init_board();
    display_board(board);
    clear(board);
    return EXIT_SUCCESS;
}
