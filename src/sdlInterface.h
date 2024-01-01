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

void init_SDL();

void init_img_textures();

void init_music_sound();

void set_icon();

SDL_Texture *get_background(char *);

int get_indice_by_color(color);

char *color_to_string(color);

char *get_image_path(char *);

void display_image_button(Button *);

void display_button(Button *);

SDL_Texture *display_title(char *, SDL_Color);

void create_level_buttons(Button *, int, int, int, int, int);

void display_txt(char *, SDL_Rect, SDL_Color);

void display_next_piece(Tetris *);

void display_stat_piece(Tetris *, SDL_Rect, SDL_Color);

char input_SDL(Tetris *);

void display_SDL(Tetris *);

void display_info_SDL(Tetris *);

int home_page_events(Button *, Button *, Button *, Tetris *);

int level_selection_events(int *, Button *, Button[], int);

void level_selection_SDL(Tetris *);

void home_page_SDL(Tetris *);

void display_highscores(Tetris *);

void save_highscores(Tetris *, int);

void update_highscores(Tetris *, char *);

void end_screen_SDL(Tetris *);

void play_sound_SDL(int);

void clear_img_textures();

void free_levels(Button[], int);

void close_SDL();