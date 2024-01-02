#pragma once

/**
 * @file nCursesInterface.h
 * @author Perin Clément (https://github.com/Douroucouliii)
 * @author Mackowiak Carole (https://github.com/Carole-prog)
 * @brief Affichage du Tetris en Ncurses
 * @date 2023-11-24
 */

#include "tetris.h"

/**
 * @brief Initialise nCurses et les couleurs qu'on va utiliser
 */
void init_nCurses();

/**
 * @brief Ferme nCurses et libère la mémoire
 */
void close_nCurses();

/**
 * @brief Affiche le jeu Tetris en Ncurses
 * @param tetris
 */
void display_nCurses(Tetris *);

/**
 * @brief Prend un input de l'utilisateur en Ncurses
 * @param tetris
 * @return char
 */
char input_nCurses(Tetris *);

/**
 * @brief Affiche les infos de Tetris en Ncurses (next piece, score, level, etc..)
 * @param tetris
 */
void display_info_nCurses(Tetris *);

/**
 * @brief Affiche la page d'accueil du jeu en Ncurses, entrer le niveau pour jouer
 * @param tetris
 */
void home_page_nCurses(Tetris *);

/**
 * @brief Affiche la page de fin en nCurses, avec l=es hisghscores etc
 * @param tetris
 */
void end_screen_nCurses(Tetris *);
