#pragma once

#include "userInterface.h"

typedef struct{
    unsigned i : 12; //Coordonnée x de la cellule
    unsigned j : 12; //Coordonnée y de la cellule
    bool isFull; //Boolean si la cell est occupé par une piece
    color c; //Couleur de la cellule
}cell;

typedef struct{
    char nom; // Nom : I , O , ...
    int num_cells; //Nombre de cellule ( pour plus tard )
    int coords[4][2]; //coordonnées piece de base
    color c; //couleur
}PieceConfig;

//Structure qui initialise le tetris
typedef struct{
    cell** board; //Notre jeu
}Tetris;

enum color{NOTHING, CYAN, YELLOW, PURPLE, ORANGE, BLUE, RED, GREEN};
typedef enum color color;

const char HEIGHT = 20;
const char WIDTH = 10;

PieceConfig pieces[7] = {
    {'I', 4, {{0, 3}, {0, 4}, {0, 5}, {0, 6}}, RED},
    {'O', 4, {{0, 4}, {0, 5}, {1, 4}, {1, 5}}, ORANGE},
    {'T', 4, {{0, 3}, {0, 4}, {0, 5}, {1, 4}}, GREEN},
    {'L', 4, {{0, 3}, {0, 4}, {0, 5}, {1, 5}}, CYAN},
    {'J', 4, {{0, 3}, {0, 4}, {0, 5}, {1, 3}}, YELLOW},
    {'Z', 4, {{0, 4}, {0, 5}, {1, 5}, {1, 6}}, BLUE},
    {'S', 4, {{0, 4}, {0, 5}, {1, 3}, {1, 4}}, PURPLE}
};

Tetris *tetris_init_();
void tetris_playGame(Tetris*, userInterface);
cell** init_board();
void display_board(cell**);
PieceConfig** init_tmpPiece();
PieceConfig* get_piece(PieceConfig**);
void set_piece(PieceConfig*, PieceConfig***, cell**, int*);
bool canMove(cell**, PieceConfig*, int, int);
void moveDownPiece(cell**, PieceConfig *);
void moveLeftPiece(cell**, PieceConfig *);
void moveRightPiece(cell**, PieceConfig *);
int getPivotX(PieceConfig*);
int getPivotY(PieceConfig*);
bool canRotate(cell**, PieceConfig*, int);
void rotateLeft(cell**, PieceConfig*);
void rotateRight(cell**, PieceConfig*);
void refresh_board(cell**, PieceConfig**, int);
void clear_board(cell**);
void clear_boardPiece(PieceConfig**, int);
void clear_tmpPiece(PieceConfig**);