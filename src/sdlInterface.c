#include "modele.h"
#include "sdlInterface.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
// Taille des cases
#define CELL_SIZE 47

// Par défaut
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;

SDL_Texture *imageTexture[8];
SDL_Texture *backgroundTexture = NULL;
SDL_Surface *backgroundSurface = NULL;

Mix_Music *musics[3];
Mix_Chunk *sounds[10];

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

void cleanBackground()
{
    if (backgroundSurface)
    {
        SDL_FreeSurface(backgroundSurface);
    }
    if (backgroundTexture)
    {
        SDL_DestroyTexture(backgroundTexture);
    }
}

void close_SDL()
{
    for(int i = 0; i < 3; i++)
    {
        if(musics[i])
        {
            Mix_FreeMusic(musics[i]);
        }
    }
    for(int i = 0; i < 9; i++)
    {
        if(sounds[i])
        {
            Mix_FreeChunk(sounds[i]);
        }
    }

    Mix_CloseAudio();
    TTF_CloseFont(font);
    cleanBackground();
    freeImgTextures();

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

char *GetImagePath(char *texte)
{
    size_t imagePathSize = strlen("assets/images/") + strlen(texte) + strlen(".bmp") + 1;

    char *imagePath = (char *)malloc(imagePathSize);
    if (!imagePath)
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
            free(ImagePath);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        int indice = GetIndiceByColor(current);
        imageTexture[indice] = SDL_CreateTextureFromSurface(renderer, image);

        if (!imageTexture[indice])
        {
            fprintf(stderr, "Erreur : texture image non charger : %s \n", SDL_GetError());
            SDL_FreeSurface(image);
            free(ImagePath);
            close_SDL();
            exit(EXIT_FAILURE);
        }

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

void initBackground()
{

    if (!backgroundTexture && !backgroundSurface)
    {
        char *imageBack = GetImagePath("fond");
        backgroundSurface = SDL_LoadBMP(imageBack);
        if (!backgroundSurface)
        {
            fprintf(stderr, "Erreur lors du chargement de l'image de fond : %s \n", SDL_GetError());
            close_SDL();
            exit(EXIT_FAILURE);
        }

        SDL_Surface *resize = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
        SDL_BlitScaled(backgroundSurface, NULL, resize, NULL);

        backgroundTexture = SDL_CreateTextureFromSurface(renderer, resize);
        SDL_FreeSurface(resize);
    }
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
    window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_BORDERLESS);
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
    font = TTF_OpenFont("assets/ttf/calibri.ttf", 42);
    if (font == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture de la police : %s\n", TTF_GetError());
        TTF_Quit();
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // Initialisation de l'audio
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        fprintf(stderr, "Erreur Mix_OpenAudio : %s", SDL_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);

    // Initialiser les musiques et les sons
    initMusicSound();

    // Initialiser le nombre de canaux maximum
    Mix_AllocateChannels(16);

    // Inialisation des textures des tuiles
    initImgTextures();

    // Initialisation de l'image du background
    initBackground();

    // Set une icone
    set_icon();
}

void display_SDL(Tetris *tetris)
{
    if (tetris == NULL)
    {
        fprintf(stderr, "Erreur : Pointeur Tetris NULL dans display_SDL");
        close_SDL();
        exit(EXIT_FAILURE);
    }
    SDL_RenderClear(renderer);

    if (SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL) != 0)
    {
        fprintf(stderr, "Erreur lors du rendu de l'image de fond : %s \n", SDL_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // On met la musique de jeu si elle n'est pas déjà en train de jouer
    if (!Mix_PlayingMusic())
    {
        Mix_PlayMusic(musics[1], -1);
    }

    int offsetX = (SCREEN_WIDTH - tetris->column * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - tetris->line * CELL_SIZE) / 2;

    offsetY -= 30;

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

void AffichageTexte(char *texte, SDL_Rect rect, SDL_Color textColor)
{
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, texte, textColor);
    if (!textSurface)
    {
        fprintf(stderr, "Erreur Surface (Affichage Texte) : %s\n", SDL_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture)
    {
        fprintf(stderr, "Erreur Texture (Affichage Texte) : %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    SDL_Rect textRect = {rect.x + 10, rect.y + 10, textSurface->w, textSurface->h};
    if (SDL_RenderCopy(renderer, textTexture, NULL, &textRect) != 0)
    {
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void AfficheNextPiece(Tetris *tetris)
{
    SDL_Rect nextpiece = {SCREEN_WIDTH - 780, 0, 400, SCREEN_HEIGHT / 4};

    int offsetX = (nextpiece.w - CELL_SIZE) / 2 + 250;
    int offsetY = (nextpiece.h - CELL_SIZE) / 2 + 50;

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
}

void AfficheStatPiece(Tetris *tetris, SDL_Rect rect, SDL_Color textColor)
{
    int offsetY = 10;
    int offsetX = 50;
    int textOffsetX = 150;

    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int cellX = rect.x - offsetX + tetris->tmpPiece[i]->coords[j][1] * (CELL_SIZE / 2);
            int cellY = rect.y + offsetY + tetris->tmpPiece[i]->coords[j][0] * (CELL_SIZE / 2);
            int indice = GetIndiceByColor(tetris->tmpPiece[i]->c);
            SDL_Rect cellRect = {cellX, cellY, CELL_SIZE / 2, CELL_SIZE / 2};

            if (SDL_RenderCopy(renderer, imageTexture[indice], NULL, &cellRect) != 0)
            {
                close_SDL();
                exit(EXIT_FAILURE);
            }
        }

        char numberString[10];
        snprintf(numberString, sizeof(numberString), "%d", tetris->pieceStats[i]);

        SDL_Surface *textSurface = TTF_RenderText_Solid(font, numberString, textColor);
        if (!textSurface)
        {
            fprintf(stderr, "Erreur Surface (Affichage PieceSat) : %s\n", SDL_GetError());
            close_SDL();
            exit(EXIT_FAILURE);
        }

        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture)
        {
            fprintf(stderr, "Erreur Texture (Affichage PieceStats) : %s\n", SDL_GetError());
            SDL_FreeSurface(textSurface);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        int textX = rect.x + offsetX + textOffsetX;
        int textY = rect.y + offsetY;

        SDL_Rect textRect = {textX, textY, textSurface->w, textSurface->h};

        if (SDL_RenderCopy(renderer, textTexture, NULL, &textRect) != 0)
        {
            SDL_DestroyTexture(textTexture);
            SDL_FreeSurface(textSurface);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
        offsetY += 80;
    }
}

void display_info_SDL(Tetris *tetris)
{
    if (tetris == NULL)
    {
        fprintf(stderr, "Erreur : pointeur Tetris NULL dans display_info_SDL");
        close_SDL();
        exit(EXIT_FAILURE);
    }

    SDL_Color textColor = {255, 255, 255};

    // Affiche le titre score
    SDL_Rect scoreRect = {SCREEN_WIDTH - 680, SCREEN_HEIGHT - 335, 200, 50};
    AffichageTexte("Score", scoreRect, textColor);

    // Affiche le score du joueur
    SDL_Rect scoreP = {SCREEN_WIDTH - 650, SCREEN_HEIGHT - 280, 200, 50};

    char ScoreString[10];
    snprintf(ScoreString, sizeof(ScoreString), "%d", tetris->score);
    AffichageTexte(ScoreString, scoreP, textColor);

    // Affiche le titre Stats
    SDL_Rect Stats = {450, 20, 100, 40};
    AffichageTexte("Statistics", Stats, textColor);

    // Affiche les Statistique de chaque pièce
    SDL_Rect PieceStats = {450, 80, 400, (SCREEN_HEIGHT * 2 / 4) + 120};
    AfficheStatPiece(tetris, PieceStats, textColor);

    // Affiche du titre niveau
    SDL_Rect NameLevel = {450, SCREEN_HEIGHT - 340, 100, 40};
    AffichageTexte("Level", NameLevel, textColor);

    // Affichage du Niveau du joueur
    SDL_Rect rectLevel = {550, SCREEN_HEIGHT - 220, 100, 40};
    char levelString[20];
    sprintf(levelString, "%d", tetris->level);

    AffichageTexte(levelString, rectLevel, textColor);

    //  Affiche le nombre de ligne clear par le joueur
    SDL_Rect linesRect = {450, SCREEN_HEIGHT - 150, 100, 40};

    char LineString[20];
    snprintf(LineString, sizeof(LineString), "Line clear: %d", tetris->nbLines);
    AffichageTexte(LineString, linesRect, textColor);

    // Affiche le titre Next
    SDL_Rect nextP = {SCREEN_WIDTH - 680, 20, 100, 40};
    AffichageTexte("Next", nextP, textColor);

    // Affiche la prochaine pièce
    AfficheNextPiece(tetris);

    SDL_RenderPresent(renderer);
}

int delay_SDL(int niveau)
{
    int frames;

    // le nombre de frames/cellules de grille par pièce en fonction du niveau
    switch (niveau)
    {
    case 0:
        frames = 48;
        break;
    case 1:
        frames = 43;
        break;
    case 2:
        frames = 38;
        break;
    case 3:
        frames = 33;
        break;
    case 4:
        frames = 28;
        break;
    case 5:
        frames = 23;
        break;
    case 6:
        frames = 18;
        break;
    case 7:
        frames = 13;
        break;
    case 8:
        frames = 8;
        break;
    case 9:
        frames = 6;
        break;
    case 10:
    case 11:
    case 12:
        frames = 5;
        break;
    case 13:
    case 14:
    case 15:
        frames = 4;
        break;
    case 16:
    case 17:
    case 18:
        frames = 3;
        break;
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
        frames = 2;
        break;
    // Niveau >28 : Killscreen : seul mattéo pourrait survivre ici
    default:
        frames = 1; // C'est la mort
        break;
    }

    // Calculer le délai en fonction du nombre de frames par pièce
    int delai = (frames * 1000) / 60; // Convertir les frames en millisecondes (60 frames par seconde)
    return delai;
}

char input_SDL(Tetris *tetris)
{
    if (tetris == NULL)
    {
        fprintf(stderr, "Erreur : pointeur Tetris NULL dans display_info_SDL");
        close_SDL();
        exit(EXIT_FAILURE);
    }
    SDL_Event event;
    int delayTime = delay_SDL(tetris->level);
    int startTime = SDL_GetTicks();
    int lastDescentTime = startTime;

    // Attendre un événement ou jusqu'à ce que le délai soit écoulé
    while (SDL_PollEvent(&event) || (SDL_GetTicks() - startTime < delayTime))
    {
        SDL_Delay(10);

        if (event.type == SDL_QUIT)
        {
            close_SDL();
            exit(EXIT_SUCCESS);
        }
        else if (event.type == SDL_KEYDOWN)
        {
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
        }
        int currentTime = SDL_GetTicks();
        if (currentTime - lastDescentTime >= delayTime)
        {
            lastDescentTime = currentTime;
            return 's';
        }
    }

    return ' ';
}

void displayImageButton(Button *button)
{
    // Définir le rectangle du bouton
    SDL_Rect buttonRect = {button->rect.x, button->rect.y, button->rect.w, button->rect.h};

    char *imagePath = GetImagePath(button->text);
    // Charger l'image du bouton
    SDL_Surface *buttonSurface = SDL_LoadBMP(imagePath);
    if (!buttonSurface)
    {
        fprintf(stderr, "Erreur : image du bouton non trouvé : %s\n", SDL_GetError());
        free(imagePath);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // Créer la texture du bouton à partir de la surface
    SDL_Texture *buttonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);
    if (!buttonTexture)
    {
        fprintf(stderr, "Erreur lors de la création de la texture du bouton : %s\n", SDL_GetError());
        free(imagePath);
        SDL_FreeSurface(buttonSurface);
        close_SDL();
        exit(EXIT_FAILURE);
    }
    SDL_Color rectColor;
    if (button->selected)
    {
        rectColor.r = 0;
        rectColor.g = 0;
        rectColor.b = 110;
        rectColor.a = 255;
    }
    else
    {
        rectColor.r = 0;
        rectColor.g = 0;
        rectColor.b = 0;
        rectColor.a = 0;
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, rectColor.r, rectColor.g, rectColor.b, rectColor.a);
    SDL_RenderFillRect(renderer, &buttonRect);

    // Rendre le bouton à l'écran
    if (SDL_RenderCopy(renderer, buttonTexture, NULL, &buttonRect) != 0)
    {
        fprintf(stderr, "Erreur Impossible de faire le rendu : %s\n", SDL_GetError());
        free(imagePath);
        SDL_FreeSurface(buttonSurface);
        SDL_DestroyTexture(buttonTexture);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // Libération de la surface et de la texture du bouton
    free(imagePath);
    SDL_FreeSurface(buttonSurface);
    SDL_DestroyTexture(buttonTexture);
}

void displayButton(Button *button, TTF_Font *font)
{
    // Définir la couleur du texte
    SDL_Color textColor = {255, 255, 255};

    // Créer la surface du texte du bouton
    SDL_Surface *buttonTextSurface = TTF_RenderText_Solid(font, button->text, textColor);
    if (!buttonTextSurface)
    {
        fprintf(stderr, "Erreur : texte du bouton non rendu : %s\n", SDL_GetError());
        TTF_CloseFont(font);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // Créer la texture du texte du bouton à partir de la surface
    SDL_Texture *buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
    if (!buttonTextTexture)
    {
        fprintf(stderr, "Erreur lors de la création de la texture du bouton : %s\n", SDL_GetError());
        SDL_FreeSurface(buttonTextSurface);
        TTF_CloseFont(font);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // Dessiner le rectangle du bouton avec la couleur appropriée
    SDL_SetRenderDrawColor(renderer, 0, 0, 110, 255);
    SDL_RenderFillRect(renderer, &button->rect);

    // Rendre le texte du bouton à l'écran
    if (SDL_RenderCopy(renderer, buttonTextTexture, NULL, &button->rect) != 0)
    {
        SDL_FreeSurface(buttonTextSurface);
        SDL_DestroyTexture(buttonTextTexture);
        TTF_CloseFont(font);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // Libérer la surface et la texture du texte du bouton
    SDL_FreeSurface(buttonTextSurface);
    SDL_DestroyTexture(buttonTextTexture);
}

SDL_Texture *initBackgroundMenu()
{
    char *imageBack = GetImagePath("menu");
    SDL_Surface *MenuSurface = SDL_LoadBMP(imageBack);
    if (!MenuSurface)
    {
        fprintf(stderr, "Erreur lors du chargement de l'image du Menu: %s \n", SDL_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }

    SDL_Surface *resize = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    SDL_BlitScaled(MenuSurface, NULL, resize, NULL);

    SDL_Texture *MenuTexture = SDL_CreateTextureFromSurface(renderer, resize);
    if (!MenuTexture)
    {
        fprintf(stderr, "Erreur lors du chargement de la texture du Menu: %s \n", SDL_GetError());
        SDL_FreeSurface(MenuSurface);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    SDL_FreeSurface(resize);
    SDL_FreeSurface(MenuSurface);
    return MenuTexture;
}

int isPointInsideRect(int x, int y, SDL_Rect rect)  
{
    return (x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h);
}

void levelSelection_SDL(Tetris *tetris)
{

    play_sound_SDL(6);

    // Créer le fond d'écran
    SDL_Texture *MenuTexture = initBackgroundMenu();

    // Charger la police pour le titre
    TTF_Font *TextFont = TTF_OpenFont("assets/ttf/Tetris.ttf", 42);
    if (!TextFont)
    {
        fprintf(stderr, "Erreur lors de l'ouverture de la police du titre : %s\n", TTF_GetError());
        TTF_Quit();
        close_SDL();
        exit(EXIT_FAILURE);
    }

    SDL_Color textColor = {255, 255, 255};
    // Titre
    SDL_Surface *titleSurface = TTF_RenderText_Solid(TextFont, "Level Selection", textColor);
    if (!titleSurface)
    {
        fprintf(stderr, "Erreur : texte du titre non rendu : %s\n", SDL_GetError());
        TTF_CloseFont(TextFont);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    SDL_Texture *titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    if (!titleTexture)
    {
        fprintf(stderr, "Erreur lors de la création de la texture du titre : %s\n", SDL_GetError());
        SDL_FreeSurface(titleSurface);
        TTF_CloseFont(TextFont);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    Button backButton = {{20, 20, 100, 50}, "Retour", 0};
    /*
    // Affichage des 19 niveaux
    const int numLevels = 19;
    Button levelButtons[numLevels];
    int buttonWidth = 100;
    int buttonHeight = 50;
    int buttonSpacing = 20;
    int levelsPerRow = 5;

    for (int i = 0; i < numLevels; i++)
    {
        int row = i / levelsPerRow;
        int col = i % levelsPerRow;
        int x = col * (buttonWidth + buttonSpacing) + 50;
        int y = row * (buttonHeight + buttonSpacing) + 150;
        levelButtons[i].rect = (SDL_Rect){x, y, buttonWidth, buttonHeight};
        levelButtons[i].selected = 0;
        int textLength = snprintf(NULL, 0, "%d", i);
        snprintf(levelButtons[i].text, textLength + 1, "%d", i);
    }
    */
    bool run = true;
    while (run)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, MenuTexture, NULL, NULL);

        // Afficher le titre
        SDL_Rect titleRect = {(SCREEN_WIDTH - titleSurface->w) / 2, 50, titleSurface->w, titleSurface->h};
        SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect);

        // Afficher le bouton de retour
        displayButton(&backButton, TextFont);

        // Afficher les boutons de niveau
        /*
        for (int i = 0; i < numLevels; i++)
        {
            displayButton(&levelButtons[i], TextFont);
        }*/

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
                case SDLK_ESCAPE:
                    run = false;
                    tetris->state = MENU;
                    break;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if (isPointInsideRect(event.button.x, event.button.y, backButton.rect))
                    {
                        run = false;
                        tetris->state = MENU;
                    }
                    /*
                    // Vérifier si le clic est sur l'un des boutons de niveau
                    for (int i = 0; i < numLevels; ++i)
                    {
                        if (isPointInsideRect(event.button.x, event.button.y, levelButtons[i].rect))
                        {
                            tetris->level = i;
                            tetris->state = GAME;
                        }
                    }
                    */
                }
                break;
            }
        }

        // Mettre à jour l'affichage
        SDL_RenderPresent(renderer);
    }

    // Libérer les ressources
    SDL_DestroyTexture(MenuTexture);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);
    TTF_CloseFont(TextFont);
}

void home_page_SDL(Tetris *tetris)
{

    tetris->state = MENU;
    SDL_Texture *MenuTexture = initBackgroundMenu();

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, MenuTexture, NULL, NULL);
    // Définir la musique
    if (!Mix_PlayingMusic())
    {
        Mix_PlayMusic(musics[0], -1);
    }

    // Affiche TETRIS
    TTF_Font *tetrisFont = TTF_OpenFont("assets/ttf/Tetris.ttf", 42);
    if (!tetrisFont)
    {
        fprintf(stderr, "Erreur lors de l'ouverture de la nouvelle police : %s\n", TTF_GetError());
        TTF_Quit();
        close_SDL();
        exit(EXIT_FAILURE);
    }

    SDL_Color textColor = {255, 255, 255};
    SDL_Surface *tetrisTextSurface = TTF_RenderText_Solid(tetrisFont, "TETRIS", textColor);
    if (!tetrisTextSurface)
    {
        fprintf(stderr, "Erreur : texte TETRIS non rendu : %s\n", SDL_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }

    SDL_Texture *tetrisTextTexture = SDL_CreateTextureFromSurface(renderer, tetrisTextSurface);
    if (!tetrisTextTexture)
    {
        fprintf(stderr, "Erreur lors de la création de la texture TETRIS : %s\n", SDL_GetError());
        SDL_FreeSurface(tetrisTextSurface);
        close_SDL();
        exit(EXIT_FAILURE);
    }
    // Afficher les boutons
    Button play = {{SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 100, 400, 150}, "JOUER", 1};
    Button options = {{SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 + 100, 400, 150}, "OPTIONS", 0};
    Button exit = {{SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 + 300, 400, 150}, "EXIT", 0};

    bool run = true;
    while (run)
    {
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, MenuTexture, NULL, NULL);

        // Affiche le titre TETRIS
        SDL_Rect tetrisTextRect = {(SCREEN_WIDTH / 2) - 450, SCREEN_HEIGHT / 4 - 200, 1000, 400};
        SDL_RenderCopy(renderer, tetrisTextTexture, NULL, &tetrisTextRect);

        // Afficher les boutons à chaque itération
        displayImageButton(&play);
        displayImageButton(&options);
        displayImageButton(&exit);

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
                        // On va a la selection
                        levelSelection_SDL(tetris);
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
    TTF_CloseFont(tetrisFont);
    // On libère le Menutexture
    SDL_DestroyTexture(MenuTexture);
}

void player_name_screen(SDL_Color textColor, char *playerName)
{
    SDL_Rect textInputRect = {100, 100, 500, 50};
    SDL_StartTextInput();

    int quit = 0;
    SDL_Event event;

    while (!quit)
    {
        // Effacer l'écran
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Gérer les événements
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
                case SDLK_RETURN:
                    if (strcmp(playerName, "") == 0)
                    {
                        continue;
                    }
                    else if (strlen(playerName) > 0)
                    {
                        quit = 1;
                        break;
                    }
                case SDLK_BACKSPACE:
                    if (strlen(playerName) > 0)
                    {
                        playerName[strlen(playerName) - 1] = '\0';
                    }
                    break;
                default:
                    break;
                }
                break;
            case SDL_TEXTINPUT:
                if (strlen(playerName) < 20)
                {
                    strcat(playerName, event.text.text);
                }
                break;
            }
        }

        // Afficher le nom du joueur
        if (strlen(playerName) > 0)
        {
            SDL_Surface *textSurface = TTF_RenderText_Solid(font, playerName, textColor);
            if (!textSurface)
            {
                fprintf(stderr, "Erreur de rendu du texte : %s\n", TTF_GetError());
                close_SDL();
                exit(EXIT_FAILURE);
            }

            SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (!textTexture)
            {
                fprintf(stderr, "Erreur de création de texture : %s\n", SDL_GetError());
                SDL_FreeSurface(textSurface);
                close_SDL();
                exit(EXIT_FAILURE);
            }

            SDL_Rect textRect = {textInputRect.x + 10, textInputRect.y + 10, textSurface->w, textSurface->h};
            if (SDL_RenderCopy(renderer, textTexture, NULL, &textRect) != 0)
            {
                fprintf(stderr, "Erreur de rendu de texture : %s\n", SDL_GetError());
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
                close_SDL();
                exit(EXIT_FAILURE);
            }

            SDL_FreeSurface(textSurface);
            SDL_DestroyTexture(textTexture);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();
}

void end_screen_SDL(Tetris *tetris)
{
    tetris->state = END;

    // Définir la couleur de fond
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // On met la musique de fin si elle n'est pas déjà en train de jouer
    if (!Mix_PlayingMusic())
    {
        Mix_PlayMusic(musics[2], -1);
    }
    SDL_Color textColor = {255, 255, 255};
    char playerName[30];
    player_name_screen(textColor, playerName);
    tetris->state = CLOSE;
}

void play_sound_SDL(int i)
{
    // Jouer le son dans le tableau d'indice i
    if (Mix_PlayChannel(-1, sounds[i], 0) == -1)
    {
        fprintf(stderr, "Erreur lors de la lecture du son : %s\n", Mix_GetError());
    }
}