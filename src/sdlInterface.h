#pragma once

#include "modele.h"
#include "SDL2/SDL.h"

typedef struct
{
    SDL_Rect rect;
    char *text;
    int selected;
} Button;

void init_SDL();
void close_SDL();
void display_SDL(Tetris *);
char input_SDL(Tetris *);
void home_page_SDL(Tetris *);