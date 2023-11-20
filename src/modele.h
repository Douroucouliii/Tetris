#pragma once

#include "userInterface.h"

#include <stdbool.h>

enum color{NOTHING, CYAN, YELLOW, PURPLE, ORANGE, BLUE, RED, GREEN};
typedef enum color color;

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
typedef struct Tetris{
    cell** board; //Notre jeu
}Tetris;

extern const char HEIGHT;
extern const char WIDTH;
extern PieceConfig pieces[7];

Tetris *tetris_init_();
void tetris_playGame(Tetris*, userInterface);
char demander_caractere();
cell** init_board();
void display_board(cell**);
PieceConfig** init_tmpPiece();
PieceConfig* get_piece(PieceConfig**);
void set_piece(PieceConfig*, PieceConfig***, cell**, int*);
bool canMove(cell**, PieceConfig*, int, int);
bool moveDownPiece(cell**, PieceConfig *);
bool moveLeftPiece(cell**, PieceConfig *);
bool moveRightPiece(cell**, PieceConfig *);
int getPivotX(PieceConfig*);
int getPivotY(PieceConfig*);
bool canRotate(cell**, PieceConfig*, int);
void rotateLeft(cell**, PieceConfig*);
void rotateRight(cell**, PieceConfig*);
void refresh_board(cell**, PieceConfig**, int);
void clear_board(cell**);
void clear_boardPiece(PieceConfig**, int);
void clear_tmpPiece(PieceConfig**);