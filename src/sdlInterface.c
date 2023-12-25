#include "modele.h"
#include "sdlInterface.h"
#include "SDL2/SDL.h"
#include <stdio.h>

void init_SDL()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        goto Quit;
    }

    window = SDL_CreateWindow("SDL2", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (NULL == window)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        goto Quit;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (NULL == renderer)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        goto Quit;
    }

    SDL_Event event;
    int quit = 0;
    while (!quit)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                quit = 1;
                break;
            case SDL_KEYDOWN:
                printf("Je vais en bas \n");
                break;
            case SDL_KEYUP:
                printf("Je vais en haut \n");
                break;
            case SDL_MOUSEMOTION:
                printf("Je bouge pouloulou \n");
                break;
            default:
                printf("Un event mais on sait pas lequel \n");
                break;
            }
        }
    }

Quit:
    if (NULL != renderer)
        SDL_DestroyRenderer(renderer);
    if (NULL != window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

void display_sdl(Tetris *tetris)
{
    return;
}