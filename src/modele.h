#pragma once

#include "userInterface.h"

#include <stdbool.h>

enum color
{
    NOTHING,
    CYAN,
    YELLOW,
    PURPLE,
    ORANGE,
    BLUE,
    RED,
    GREEN
};
typedef enum color color;

typedef struct
{
    bool isFull; // Boolean si la cell est occupé par une piece
    color c;     // Couleur de la cellule
} cell;

typedef struct
{
    char name;        // Nom : I , O , ...
    int num_cells;    // Nombre de cellules
    int coords[4][2]; // coordonnées piece de base
    color c;          // couleur
} PieceConfig;

// Structure qui initialise le tetris
typedef struct Tetris
{
    unsigned line : 5;
    unsigned column : 5;
    cell **board;             // Notre jeu
    PieceConfig **tmpPiece;   // tableau des structures de toutes les pièces possibles
    PieceConfig **boardPiece; // Tableau des pieces qui sont déjà dans notre jeu
    int nbBoardPiece;         // Le nombre de pieces dans notre boardPiece
    bool end;                 // Si on est à la fin de la partie
    int nbLines;              // Le nombre de lignes supprimées
    int score;                // Le score (le score utilisé est le systeme de score NES)
    int level;                // Le niveau actuelle du jeu (vitesse de chute des pieces)
} Tetris;

extern PieceConfig pieces[7];

Tetris *tetris_init_();
void init_board(Tetris *);
void init_tmpPiece(Tetris *);
void tetris_playGame(Tetris *, userInterface);
void get_piece(Tetris *);
bool can_move(Tetris *, int, int);
bool move_down_piece(Tetris *);
bool move_left_piece(Tetris *);
bool move_right_piece(Tetris *);
int get_pivot_X(PieceConfig *);
int get_pivot_y(PieceConfig *);
bool can_rotate(Tetris *, int);
void rotate_left(Tetris *);
void rotate_right(Tetris *);
void refresh_board(Tetris *);
bool is_full_line(Tetris *, int);
void delete_line(Tetris *, int);
void delete_all_line(Tetris *);
void add_score(Tetris *, int);
void clear_board(Tetris *);
void clear_boardPiece(Tetris *);
void clear_tmpPiece(Tetris *);
void clear_pointeur_fct(userInterface);
void clear_all(Tetris *, userInterface);