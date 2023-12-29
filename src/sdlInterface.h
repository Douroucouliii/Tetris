#pragma once

#include "modele.h"
#include "SDL2/SDL.h"

/**
 * @brief
 *
 */
typedef struct
{
    SDL_Rect rect;
    char *text;
    int selected;
} Button;

/**
 * @brief
 *
 * @return SDL_Surface*
 */
SDL_Surface *resizeSurface(SDL_Surface *, int, int);

/**
 * @brief
 *
 * @return char*
 */
char *ColorToString(color);

/**
 * @brief
 *
 * @return int
 */
int GetIndiceByColor(color);

/**
 * @brief
 *
 */
void freeImgTextures();

/**
 * @brief
 *
 * @return char*
 */
char *GetImagePath(char *);

/**
 * @brief
 *
 */
void initImgTextures();
/**
 * @brief Set the icon object
 *
 */
void set_icon();

/**
 * @brief
 *
 */
void init_SDL();

/**
 * @brief
 *
 */
void close_SDL();

/**
 * @brief
 *
 */
void display_SDL(Tetris *);

/**
 * @brief
 *
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