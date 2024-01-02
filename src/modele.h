#pragma once

/**
 * @file modele.h
 * @author Perin Clément (https://github.com/Douroucouliii)
 * @author Mackowiak Carole (https://github.com/Carole-prog)
 * @brief Squelette du projet Tetris
 * @date 2023-11-24
 */

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
 * @brief Une enumeration de differentes etats pour notre jeu Tetris.
 *
 */
enum state
{
    MENU,    /**< Etat du menu principal */
    GAME,    /**< Etat du jeu */
    END,     /**< Etat de fin de jeu */
    CLOSE,   /**< Etat de fermeture du jeu */
    RESTART, /**<ETat de relancement de jeu*/
};
typedef enum state state;

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
 * @brief Strucutre pour avoir un tableau des hightscores pour notre Tetris
 *
 */
typedef struct
{
    char *name; /**< Indique le nom du joueur */
    int score;  /**< Indique le score du joueur*/
} Highscore;

/**
 * @brief Structure représentant le jeu : Tetris
 *
 */
typedef struct Tetris
{
    unsigned line : 5;        /**< Valeur par defaut pour les lignes de notre plateau   */
    unsigned column : 5;      /**< Valeur par defaut pour les colonnes de notre plateau */
    cell **board;             /**< Tableau representant notre plateau de jeu Tetris     */
    PieceConfig **tmpPiece;   /**< Tableau des structures de toutes les pièces possibles pour notre Tetris*/
    PieceConfig **boardPiece; /**< Tableau des pièces déjà présente dans le Tetris      */
    int nbBoardPiece;         /**< Le nombre de pièces dans le tableau boardPiece       */
    state state;              /**< L'état actuel du jeu ( HOME, GAME, END, OPTION )     */
    int nbLines;              /**< Le nombre de ligne supprimées                        */
    int score;                /**< Le score du jeu ( On utilise le système de score de la NES )*/
    int level;                /**< Le niveau actuel du jeu ( Cela affecte la vitesse de déplacement des pièces )*/
    int line_until_first_level_up;/**< Le nombre de ligne avant de passer au niveau suivant (premiere fois, ensuite, 10lignes/level) */
    PieceConfig *nextPiece;   /**< La prochaine pièce qui va arriver sur le plateau     */
    int pieceStats[7];        /**< Tableau des statistiques de pièces                   */
    FILE *file;               /**< Fichier de sauvegarde des highscores                 */
    Highscore *highscores;    /**< Tableau des highscores                               */
    bool isPanic;             /**< Indique si le joueur est en mode panic (proche de la mort)*/
} Tetris;

/**
 * @brief Tableau statique des pièces de notre jeu.
 *
 */
extern PieceConfig pieces[7];

/**
 * @brief Fonction qui initialise notre jeu Tetris et qui retourne un pointeur de notre jeu
 *
 * @return Tetris*
 */
Tetris *tetris_init_();

/**
 * @brief Fonction qui intialise notre tableau de cellule
 *
 * @param tetris
 */
void init_board(Tetris *);

/**
 * @brief Fonction qui initialise le tableau de pointeur de nos pieces
 *
 * @param tetris
 */
void init_tmpPiece(Tetris *);

/**
 * @brief Fonction qui initialise le tableau de highscores par rapport à tetris->file.
 * @param tetris
 */
void init_highscore(Tetris *);

/**
 * @brief Fonction qui permet au joueur de jouer au Tetris avec l'interface voulu ( NCurses ou SDL )
 *
 * @param tetris
 * @param nCurse
 * @param SDL
 */
void tetris_playGame(Tetris *, userInterface, userInterface);

/**
 * @brief Fonction qui gère le menu du jeu Tetris
 * @param tetris
 * @param userInterface
 */
void homescreen(Tetris *, userInterface);

/**
 * @brief Fonction qui gère le menu du jeu Tetris
 * @param tetris
 * @param userInterface
 */
void game(Tetris *, userInterface, userInterface, userInterface);

/**
 * @brief
 *
 */
void restart_game(Tetris *, userInterface);

/**
 * @brief Fonction qui gère le menu du jeu Tetris
 * @param tetris
 * @param userInterface
 */
void endscreen(Tetris *, userInterface);

/**
 * @brief Fonction qui donne la prochaine pièce qui va apparaître sur le plateau.
 *
 * @param tetris
 * @return PieceConfig*
 */
PieceConfig *get_next_piece(Tetris *);

/**
 * @brief Fonction qui met à jour les statistiques de nos pieces
 *
 * @param tetris
 * @param name
 */
void update_stats(Tetris *, char);

/**
 * @brief Fonction qui met à jour le plateau de jeu avec la piece actuelle en paramètre.
 *
 * @param tetris
 * @param piece
 */
void update_piece(Tetris *, PieceConfig *);

/**
 * @brief Fonction qui donne la piece qui va etre placer sur le Tetris et qui remet à jour la prochaine piece qui va apparaitre
 *
 * @param tetris
 */
void get_piece(Tetris *);

/**
 * @brief Fonction qui permet de restauré notre plateau
 * @param tetris
 * @param temp_cells[][4][2]
 */
void restore_board_state(Tetris *tetris, bool temp_cells[][4][2]);

bool is_same_as_old_coords(Tetris *tetris, int x, int y, int oldX, int oldY);

/**
 * @brief Fonction qui retourne vrai ou faux, si la pièce peut bouger grâce aux variables Varx et VarY qui gère l'orientation ( Bas, Gauche, Droite, Haut )
 *
 * @param tetris
 * @param VarX  ( Valeurs entre [-1,1] )
 * @param VarY ( Valeurs entre [-1,1] )
 * @return true ; si la piece peut bouger
 * @return false ; si la piece ne peut pas bouger
 */
bool can_move(Tetris *, int, int);

/**
 * @brief Fonction qui retourne vrai ou faux si l'on peut bouger la pièce vers le bas
 *
 * @param tetris
 *
 * @return true
 * @return false
 */
bool move_down_piece(Tetris *);

/**
 * @brief Fonction qui retourne vrai ou faux si l'on peut bouger la pièce vers la gauche
 *
 * @param tetris
 *
 * @return true
 * @return false
 */
bool move_left_piece(Tetris *);

/**
 * @brief Fonction qui retourne vrai ou faux, si l'on peut bouger la piece vers la droite
 *
 * @param tetris
 *
 * @return true
 * @return false
 */
bool move_right_piece(Tetris *);

/**
 * @brief Fonction qui permet de savoir qu'elle pivotX avoir pour tourner notre piece et retourne -1 si il ne trouve pas de qu'elle piece, on parle.
 *
 * @param piece
 *
 * @return int
 */
int get_pivot_X(PieceConfig *);

/**
 * @brief Fonction qui permet de savoir qu'elle pivotY avoir pour tourner notre piece et retourne -1 si il ne trouve pas de qu'elle piece, on parle.
 *
 * @param piece
 *
 * @return int
 */
int get_pivot_y(PieceConfig *);

/**
 * @brief Fonction qui retourne vrai ou faux pour savoir si l'on peut tourner notre piece dans un sens ( 1 pour tourner à droite et -1 pour tourner à gauche ).
 *
 * @param tetris
 * @param rotationDirection
 *
 * @return true
 * @return false
 */
bool can_rotate(Tetris *, int);

/**
 * @brief Fonction qui permet de tourner la piece à gauche et change les coordonnées de notre pièce par rapport à son pivot.
 * @param tetris
 */
void rotate_left(Tetris *);

/**
 * @brief Fonction qui permet de tourner la piece à droite et change les coordonnées de notre piece par rapport à son pivot.
 * @param tetris
 */
void rotate_right(Tetris *);

/**
 * @brief Fonction qui permet de raffraichir notre affichage sur la console
 * @param tetris
 */
void refresh_board(Tetris *);

/**
 * @brief Fonction qui nous indique si la ligne de notre tableau est complète
 * @param tetris
 * @param ligne ( indice )
 * @return true
 * @return false
 */
bool is_full_line(Tetris *, int);

/**
 * @brief Fonction qui enlève la ligne complète et descend toutes les autres (en mettant à jours leurs coordonées ).
 * @param tetris
 * @param ligne ( indice )
 */
void delete_line(Tetris *, int);

/**
 * @brief Fonction qui enlève toutes les lignes complète de notre jeu Tetris et met à jour le score du joueur et son passage de level en level
 * @param tetris
 * @param ui
 */
void delete_all_line(Tetris *, userInterface);

/**
 * @brief Modifie la couleur de toutes les pieces aléatoirement (level up)
 * @param tetris
 */
void switch_color(Tetris *);

/**
 * @brief Fonction qui met à jour le score du joueur.
 * @param tetris
 * @param score_line
 */
void add_score(Tetris *, int);

/**
 * @brief Fonction qui fait un sleep avant d'apparaitre la prochaine piece : "time until piece apparition: 10 frames for the bottom two rows, +2 frames for each group of 4 rows above that"
 * @param tetris
 */
int frame_sleep_NES(Tetris *);

/**
 * @brief Fonction qui met à jour la variable isPanic de notre jeu Tetris en fonction de si le joueur est proche de la mort ou non
 * @param tetris
 */
void is_panic(Tetris *);

/**
 * @brief Fonction qui retourne le nombre de frame avant de descendre la pièce
 * @param tetris
 * @return int : nombre de frames
 */
int delay(Tetris *);

/**
 * @brief Fonction qui calcule le nombre de ligne avant de passer au niveau suivant (premiere fois, ensuite, 10lignes/level)
 * @param tetris 
 */
void line_until_first_level_up(Tetris *);

/**
 * @brief Fonction qui permet de liberer l'espace alloué à notre plateau de jeu
 * @param tetris
 */
void clear_board(Tetris *);

/**
 * @brief Fonction qui permet de liberer l'espace alloué à notre tableau de piece
 * @param tetris
 */
void clear_boardPiece(Tetris *);

/**
 * @brief Libère la mémoire allouée pour le tableau des configurations temporaires de pièces Tetris
 * @param tetris
 */
void clear_tmpPiece(Tetris *);

/**
 * @brief Libère la mémoire allouée pour le tableau des highscores.
 * @param tetris
 */
void clear_highscores(Tetris *);

/**
 * @brief  Libère la mémoire allouée pour le tableau de pointeurs de fonctions d'interface utilisateur
 * @param userInterface
 */
void clear_pointeur_fct(userInterface);

/**
 * @brief Libère la mémoire allouée pour toutes la structure du jeu Tetris
 * @param tetris
 * @param userInterface
 */
void clear_tetris(Tetris *, userInterface);