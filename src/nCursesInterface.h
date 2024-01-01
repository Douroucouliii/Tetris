#pragma once

/**
 * @file nCursesInterface.h
 * @author Perin Clément (https://github.com/Douroucouliii)
 * @author Mackowiak Carole (https://github.com/Carole-prog)
 * @brief Affichage du Tetris en Ncurses
 * @date 2023-11-24
 */

#include "modele.h"

/**
 * @brief
 *
 */
void init_nCurses();

/**
 * @brief
 *
 */
void close_nCurses();

/**
 * @brief
 *
 */
void display_nCurses(Tetris *);

/**
 * @brief
 *
 * @return char
 */
char input_nCurses(Tetris *);

/**
 * @brief
 *
 */
void display_info_nCurses(Tetris *);

/**
 * @brief
 *
 */
void home_page_nCurses(Tetris *);

/**
 * @brief
 *
 */
void end_screen_nCurses(Tetris *);
