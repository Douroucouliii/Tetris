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
    PieceConfig** tmpPiece; //tableau des structures de toutes les pièces possibles
    PieceConfig** boardPiece; //Tableau des pieces qui sont déjà dans notre jeu
    int nbBoardPiece; //Le nombre de pieces dans notre boardPiece
}Tetris;

extern const char HEIGHT;
extern const char WIDTH;
extern PieceConfig pieces[7];

Tetris *tetris_init_();
void tetris_playGame(Tetris*, userInterface);
char demander_caractere();
cell** init_board();
PieceConfig** init_tmpPiece();
void get_piece(Tetris*);
bool can_move(PieceConfig*, int, int);
bool move_down_piece(Tetris*);
bool move_left_piece(Tetris*);
bool move_right_piece(Tetris*);
int get_pivot_X(PieceConfig*);
int get_pivot_y(PieceConfig*);
bool can_rotate(cell**, PieceConfig*, int);
void rotate_left(Tetris*);
void rotate_right(Tetris*);
void refresh_board(cell**, PieceConfig**, int);
void clear_board(cell**);
void clear_boardPiece(PieceConfig**, int);
void clear_tmpPiece(PieceConfig**);
void clear_pointeur_fct(userInterface);
void clear_all(cell**, PieceConfig**, PieceConfig**, int, userInterface);