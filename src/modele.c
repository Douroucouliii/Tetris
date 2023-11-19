//Faudra pas oublier les commentaires :D  (attendons le cours dessus pour les faire plus propre)
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "modele.h"

Tetris* tetris_init_(){
    //Bon pour l'instant je crée un tetris tout nul et vide mais voilà c'est là qu'on l'initialise
    Tetris* tetris = (Tetris *) calloc(1,sizeof(Tetris));
    if(!tetris){
        perror("calloc()\n");
        exit(EXIT_FAILURE);
    }
    return tetris;
}

void tetris_playGame(Tetris* tetris, userInterface ui){

    //ba dcp ici c'est notre "boucle pour lancer notre tetris"

    return;
}

cell** init_board() {
    cell** board = (cell **) malloc(HEIGHT * sizeof(cell*));
    if(!board){
        perror("Erreur malloc()\n");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i < HEIGHT; i++) {
        board[i] = (cell *) malloc(WIDTH * sizeof(cell));
        if (!board[i]) {
            perror("Erreur malloc()\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < WIDTH; j++) {
            board[i][j].i = i;
            board[i][j].j = j; 
            board[i][j].isFull = false;
            board[i][j].c = NOTHING;
        }
    }
    return board;
}

void display_board(cell** c) {
    printf("┌");
    for (int i = 0; i < WIDTH * 2; i++) {
        printf("─");
    }
    printf("┐\n");
    for (int i = 0; i < HEIGHT; i++) {
        printf("│");
        for (int j = 0; j < WIDTH; j++) {
            if (!c[i][j].isFull)
                printf("  ");
            else {
                // Utiliser des codes ANSI de couleur pour chaque pièce
                switch (c[i][j].c) {
                    case NOTHING:
                        printf("  ");
                        break;
                    case CYAN:
                        printf("\033[0;36m[]\033[0m");
                        break;
                    case YELLOW:
                        printf("\033[1;33m[]\033[0m");
                        break;
                    case PURPLE:
                        printf("\033[0;35m[]\033[0m");
                        break;
                    case ORANGE:
                        printf("\033[0;33m[]\033[0m");
                        break;
                    case BLUE:
                        printf("\033[0;34m[]\033[0m");
                        break;
                    case RED:
                        printf("\033[0;31m[]\033[0m");
                        break;
                    case GREEN:
                        printf("\033[0;32m[]\033[0m");
                        break;
                    default:
                        printf("[]");
                }
            }
        }
        printf("│\n");
    }
    printf("└");
    for (int i = 0; i < WIDTH * 2; i++) {
        printf("─");
    }
    printf("┘\n\n");
}

PieceConfig** init_tmpPiece() {
    PieceConfig** tmpPiece = (PieceConfig**) malloc(7 * sizeof(PieceConfig*));
    if (!tmpPiece) {
        perror("Erreur malloc()\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 7; i++) {
        tmpPiece[i] = (PieceConfig*) malloc(sizeof(PieceConfig));
        if (!tmpPiece[i]) {
            perror("Erreur malloc()\n");
            exit(EXIT_FAILURE);
        }
        // Initialise à vide
        tmpPiece[i]->nom = pieces[i].nom;
        tmpPiece[i]->num_cells = pieces[i].num_cells;
        tmpPiece[i]->c = pieces[i].c;
        for (int j = 0; j < 4; j++) {
            tmpPiece[i]->coords[j][0] = 0;
            tmpPiece[i]->coords[j][1] = 0;
        }
        // On lui attributs les coordonnées de chaque piece ( avec le tableau pieces )
        for (int j = 0; j < pieces[i].num_cells; j++) {
            tmpPiece[i]->coords[j][0] = pieces[i].coords[j][0];
            tmpPiece[i]->coords[j][1] = pieces[i].coords[j][1];
        }
    }
    return tmpPiece;
}

PieceConfig* get_piece(PieceConfig** tmpPiece) {
    // On crée une pièce qu'on a memcpy sur une pièce de tmpPiece aléatoirement
    // On retourne la pièce
    int randomIndex = rand() % 7;
    PieceConfig* newPiece = (PieceConfig*) malloc(sizeof(PieceConfig));
    if (!newPiece) {
        perror("Erreur malloc()\n");
        exit(EXIT_FAILURE);
    }
    memcpy(newPiece, tmpPiece[randomIndex], sizeof(PieceConfig));
    return newPiece;
}

void set_piece(PieceConfig* p, PieceConfig*** boardPiece, cell** board, int* nbBoardPiece) {
    // Ajouter la pièce au tableau de pièces (si c'est vide, on malloc, sinon realloc)
    // Mettre à jour la grille du jeu (si une pièce est déjà présente à un emplacement, c'est qu'on a perdu);
    (*nbBoardPiece)++;
    if (!*boardPiece) {
        *boardPiece = (PieceConfig**) malloc(*nbBoardPiece * sizeof(PieceConfig*));
    } else {
        *boardPiece = (PieceConfig**) realloc(*boardPiece, *nbBoardPiece * sizeof(PieceConfig*));
    }
    if (!*boardPiece) {
        perror("Erreur malloc()/realloc()\n");
        exit(EXIT_FAILURE);
    }
    (*boardPiece)[*nbBoardPiece - 1] = p;
    // Mettre à jour la grille
    for (int ind = 0; ind < p->num_cells; ind++) {
        int coord_x = p->coords[ind][0];
        int coord_y = p->coords[ind][1];
        if (coord_x < 0 || coord_x >= HEIGHT || coord_y < 0 || coord_y >= WIDTH) {
            // La pièce est en dehors des limites du plateau, donc on a perdu :(
            printf("\nTu as perdu la partie, dommage !\n\n");
            return;
        }
        if (board[coord_x][coord_y].isFull == true) {
            // La case est déjà occupée, donc on a perdu :(
            printf("\nTu as perdu la partie, dommage !\n\n");
            return;
        } else {
            board[coord_x][coord_y].isFull = true;
            board[coord_x][coord_y].c = p->c;
        }
    }
}

/*
La fonction CanMove prend en paramètre 
    board : notre plateau
    piece : une piece standard
    varX et varY : l'orientation de la piece ( Bas , Gauche , Droite , Haut )
Cette fonction ressor un type boolean qui nous permet de savoir si nous pouvons ou non aller dans une direction.
*/
bool canMove(cell** board, PieceConfig* piece, int varX , int varY){
    for(int i = 0 ; i < piece -> num_cells; i++){
        int coord_x = piece -> coords[i][0] + varX;
        int coord_y = piece -> coords[i][1] + varY;
        if( coord_x < 0 || coord_x >= HEIGHT || coord_y < 0 || coord_y >= WIDTH) return false;
    }
    return true;
}

void moveDownPiece(cell** c, PieceConfig *p){
    if(!canMove(c,p,1,0)){
        //déplacement pas possible
        return;
    }
    //On déplace la piece vers le bas
    for( int i = 0 ; i < p -> num_cells ; i++){
        p->coords[i][0]++;
    }
}

void moveLeftPiece(cell** c, PieceConfig *p){
    if(!canMove(c,p,0,-1)){
        return;
    }
    for( int i = 0 ; i < p -> num_cells ; i++){
        p->coords[i][1]--;
    }
}

void moveRightPiece(cell** c, PieceConfig *p){
    if(!canMove(c,p,0,1)){
        return;
    }
    for( int i = 0 ; i < p -> num_cells ; i++){
        p->coords[i][1]++;
    }
}


/*Si ça bug pour le 'I' : De base le pivot est à [2][0]*/
int getPivotX(PieceConfig* p){
    if(( p -> nom == 'J') || (p -> nom == 'T') || (p -> nom =='Z') || (p->nom == 'L') || (p->nom == 'I')){
        return p -> coords[1][0];
    }else if(p -> nom == 'S'){
        return p -> coords[0][0];
    }
    return -1;
}

int getPivotY(PieceConfig* p){
    if(( p -> nom == 'J') || (p -> nom == 'T') || (p -> nom =='Z') || (p->nom == 'L') || (p->nom == 'I') ){
        return p -> coords[1][1];
    }else if(p -> nom == 'S'){
        return p -> coords[0][1];
    }
    return -1;
}

/*
Fonction qui permet de rotate une piece standard.
    Direction : -1 //Rotation Gauche
    Direction :  1 //Rotation Droites
*/
bool canRotate(cell** c, PieceConfig* p, int rotationDirection){
    int newCoords[4][2];
    
    int pivotX = getPivotX(p);
    int pivotY = getPivotY(p);

    for( int i = 0 ; i < p -> num_cells ; i++){
        int oldX = p -> coords[i][0];
        int oldY = p -> coords[i][1];
        if(rotationDirection == 1 ){ //Rotation Droite
            newCoords[i][0] = pivotX - (oldY - pivotY);
            newCoords[i][1] = pivotY + (oldX - pivotX);
        }else if(rotationDirection == -1){ //Rotation Gauche
            newCoords[i][0] = pivotX + (oldY - pivotY);
            newCoords[i][1] = pivotY - (oldX - pivotX);
        }
        int coord_x = newCoords[i][0];
        int coord_y = newCoords[i][1];
        if( coord_x < 0 || coord_x >= HEIGHT || coord_y < 0 || coord_y >= WIDTH){
            return false;
        }
    }
    return true;
}

void rotateLeft(cell** board, PieceConfig* piece) {
    if((!canRotate(board, piece, -1)) || (piece -> nom == 'O')){
        return;
    }
    int pivotX = getPivotX(piece);
    int pivotY = getPivotY(piece);

    for (int i = 0; i < piece->num_cells; i++) {
        int oldX = piece->coords[i][0];
        int oldY = piece->coords[i][1];
        // Mise à jour des coordonnées après la rotation à gauche
        piece->coords[i][0] = pivotX + (oldY - pivotY);
        piece->coords[i][1] = pivotY - (oldX - pivotX);
    }
}

void rotateRight(cell** board, PieceConfig* piece) {
    if((!canRotate(board, piece, 1)) || (piece -> nom == 'O')) {
        return;
    }
    
    int pivotX = getPivotX(piece);
    int pivotY = getPivotY(piece);
    
    for (int i = 0; i < piece->num_cells; i++) {
        int oldX = piece->coords[i][0];
        int oldY = piece->coords[i][1];
        // Mise à jour des coordonnées après la rotation à droite
        piece->coords[i][0] = pivotX - (oldY - pivotY);
        piece->coords[i][1] = pivotY + (oldX - pivotX);
    }
}


void refresh_board(cell** grille, PieceConfig** lesPieces, int nbBoardPiece){
    //On efface le tableau
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            grille[i][j].isFull = false;
            grille[i][j].c = NOTHING; // pas oublier NOTHING
        }
    }

    for( int i = 0 ; i < nbBoardPiece ; i ++){
        PieceConfig* p = lesPieces[i];
        for (int ind = 0; ind < p->num_cells; ind++) {
            int coord_x = p->coords[ind][0];
            int coord_y = p->coords[ind][1];
            // Vérifier que les coordonnées sont valides ( au cas où ptet faire une fonction verifie vive le refactoring)
            if (coord_x >= 0 && coord_x < HEIGHT && coord_y >= 0 && coord_y < WIDTH) {
                grille[coord_x][coord_y].isFull = true;
                grille[coord_x][coord_y].c = p->c;
            }
        }
    }
}

void clear_board(cell** c){
    if(!c) return;
    for(int i = 0; i < HEIGHT; i++) {
        free(c[i]);
    }
    free(c);
}

void clear_boardPiece(PieceConfig** p, int nbBoardPiece){
    if(!p) return;
    for (int i=0; i<nbBoardPiece; i++) {
        free(p[i]);
    }
    free(p);
}

void clear_tmpPiece(PieceConfig** p){
    if(!p) return;
    for (int i=0; i<7; i++) {
        free(p[i]);
    }
    free(p);
}

int main(){
    srand(time(NULL));

    //Tableau de pointeur de piece (les 7 pieces du jeu), qui serviront pour memcpy. Evite de devoir regénérer une piece à chaque fois.
    PieceConfig** tmpPiece = init_tmpPiece();
    //Tableau de pointeur de piece qui stocke toutes les pièces qui ont été dans le jeu.
    PieceConfig** boardPiece = NULL;
    int nbBoardPiece = 0;
    //Tableau de cellule, c'est notre jeu
    cell** board = init_board();
    //Pour l'instant krol je prend une piece aléatoire avec le get (memcpy etc), et je la met dans la grille avec le set :)
    PieceConfig* piece = get_piece(tmpPiece);
    set_piece(piece,&boardPiece,board,&nbBoardPiece);
    display_board(board);
    // Déplace la pièce vers le bas, met à jour la grille et réaffiche le tableau
    moveDownPiece(board, piece);
    refresh_board(board, &piece, nbBoardPiece);
    display_board(board);
    //
    rotateLeft(board, piece);
    refresh_board(board, &piece, nbBoardPiece);
    display_board(board);

    clear_board(board);
    clear_boardPiece(boardPiece, nbBoardPiece);
    clear_tmpPiece(tmpPiece);

    return EXIT_SUCCESS;
}
