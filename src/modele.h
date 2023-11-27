#pragma once

#include "userInterface.h"

#include <stdbool.h>

/**
 * @brief Une enumeration de differentes couleurs pour nos pieces dans le Tetris.
 * 
 */
enum color
{
    NOTHING, /**< Aucune couleur */
    CYAN,    /**< Couleur Cyan   */
    YELLOW,  /**< Couleur Jaune  */
    PURPLE,  /**< Couleur Violet */
    ORANGE,  /**< Couleur Orange */
    BLUE,    /**< Couleur Bleu   */
    RED,     /**< Couleur Rouge  */
    GREEN    /**< Couleur Vert   */
};
typedef enum color color;

/**
 * @brief Structure de nos cellule pour notre Matrice ( Plateau du Tetris )
 * 
 */
typedef struct
{
    bool isFull; /**< Indique si notre cellule contient une pièce ( true ) ou non ( false ) */
    color c;     /**< Permet de savoir la couleur de notre cellule ( elle prend une des variables de notre enumeration color )*/
} cell;

/**
 * @brief Structure pour la configuration de nos pieces pour notre Tetris
 * 
 */
typedef struct
{
    char name;        /**< Indique le nom de notre piece ( 'I', 'O', etc. ) */
    int num_cells;    /**< Indique le nombre de cellules composant notre piece*/
    int coords[4][2]; /**< Les coordonnées de notre pièce de base */
    color c;          /**< Permet de savoir la couleur de notre piece */
} PieceConfig;


/**
 * @brief Structure représentant le jeu : Tetris
 * 
 */
typedef struct Tetris
{
    unsigned line : 5;
    unsigned column : 5;
    cell **board;             /**< Tableau representant notre plateau de jeu Tetris*/
    PieceConfig **tmpPiece;   /**< Tableau des structures de toutes les pièces possibles pour notre Tetris*/
    PieceConfig **boardPiece; /**< Tableau des pièces déjà présente dans le Tetris*/
    int nbBoardPiece;         /**< Le nombre de pièces dans le tableau boardPiece */
    bool end;                 /**< Indique si la partie est terminée */
    int nbLines;              /**< Le nombre de ligne supprimées */
    int score;                /**< Le score du jeu ( On utilise le système de score de la NES )*/
    int level;                /**< Le niveau actuel du jeu ( Cela affecte la vitesse de déplacement des pièces )*/
    PieceConfig* nextPiece;   /**< La prochaine pièce qui va arriver */
} Tetris;

extern PieceConfig pieces[7];

/**
 * @brief 
 * 
 * @return Tetris* 
 */
Tetris *tetris_init_();

/**
 * @brief 
 * 
 */
void init_board(Tetris *);

/**
 * @brief 
 * 
 */
void init_tmpPiece(Tetris *);

/**
 * @brief 
 * 
 */
void tetris_playGame(Tetris *, userInterface);

/**
 * @brief Get the next piece object
 * 
 * @param tetris 
 * @return PieceConfig* 
 */
PieceConfig *get_next_piece(Tetris *);

/**
 * @brief 
 * 
 */
void update_piece(Tetris *,PieceConfig *)

/**
 * @brief Get the piece object
 * 
 */
void get_piece(Tetris *);

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool can_move(Tetris *, int, int);

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool move_down_piece(Tetris *);
/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool move_left_piece(Tetris *);

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool move_right_piece(Tetris *);

/**
 * @brief Get the pivot X object
 * 
 * @return int 
 */
int get_pivot_X(PieceConfig *);

/**
 * @brief Get the pivot y object
 * 
 * @return int 
 */
int get_pivot_y(PieceConfig *);

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool can_rotate(Tetris *, int);

/**
 * @brief 
 * 
 */
void rotate_left(Tetris *);

/**
 * @brief 
 * 
 */
void rotate_right(Tetris *);

/**
 * @brief 
 * 
 */
void refresh_board(Tetris *);

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool is_full_line(Tetris *, int);

/**
 * @brief 
 * 
 */
void delete_line(Tetris *, int);

/**
 * @brief 
 * 
 */
void delete_all_line(Tetris *);

/**
 * @brief 
 * 
 */
void add_score(Tetris *, int);

/**
 * @brief 
 * 
 */
void clear_board(Tetris *);

/**
 * @brief 
 * 
 */
void clear_boardPiece(Tetris *);

/**
 * @brief 
 * 
 */
void clear_tmpPiece(Tetris *);

/**
 * @brief 
 * 
 */
void clear_pointeur_fct(userInterface);

/**
 * @brief 
 * 
 */
void clear_all(Tetris *, userInterface);