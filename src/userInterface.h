#pragma once

/**
 * @file userInterface.h
 * @author Perin Clément (https://github.com/Douroucouliii)
 * @author Mackowiak Carole (https://github.com/Carole-prog)
 * @brief Pointeurs de fonctions pour les deux vues ( SDL et nCurses )
 * @date 2023-11-24
 */

#include <stdio.h>

typedef struct Tetris Tetris;

/**
 * @brief Structure des pointeurs de fonctions
 *
 */
typedef struct
{
    void (*init_interface)(void);   /**< Permet d'initialiser l'interface */
    void (*close_interface)(void);  /**< Permet de fermer l'interface     */
    void (*display)(Tetris *);      /**< Permet d'afficher le jeu tetris */
    char (*input)(Tetris *);        /**< Permet de récupérer un input selon si c'est SDL ou NCurses */
    void (*home_page)(Tetris *);    /**< Permet d'accéder au menu des deux interfaces */
    void (*end_screen)(Tetris *);   /**< Permet d'afficher la fin de partie (highscore, nom, etc) */
    void (*display_info)(Tetris *); /**< Permet d'afficher toutes les informations annexes du jeu tetris (score/niveau etc...)*/
    void (*play_sound)(int);        /**< Permet de jouer un son du tableau de son (seulement pour SDL) */
} Function;

/**
 * @brief Structure qui permet de liée les pointeurs de fonctions avec le nom de l'interface utilisé.
 *
 */
typedef struct
{
    char *instance;      /**< Nom de l'instance ( nCurses ou SDL ) */
    Function *functions; /**< Pointeurs de fonctions de notre interface */
} userInterface;

/**
 * @brief Cette fonction permet d'initialiser les pointeurs de fonctions avec la vue SDL.
 * @param void
 * @return userInterface
 */
userInterface sdlInterface_init_();

/**
 * @brief Cette fonction permet d'initialiser les pointeurs de fonctions avec la vue nCurses.
 * @param void
 * @return userInterface
 */
userInterface nCursesInterface_init_();
