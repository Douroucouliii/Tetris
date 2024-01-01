#pragma once

/**
 * @file sdlInterface.h
 * @author Perin Clément (https://github.com/Douroucouliii)
 * @author Mackowiak Carole (https://github.com/Carole-prog)
 * @brief Affichage du Tetris en SDL
 * @date 2023-11-24
 */

#include "modele.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

/**
 * @brief C'est une structure qui permet d'imiter le comportement d'un bouton.
 *
 */
typedef struct
{
    SDL_Rect rect;
    char *text;
    int selected;
} Button;

/**
 * @brief C'est une fonction qui prend en paramètre une énumération de type color et qui retourne un char* correspondant à cette enum.
 * @param color - Une énumération de couleur dans modele.h
 * @return char*
 */
char *color_to_string(color);

/**
 * @brief C'est une fonction qui retourne l'indice d'une couleur dans son énumération.
 * @param color -  Une énumération de couleur dans modele.h
 * @return int
 */
int get_indice_by_color(color);

/**
 * @brief Une fonction qui permet de libérer la mémoire pour le Tableau ImgTextures.
 *
 */
void clear_img_textures();

/**
 * @brief Une fonction qui permet de trouver le chemin ( char* ) dans le dossier images par rapport à son nom.
 * Exemple :  On cherche BLUE qui donne notre tuile Bleu.
 * On met alors en paramètre "BLUE" et celui-ci nous ressort : "assets/images/BLUE.bmp"
 *
 * @return char*
 */
char *get_image_path(char *);

/**
 * @brief Une fonction qui initialise le tableau ImgTextures qui prend toutes les textures de nos tuiles.
 *
 */
void init_img_textures();

/**
 * @brief Une fonction qui met une icône sur notre fenêtre SDL
 *
 */
void set_icon();

/**
 * @brief Cette fonction permet d'initialiser la plus part de nos besoins ( La fenêtre , ...)
 *
 */
void init_SDL();

/**
 * @brief Cette fonction permet de quitter notre programme SDL
 *
 */
void close_SDL();

/**
 * @brief
 *
 */
void init_background();

/**
 * @brief Cette fonction affiche notre plateau de tetris
 * @param tetris*
 */
void display_SDL(Tetris *);

/**
 * @brief Cette fonction affiche la prochaine pièce, les statistiques des pièces, le score du joueur, les lignes,..
 *@param tetris*
 */
void display_info_SDL(Tetris *);

/**
 * @brief
 *
 * @return char
 */
char input_SDL(Tetris *);

/**
 * @brief
 *
 */
void display_button(Button *);

/**
 * @brief
 *
 */
void display_image_button(Button *);
/**
 * @brief
 *
 */
void home_page_SDL(Tetris *);

/**
 * @brief
 *
 */
void level_selection_SDL(Tetris *);

/**
 * @brief
 *
 */
void end_screen_SDL(Tetris *);

/**
 * @brief
 *
 */
void play_sound_SDL(int);