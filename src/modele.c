//Faudra pas oublier les commentaires :D  (attendons le cours dessus pour les faire plus propre)
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "modele.h"

PieceConfig pieces[7] = {
    {'I', 4, {{0, 3}, {0, 4}, {0, 5}, {0, 6}}, RED},
    {'O', 4, {{0, 4}, {0, 5}, {1, 4}, {1, 5}}, ORANGE},
    {'T', 4, {{0, 4}, {0, 5}, {0, 6}, {1, 5}}, GREEN},
    {'J', 4, {{0, 4}, {0, 5}, {0, 6}, {1, 6}}, CYAN},
    {'L', 4, {{0, 4}, {0, 5}, {0, 6}, {1, 4}}, YELLOW},
    {'Z', 4, {{0, 4}, {0, 5}, {1, 5}, {1, 6}}, BLUE},
    {'S', 4, {{0, 5}, {0, 6}, {1, 4}, {1, 5}}, PURPLE}
};

Tetris* tetris_init_(){
    Tetris* tetris = (Tetris *) calloc(1,sizeof(Tetris));
    if(!tetris){
        perror("calloc()\n");
        exit(EXIT_FAILURE);
    }
    //Tableau de cellule, c'est notre jeu
    init_board(tetris);

    //Tableau de pointeur de piece (les 7 pieces du jeu), qui serviront pour memcpy. Evite de devoir regénérer une piece à chaque fois.
    init_tmpPiece(tetris);

    //Tableau de pointeur de piece qui stocke toutes les pièces qui ont été dans le jeu.
    PieceConfig** boardPiece = NULL;

    tetris->boardPiece = boardPiece;
    //Compteur de piece dans le boardPiece
    tetris->nbBoardPiece = 0;
    tetris->ligne=20;
    tetris->colonne=10;

    return tetris;
}

void tetris_playGame(Tetris* tetris, userInterface ui){
    //Faut faire un modeleTxt, modeleSdl, modelenCurses, ici je fait juste pour le modele texte (il faudra le mettre à part plus tard genre pointeur de fonctions)
    srand(time(NULL));

    //Pour l'instant krol je prend une piece aléatoire avec le get (memcpy etc), et je la met dans la grille avec le set :)

    get_piece(tetris);
    ui.fonctions->affiche(tetris);

    char car = demander_caractere();
    while(1){
        if(car == 'q'){
            move_left_piece(tetris);
        }
        if(car == 's'){
            if(!move_down_piece(tetris)){
                get_piece(tetris);
                //clear_all(tetris, ui);
                //free(tetris);
                //return;
            }
        }
        if(car == 'd'){
            move_right_piece(tetris);
        }
        if(car == 'a'){
            rotate_left(tetris);
        }
        if(car == 'e'){
            rotate_right(tetris);
        }
        refresh_board(tetris);
        ui.fonctions->affiche(tetris);
        car = demander_caractere();
    }
    return;
}

char demander_caractere(){
    char car;
    do{
        scanf("%c", &car);
    } while(car!='q' && car!='s' && car!='d' && car!='a' && car!='e');
    return car;
}

void init_board(Tetris *tetris) {
    cell** board = (cell **) malloc(tetris->ligne * sizeof(cell*));
    if(!board){
        perror("Erreur malloc()\n");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i < tetris->ligne; i++) {
        board[i] = (cell *) malloc(tetris->colonne * sizeof(cell));
        if (!board[i]) {
            perror("Erreur malloc()\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < tetris->colonne; j++) {
            board[i][j].i = i;
            board[i][j].j = j; 
            board[i][j].isFull = false;
            board[i][j].c = NOTHING;
        }
    }
    tetris->board = board;
}

void init_tmpPiece(Tetris *tetris) {
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
    tetris->tmpPiece=tmpPiece;
}

//On ajoute une piece de tmpPiece dans le tableau
void get_piece(Tetris *tetris) {
    //on memcpy sur une pièce de tmpPiece aléatoirement et on l'ajoute dans le table des pieces
    int randomIndex = rand() % 7;
    PieceConfig* newPiece = (PieceConfig*) malloc(sizeof(PieceConfig));
    if (!newPiece) {
        perror("Erreur malloc()\n");
        exit(EXIT_FAILURE);
    }
    memcpy(newPiece, tetris->tmpPiece[randomIndex], sizeof(PieceConfig));                                                                                                       
    tetris->nbBoardPiece++;
    if (!tetris->boardPiece) {
        tetris->boardPiece = (PieceConfig**) malloc(tetris->nbBoardPiece * sizeof(PieceConfig*));
    } else {
        tetris->boardPiece = (PieceConfig**) realloc(tetris->boardPiece, tetris->nbBoardPiece * sizeof(PieceConfig*));
    }
    if (!tetris->boardPiece) {
        perror("Erreur malloc()/realloc()\n");
        exit(EXIT_FAILURE);
    }
    tetris->boardPiece[tetris->nbBoardPiece -1] = newPiece;                                                                                                                                                                                                                                                                 
    //On met à jour la grille
    for (int ind = 0; ind < newPiece->num_cells; ind++) {                                                                                                                                                       
        int coord_x = newPiece->coords[ind][0];
        int coord_y = newPiece->coords[ind][1];
        if (coord_x < 0 || coord_x >= tetris->ligne || coord_y < 0 || coord_y >= tetris->colonne) {
            // La pièce est en dehors des limites du plateau, donc on a perdu :(
            printf("\nTu as perdu la partie, dommage !\n\n");
            return;
        }
        if (tetris->board[coord_x][coord_y].isFull == true) {
            // La case est déjà occupée, donc on a perdu :(
            printf("\nTu as perdu la partie, dommage !\n\n");
            return;
        } else {
            tetris->board[coord_x][coord_y].isFull = true;
            tetris->board[coord_x][coord_y].c = newPiece->c;
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
bool can_move(Tetris *tetris, int varX, int varY){
    for(int i=0 ; i<tetris->boardPiece[tetris->nbBoardPiece -1]->num_cells; i++){
        int coord_x = tetris->boardPiece[tetris->nbBoardPiece -1]->coords[i][0] + varX;
        int coord_y = tetris->boardPiece[tetris->nbBoardPiece -1]->coords[i][1] + varY;
        if( coord_x < 0 || coord_x >= tetris->ligne || coord_y < 0 || coord_y >= tetris->colonne) return false;
    }
    return true;
}

bool move_down_piece(Tetris *tetris){
    if(!can_move(tetris,1,0)){
        //déplacement pas possible
        return false;
    }
    //On déplace la piece vers le bas
    for( int i = 0 ; i < tetris->boardPiece[tetris->nbBoardPiece -1]->num_cells; i++){
        tetris->boardPiece[tetris->nbBoardPiece-1]->coords[i][0]++;
    }
    return true;
}

bool move_left_piece(Tetris *tetris){
    if(!can_move(tetris,0,-1)){
        //déplacement pas possible
        return false;
    }
    //On déplace la piece vers le bas
    for( int i = 0 ; i < tetris->boardPiece[tetris->nbBoardPiece -1]->num_cells; i++){
        tetris->boardPiece[tetris->nbBoardPiece -1]->coords[i][1]--;
    }
    return true;
}

bool move_right_piece(Tetris *tetris){
    if(!can_move(tetris,0,1)){
        //déplacement pas possible
        return false;
    }
    //On déplace la piece vers le bas
    for( int i = 0 ; i < tetris->boardPiece[tetris->nbBoardPiece -1]->num_cells; i++){
        tetris->boardPiece[tetris->nbBoardPiece -1]->coords[i][1]++;
    }
    return true;
}


/*Si ça bug pour le 'I' : De base le pivot est à [2][0]*/
int get_pivot_X(PieceConfig* p){
    if(( p -> nom == 'J') || (p -> nom == 'T') || (p -> nom =='Z') || (p->nom == 'L') || (p->nom == 'I')){
        return p -> coords[1][0];
    }else if(p -> nom == 'S'){
        return p -> coords[0][0];
    }
    return -1;
}

int get_pivot_Y(PieceConfig* p){
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
bool can_rotate(Tetris* tetris, int rotationDirection){
    int newCoords[4][2];
    
    int pivotX = get_pivot_X(tetris->boardPiece[tetris->nbBoardPiece -1]);
    int pivotY = get_pivot_Y(tetris->boardPiece[tetris->nbBoardPiece -1]);

    for( int i = 0 ; i < tetris->boardPiece[tetris->nbBoardPiece -1]->num_cells ; i++){
        int oldX = tetris->boardPiece[tetris->nbBoardPiece -1]->coords[i][0];
        int oldY = tetris->boardPiece[tetris->nbBoardPiece -1]->coords[i][1];
        if(rotationDirection == 1 ){ //Rotation Droite
            newCoords[i][0] = pivotX - (oldY - pivotY);
            newCoords[i][1] = pivotY + (oldX - pivotX);
        }else if(rotationDirection == -1){ //Rotation Gauche
            newCoords[i][0] = pivotX + (oldY - pivotY);
            newCoords[i][1] = pivotY - (oldX - pivotX);
        }
        int coord_x = newCoords[i][0];
        int coord_y = newCoords[i][1];
        if( coord_x < 0 || coord_x >= tetris->ligne || coord_y < 0 || coord_y >= tetris->colonne){
            return false;
        }
    }
    return true;
}

void rotate_left(Tetris *tetris) {
    if((!can_rotate(tetris, -1)) || (tetris->boardPiece[tetris->nbBoardPiece -1] -> nom == 'O')){
        return;
    }
    int pivotX = get_pivot_X(tetris->boardPiece[tetris->nbBoardPiece -1]);
    int pivotY = get_pivot_Y(tetris->boardPiece[tetris->nbBoardPiece -1]);

    for (int i = 0; i < tetris->boardPiece[tetris->nbBoardPiece -1]->num_cells; i++) {
        int oldX = tetris->boardPiece[tetris->nbBoardPiece -1]->coords[i][0];
        int oldY = tetris->boardPiece[tetris->nbBoardPiece -1]->coords[i][1];
        // Mise à jour des coordonnées après la rotation à gauche
        tetris->boardPiece[tetris->nbBoardPiece -1]->coords[i][0] = pivotX + (oldY - pivotY);
        tetris->boardPiece[tetris->nbBoardPiece -1]->coords[i][1] = pivotY - (oldX - pivotX);
    }
}

void rotate_right(Tetris* tetris) {
    if((!can_rotate(tetris, 1)) || (tetris->boardPiece[tetris->nbBoardPiece -1] -> nom == 'O')){
        return;
    }
    int pivotX = get_pivot_X(tetris->boardPiece[tetris->nbBoardPiece -1]);
    int pivotY = get_pivot_Y(tetris->boardPiece[tetris->nbBoardPiece -1]);

    for (int i = 0; i < tetris->boardPiece[tetris->nbBoardPiece -1]->num_cells; i++) {
        int oldX = tetris->boardPiece[tetris->nbBoardPiece -1]->coords[i][0];
        int oldY = tetris->boardPiece[tetris->nbBoardPiece -1]->coords[i][1];
        // Mise à jour des coordonnées après la rotation à gauche
        tetris->boardPiece[tetris->nbBoardPiece -1]->coords[i][0] = pivotX + (oldY - pivotY);
        tetris->boardPiece[tetris->nbBoardPiece -1]->coords[i][1] = pivotY - (oldX - pivotX);
    }
}

void refresh_board(Tetris* tetris){
    //On efface le tableau
    for (int i = 0; i < tetris->ligne; i++) {
        for (int j = 0; j < tetris->colonne; j++) {
            tetris->board[i][j].isFull = false;
            tetris->board[i][j].c = NOTHING; // pas oublier NOTHING
        }
    }

    for( int i = 0 ; i < tetris->nbBoardPiece ; i ++){
        PieceConfig* p = tetris->boardPiece[i];
        for (int ind = 0; ind < p->num_cells; ind++) {
            int coord_x = p->coords[ind][0];
            int coord_y = p->coords[ind][1];
            // Vérifier que les coordonnées sont valides ( au cas où ptet faire une fonction verifie vive le refactoring)
            if (coord_x >= 0 && coord_x < tetris->ligne && coord_y >= 0 && coord_y < tetris->colonne) {
                tetris->board[coord_x][coord_y].isFull = true;
                tetris->board[coord_x][coord_y].c = p->c;
            }
        }
    }
}

void clear_board(Tetris *tetris){
    if(!tetris->board) return;
    for(int i = 0; i < tetris->ligne; i++) {
        free(tetris->board[i]);
    }
    free(tetris->board);
}

void clear_boardPiece(Tetris* tetris){
    if(!tetris->boardPiece) return;
    for (int i=0; i<tetris->nbBoardPiece; i++) {
        free(tetris->boardPiece[i]);
    }
    free(tetris->boardPiece);
}

void clear_tmpPiece(Tetris *tetris){
    if(!tetris->tmpPiece) return;
    for (int i=0; i<7; i++) {
        free(tetris->tmpPiece[i]);
    }
    free(tetris->tmpPiece);
}

void clear_pointeur_fct(userInterface ui){
    free(ui.fonctions);
}

void clear_all(Tetris* t, userInterface ui){
    clear_board(t);
    clear_boardPiece(t);
    clear_tmpPiece(t);
    clear_pointeur_fct(ui);
}