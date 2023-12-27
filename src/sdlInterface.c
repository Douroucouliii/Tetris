#include "modele.h"
#include "sdlInterface.h"
#include "SDL2/SDL.h"
#include <stdio.h>
#include <stdlib.h>
// Taille des cases
#define CELL_SIZE 50

// Par défaut
unsigned SCREEN_WIDTH = 800;
unsigned SCREEN_HEIGHT = 600;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void close_SDL()
{
    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
    }
    SDL_Quit();
}

void init_SDL()
{
    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "ERREUR SDL_Init : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // On récupère la taille de l'écran du joueur

    SDL_DisplayMode mode;
    if (SDL_GetDesktopDisplayMode(0, &mode) != 0)
    {
        printf("Erreur lors de la récupération de la taille de l'écran : %s", SDL_GetError());
        close_SDL();
    }
    SCREEN_HEIGHT = mode.h;
    SCREEN_WIDTH = mode.w;

    // On crée notre fenêtre SDL
    window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        close_SDL();
    }

    // On crée le renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        close_SDL();
    }

    // SDL_SetwindowIcon() //c'est là ou on doit mettre l'icone
}

SDL_Surface *resizeSurface(SDL_Surface *originalSurface, int newWidth, int newHeight)
{
    if (!originalSurface)
    {
        fprintf(stderr, "Erreur : surface d'origine non valide\n");
        return NULL;
    }

    // Crée une nouvelle surface avec la nouvelle taille
    SDL_Surface *newSurface = SDL_CreateRGBSurface(0, newWidth, newHeight, originalSurface->format->BitsPerPixel,
                                                   originalSurface->format->Rmask, originalSurface->format->Gmask,
                                                   originalSurface->format->Bmask, originalSurface->format->Amask);
    if (!newSurface)
    {
        fprintf(stderr, "Erreur lors de la création de la nouvelle surface : %s\n", SDL_GetError());
        return NULL;
    }

    // Redimensionne l'image
    if (SDL_BlitScaled(originalSurface, NULL, newSurface, NULL) != 0)
    {
        fprintf(stderr, "Erreur lors du redimensionnement de l'image : %s\n", SDL_GetError());
        SDL_FreeSurface(newSurface);
        return NULL;
    }

    return newSurface;
}

char *ColorToString(color color)
{
    switch (color)
    {
    case NOTHING:
        return "NOTHING";
    case BLUE:
        return "BLUE";
    case PURPLE:
        return "PURPLE";
    case RED:
        return "RED";
    case YELLOW:
        return "YELLOW";
    case GREEN:
        return "GREEN";
    case CYAN:
        return "CYAN";
    case ORANGE:
        return "ORANGE";
    default:
        return "UNKNOWN";
    }
}

void display_SDL(Tetris *tetris)
{
    if (tetris == NULL)
    {
        fprintf(stderr, "Erreur : pointeur Tetris NULL dans display_sdl");
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 127, 255);
    SDL_RenderClear(renderer);

    for (int i = 0; i < tetris->line; i++)
    {
        for (int j = 0; j < tetris->column; j++)
        {
            SDL_Rect rect = {(SCREEN_WIDTH - tetris->column * CELL_SIZE) / 2 + j * CELL_SIZE,
                             (SCREEN_HEIGHT - tetris->line * CELL_SIZE) / 2 + i * CELL_SIZE,
                             CELL_SIZE, CELL_SIZE};

            size_t imagePathSize = strlen("images/") + strlen(ColorToString(tetris->board[i][j].c)) + strlen(".bmp") + 1;
            char *imagePath = (char *)malloc(imagePathSize);

            if (imagePath == NULL)
            {
                fprintf(stderr, "Erreur d'allocation mémoire pour le chemin de l'image\n");
                free(imagePath);
                close_SDL();
            }

            strcpy(imagePath, "images/");
            strcat(imagePath, ColorToString(tetris->board[i][j].c)); // On récupère NOTHING ( BLUE, YELLOW , ect)
            strcat(imagePath, ".bmp");

            SDL_Surface *image = SDL_LoadBMP(imagePath);
            if (!image)
            {
                fprintf(stderr, "Erreur : image non trouvé : %s \n", SDL_GetError());
                close_SDL();
            }

            SDL_Surface *resizedSurface = resizeSurface(image, CELL_SIZE, CELL_SIZE);
            if (!resizedSurface)
            {
                SDL_FreeSurface(image);
                close_SDL();
            }

            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, resizedSurface);
            SDL_FreeSurface(image);
            SDL_FreeSurface(resizedSurface);
            if (!texture)
            {
                SDL_DestroyTexture(texture);
                close_SDL();
            }

            if (SDL_RenderCopy(renderer, texture, NULL, &rect) != 0)
            {
                close_SDL();
            }
            free(imagePath);
            SDL_DestroyTexture(texture);
        }
    }
    SDL_RenderPresent(renderer);
}

char input_SDL(Tetris *tetris)
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            close_SDL();
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_q:
                return 'q';
            case SDLK_d:
                return 'd';
            case SDLK_s:
                return 's';
            case SDLK_a:
                return 'a';
            case SDLK_e:
                return 'e';
            case SDLK_0:
                return '0';
            case SDLK_1:
                return '1';
            case SDLK_2:
                return '2';
            case SDLK_3:
                return '3';
            case SDLK_4:
                return '4';
            case SDLK_5:
                return '5';
            case SDLK_6:
                return '6';
            case SDLK_7:
                return '7';
            case SDLK_8:
                return '8';
            case SDLK_9:
                return '9';
            }
            break;
        }
    }

    return ' ';
}