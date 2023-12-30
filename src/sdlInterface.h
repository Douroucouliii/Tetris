#pragma once

#include "modele.h"
#include "SDL2/SDL.h"

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
char *ColorToString(color);

/**
 * @brief C'est une fonction qui retourne l'indice d'une couleur dans son énumération.
 * @param color -  Une énumération de couleur dans modele.h
 * @return int
 */
int GetIndiceByColor(color);

/**
 * @brief Une fonction qui permet de libérer la mémoire pour le Tableau ImgTextures.
 *
 */
void freeImgTextures();

/**
 * @brief Une fonction qui permet de trouver le chemin ( char* ) dans le dossier images par rapport à son nom.
 * Exemple :  On cherche BLUE qui donne notre tuile Bleu.
 * On met alors en paramètre "BLUE" et celui-ci nous ressort : "assets/images/BLUE.bmp"
 *
 * @return char*
 */
char *GetImagePath(char *);

/**
 * @brief Une fonction qui initialise le tableau ImgTextures qui prend toutes les textures de nos tuiles.
 *
 */
void initImgTextures();

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
void initBackground();

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
 * @return int
 */
int delay_SDL(int);

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
void displayButton(Button *);

/**
 * @brief
 *
 */
void home_page_SDL(Tetris *);

/**
 * @brief
 *
 */
void end_screen_SDL(Tetris *, FILE *);

/**
 * @brief
 *
 */
void play_sound_SDL(int);