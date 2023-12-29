#include "modele.h"
#include "sdlInterface.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// Taille des cases
#define CELL_SIZE 50

// Par défaut
unsigned SCREEN_WIDTH = 800;
unsigned SCREEN_HEIGHT = 600;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font;

SDL_Texture *imageTexture[8];

Mix_Music *musics[3];
Mix_Chunk *sounds[10];

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

int GetIndiceByColor(color Couleur)
{
    for (color current = NOTHING; current <= GREEN; current++)
    {
        if (current == Couleur)
        {
            return current;
        }
    }
    return -1;
}

void freeImgTextures()
{
    for (int i = 0; i < 8; i++)
    {
        SDL_DestroyTexture(imageTexture[i]);
    }
}

void close_SDL()
{

    // On libère la musique de fin
    Mix_FreeMusic(musics[2]);

    freeImgTextures();
    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
    }
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
    }
    // On ferme l'audio
    Mix_CloseAudio();

    SDL_Quit();
}

char *GetImagePath(char *texte)
{
    size_t imagePathSize = strlen("assets/images/") + strlen(texte) + strlen(".bmp") + 1;

    char *imagePath = (char *)malloc(imagePathSize);
    if (imagePath == NULL)
    {
        fprintf(stderr, "Erreur d'allocation mémoire pour le chemin de l'image\n");
        close_SDL();
        exit(EXIT_FAILURE);
    }

    strcpy(imagePath, "assets/images/");
    strcat(imagePath, texte);
    strcat(imagePath, ".bmp");

    return imagePath;
}

void initImgTextures()
{
    for (color current = NOTHING; current <= GREEN; current++)
    {
        char *ImagePath = GetImagePath(ColorToString(current));
        SDL_Surface *image = SDL_LoadBMP(ImagePath);
        if (!image)
        {
            fprintf(stderr, "Erreur : image non trouvé : %s \n", SDL_GetError());
            close_SDL();
            exit(EXIT_FAILURE);
        }

        int indice = GetIndiceByColor(current);
        imageTexture[indice] = SDL_CreateTextureFromSurface(renderer, image);

        SDL_FreeSurface(image);
        free(ImagePath);
    }
}

void set_icon()
{
    SDL_Surface *iconSurface = SDL_LoadBMP("assets/images/icon.bmp");
    if (!iconSurface)
    {
        fprintf(stderr, "Erreur Chargement de l'icône : %s", SDL_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }
    SDL_SetWindowIcon(window, iconSurface);
    SDL_FreeSurface(iconSurface);
}

void initMusicSound()
{
    // Initialiser les musiques
    musics[0] = Mix_LoadMUS("assets/music/menu.mp3");
    musics[1] = Mix_LoadMUS("assets/music/game.mp3");
    musics[2] = Mix_LoadMUS("assets/music/end.mp3");

    // Initialiser les sons
    sounds[0] = Mix_LoadWAV("assets/music/moove.wav");
    sounds[1] = Mix_LoadWAV("assets/music/rotate.wav");
    sounds[2] = Mix_LoadWAV("assets/music/line.wav");
    sounds[3] = Mix_LoadWAV("assets/music/tetris.wav");
    sounds[4] = Mix_LoadWAV("assets/music/levelup.wav");
    sounds[5] = Mix_LoadWAV("assets/music/selection.wav");
    sounds[6] = Mix_LoadWAV("assets/music/load.wav");
    sounds[7] = Mix_LoadWAV("assets/music/fall.wav");
    sounds[8] = Mix_LoadWAV("assets/music/gameover.wav");
}

void init_SDL()
{
    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "ERREUR SDL_Init : %s", SDL_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // On récupère la taille de l'écran du joueur

    SDL_DisplayMode mode;
    if (SDL_GetDesktopDisplayMode(0, &mode) != 0)
    {
        printf("Erreur lors de la récupération de la taille de l'écran : %s", SDL_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }
    SCREEN_HEIGHT = mode.h;
    SCREEN_WIDTH = mode.w;

    // On crée notre fenêtre SDL
    window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // On crée le renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }
    // Initialisation de la Font
    TTF_Init();
    font = TTF_OpenFont("assets/ttf/Tetris.ttf", 52);

    // Initialisation de l'audio
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        fprintf(stderr, "Erreur Mix_OpenAudio : %s", SDL_GetError());
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);
    // Initialiser les musiques et les sons
    initMusicSound();
    //Initialiser le nombre de canaux maximum
    Mix_AllocateChannels(16);

    // Inialisation des textures des tuiles
    initImgTextures();

    // Set une icone
    set_icon();
}

void display_SDL(Tetris *tetris)
{
    if (tetris == NULL)
    {
        fprintf(stderr, "Erreur : pointeur Tetris NULL dans display_sdl");
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // On met la musique de jeu si elle n'est pas déjà en train de jouer
    if (!Mix_PlayingMusic())
    {
        Mix_PlayMusic(musics[1], -1);
    }

    int offsetX = (SCREEN_WIDTH - tetris->column * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - tetris->line * CELL_SIZE) / 2;

    for (int i = 0; i < tetris->line; i++)
    {
        for (int j = 0; j < tetris->column; j++)
        {
            SDL_Rect rect = {(offsetX + j * CELL_SIZE),
                             (offsetY + i * CELL_SIZE),
                             CELL_SIZE, CELL_SIZE};
            int indice = GetIndiceByColor(tetris->board[i][j].c);

            if (SDL_RenderCopy(renderer, imageTexture[indice], NULL, &rect) != 0)
            {
                close_SDL();
                exit(EXIT_FAILURE);
            }
        }
    }
    display_info_SDL(tetris);
    SDL_RenderPresent(renderer);
}

void display_info_SDL(Tetris *tetris)
{
    SDL_Color textColor = {255, 255, 255};
    // Affiche le score
    SDL_Rect scoreRect = {SCREEN_WIDTH - 500, SCREEN_HEIGHT - 150, 200, 50};
    SDL_RenderFillRect(renderer, &scoreRect);

    char scoreString[20];
    snprintf(scoreString, sizeof(scoreString), "Score: %d", tetris->score);

    SDL_Surface *scoreSurface = TTF_RenderText_Solid(font, scoreString, textColor);
    SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

    SDL_Rect scoreTextRect = {scoreRect.x + 10, scoreRect.y + 10, scoreSurface->w, scoreSurface->h};
    SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreTextRect);

    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);

    // Affiche le nombre de ligne
    SDL_Rect linesRect = {SCREEN_WIDTH - 500, SCREEN_HEIGHT - 100, 200, 50};
    SDL_RenderFillRect(renderer, &linesRect);

    char linesString[20];
    snprintf(linesString, sizeof(linesString), "Lines: %d", tetris->nbLines);

    SDL_Surface *linesSurface = TTF_RenderText_Solid(font, linesString, textColor);
    SDL_Texture *linesTexture = SDL_CreateTextureFromSurface(renderer, linesSurface);

    SDL_Rect linesTextRect = {linesRect.x + 10, linesRect.y + 10, linesSurface->w, linesSurface->h};
    SDL_RenderCopy(renderer, linesTexture, NULL, &linesTextRect);

    SDL_FreeSurface(linesSurface);
    SDL_DestroyTexture(linesTexture);

    // Affiche les Statistique de chaque pièce
    SDL_Rect PieceStats = {250, 40, 400, (SCREEN_HEIGHT * 2 / 4) + 120};

    int offsetY = 10;
    int offsetX = 50;
    int textOffsetX = 150;

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int cellX = PieceStats.x - offsetX + tetris->tmpPiece[i]->coords[j][1] * (CELL_SIZE / 2);
            int cellY = PieceStats.y + offsetY + tetris->tmpPiece[i]->coords[j][0] * (CELL_SIZE / 2);
            int indice = GetIndiceByColor(tetris->tmpPiece[i]->c);
            SDL_Rect cellRect = {cellX, cellY, CELL_SIZE / 2, CELL_SIZE / 2};

            if (SDL_RenderCopy(renderer, imageTexture[indice], NULL, &cellRect) != 0)
            {
                close_SDL();
                exit(EXIT_FAILURE);
            }
        }

        int number = tetris->pieceStats[i];

        char numberString[10];
        snprintf(numberString, sizeof(numberString), "%d", number);

        SDL_Surface *textSurface = TTF_RenderText_Solid(font, numberString, textColor);
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

        int textX = PieceStats.x + offsetX + textOffsetX;
        int textY = PieceStats.y + offsetY;

        SDL_Rect textRect = {textX, textY, textSurface->w, textSurface->h};

        if (SDL_RenderCopy(renderer, textTexture, NULL, &textRect) != 0)
        {
            close_SDL();
            exit(EXIT_FAILURE);
        }

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        offsetY += 100;
    }

    // Affiche le niveau
    char levelString[20];
    sprintf(levelString, "Niveau %d", tetris->level);
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, levelString, textColor);
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect textRect = {300, SCREEN_HEIGHT - 150, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);

    // Affiche la prochaine pièce
    SDL_Rect nextpiece = {SCREEN_WIDTH - 600, 40, 400, SCREEN_HEIGHT / 4};

    offsetX = (nextpiece.w - CELL_SIZE) / 2 + 250;
    offsetY = (nextpiece.h - CELL_SIZE) / 2 + 50;

    for (int j = 0; j < 4; j++)
    {
        int cellX = nextpiece.x + offsetX - tetris->nextPiece->coords[j][1] * CELL_SIZE;
        int cellY = nextpiece.y + offsetY - tetris->nextPiece->coords[j][0] * CELL_SIZE;
        int indice = GetIndiceByColor(tetris->nextPiece->c);
        SDL_Rect cellRect = {cellX, cellY, CELL_SIZE, CELL_SIZE};

        if (SDL_RenderCopy(renderer, imageTexture[indice], NULL, &cellRect) != 0)
        {
            close_SDL();
            exit(EXIT_FAILURE);
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
            exit(EXIT_SUCCESS);
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
            }
            break;
        }
    }

    return ' ';
}

void displayButton(Button *button)
{

    // Définir le rectangle du bouton
    SDL_Rect buttonRect = {button->rect.x, button->rect.y, button->rect.w, button->rect.h};

    char *imagePath = GetImagePath(button->text);
    // Charger l'image du bouton
    SDL_Surface *buttonSurface = SDL_LoadBMP(imagePath);
    if (!buttonSurface)
    {
        fprintf(stderr, "Erreur : image du bouton non trouvé : %s\n", SDL_GetError());
        SDL_FreeSurface(buttonSurface);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // Créer la texture du bouton à partir de la surface
    SDL_Texture *buttonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);
    if (!buttonTexture)
    {
        fprintf(stderr, "Erreur lors de la création de la texture du bouton : %s\n", SDL_GetError());
        SDL_FreeSurface(buttonSurface);
        SDL_DestroyTexture(buttonTexture);
        exit(EXIT_FAILURE);
    }

    SDL_Color rectColor = button->selected ? (SDL_Color){255, 0, 0, 255} : (SDL_Color){0, 0, 0, 255};
    SDL_SetRenderDrawColor(renderer, rectColor.r, rectColor.g, rectColor.b, rectColor.a);
    SDL_RenderFillRect(renderer, &buttonRect);

    // Rendre le bouton à l'écran
    if (SDL_RenderCopy(renderer, buttonTexture, NULL, &buttonRect) != 0)
    {
        close_SDL();
        SDL_FreeSurface(buttonSurface);
        SDL_DestroyTexture(buttonTexture);
        exit(EXIT_FAILURE);
    }

    // Libération correcte de la surface et de la texture du bouton
    SDL_FreeSurface(buttonSurface);
    SDL_DestroyTexture(buttonTexture);
    free(imagePath);
}

void home_page_SDL(Tetris *tetris)
{

    tetris->state = MENU;

    // Définir la couleur de fond
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Définir la musique
    Mix_PlayMusic(musics[0], -1);

    // Afficher les boutons
    Button play = {{SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 - 100, 500, 150}, "JOUER", 1};
    Button options = {{SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 + 100, 500, 150}, "OPTIONS", 0};
    Button exit = {{SCREEN_WIDTH / 2 - 250, SCREEN_HEIGHT / 2 + 300, 500, 150}, "EXIT", 0};

    bool run = true;
    while (run)
    {
        SDL_RenderClear(renderer);
        // Afficher les boutons à chaque itération
        displayButton(&play);
        displayButton(&options);
        displayButton(&exit);

        // Gestion des événements
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
                case SDLK_RETURN:
                case SDLK_RETURN2:
                case SDLK_KP_ENTER:
                    run = false;
                    if (play.selected == 1)
                    {
                        tetris->state = GAME;
                    }
                    else if (exit.selected == 1)
                    {
                        tetris->state = CLOSE;
                    }
                    else if (options.selected == 1)
                    {
                        tetris->state = OPTION;
                    }
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    if (play.selected == 1)
                    {
                        options.selected = 1;
                        play.selected = 0;
                    }
                    else if (options.selected == 1)
                    {
                        exit.selected = 1;
                        options.selected = 0;
                    }
                    else if (exit.selected == 1)
                    {
                        play.selected = 1;
                        exit.selected = 0;
                    }
                    break;
                case SDLK_z:
                case SDLK_UP:
                    if (play.selected == 1)
                    {
                        exit.selected = 1;
                        play.selected = 0;
                    }
                    else if (options.selected == 1)
                    {
                        play.selected = 1;
                        options.selected = 0;
                    }
                    else if (exit.selected == 1)
                    {
                        options.selected = 1;
                        exit.selected = 0;
                    }
                    break;
                }
                break;
            }
        }

        // Mettre à jour l'affichage
        SDL_RenderPresent(renderer);
    }

    // On libère les ressources musiques
    Mix_FreeMusic(musics[0]);
}

void end_screen_SDL(Tetris *tetris, FILE *f)
{

    tetris->state = END;

    // Définir la couleur de fond
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // On libère la musique du jeu
    Mix_FreeMusic(musics[1]);

    // On met la musique de fin si elle n'est pas déjà en train de jouer
    if (!Mix_PlayingMusic())
    {
        Mix_PlayMusic(musics[2], -1);
    }

    // sleep 3 secondes
    SDL_Delay(3000);

    tetris->state = CLOSE;
}

void play_sound_SDL(int i){
    // Jouer le son dans le tableau d'indice i
    if(Mix_PlayChannel(-1, sounds[i], 0) == -1) {
        printf("Mix_PlayChannel: %s\n", Mix_GetError());
              
    }
}