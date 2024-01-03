#include "tetris.h"
#include "sdlInterface.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_mixer.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CELL_SIZE 47
int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

// Variables Globales

SDL_Window *window = NULL;             // Variable pour récupéré la window pour le close_SDL
SDL_Renderer *renderer = NULL;         // Variable pour récupéré notre renderer
SDL_Texture *backgroundTexture = NULL; // Texture pour notre background (récurrent)

SDL_Texture *imageTexture[8]; // Tableau de textures pour nos tuiles
TTF_Font *ButtonFont = NULL;  // Police de caractères pour les boutons
TTF_Font *font = NULL;        // Police de caractères principale du jeu

bool checksounds = false; // Variable pour savoir si dans les options les sons sont activés ou non
bool checkmusics = false; // Variable pour savoir si dans les options la musique est activé ou non

bool save = false;                    // Variable pour savoir si on a déjà sauvegarder notre score
int volumeLevel = MIX_MAX_VOLUME / 2; // Variable qui met le volume de base du jeu à mis chemin

Mix_Music *musics[4];    // Tableau statique de musique
Mix_Chunk *sounds[10];   // Tableau statique de sons
Mix_Music *currentMusic; // Musique actuellement jouée dans le jeu

// INIT & SET

void init_SDL()
{
    // Initialisation de SDL_VIDEO
    if (0 != SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "ERREUR SDL_Init : %s", SDL_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // On récupère ici la taille de l'écran du joueur

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

    // Initialisation de la font
    TTF_Init();

    // Initialisation de la font des boutons
    ButtonFont = TTF_OpenFont("assets/ttf/Tetris.ttf", 42);
    if (!ButtonFont)
    {
        fprintf(stderr, "Erreur lors de l'ouverture de la police du titre (button font): %s\n", TTF_GetError());
        TTF_CloseFont(ButtonFont);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // Initialisation de la font pour tout le jeu
    font = TTF_OpenFont("assets/ttf/Tetris.ttf", 32);
    if (!font)
    {
        fprintf(stderr, "Erreur lors de l'ouverture de la police du titre (font): %s\n", TTF_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // Initialisation de l'audio
    init_audio_SDL();

    // Initialisation des textures des tuiles
    init_img_textures();

    // Initialisation de background
    backgroundTexture = get_background("fond");

    // Set une icone pour notre jeu
    set_icon();
}

void init_img_textures()
{
    // Pour toutes les énumérations color qu'il y a, il y a une image qui lui est désignée
    for (color current = NOTHING; current <= GREEN; current++)
    {
        // On récupère son chemin (ex : assets/images/BLUE.bmp )
        char *ImagePath = get_image_path(color_to_string(current));

        // On crée notre Surface avec cette image
        SDL_Surface *image = SDL_LoadBMP(ImagePath);
        if (!image)
        {
            fprintf(stderr, "Erreur : image non trouvé : %s \n", SDL_GetError());
            free(ImagePath);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        // On récupère l'indice de cette couleur
        int indice = get_indice_by_color(current);

        // Ensuite on crée notre Texture et on l'attribu dans notre tableau
        imageTexture[indice] = SDL_CreateTextureFromSurface(renderer, image);
        if (!imageTexture[indice])
        {
            fprintf(stderr, "Erreur : texture image non charger : %s \n", SDL_GetError());
            SDL_FreeSurface(image);
            free(ImagePath);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        // On oublie pas de libéré ce que l'on peut
        SDL_FreeSurface(image);
        free(ImagePath);
    }
}

void init_audio_SDL()
{
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        fprintf(stderr, "Erreur Mix_OpenAudio (SDL) : %s", SDL_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }
    Mix_VolumeMusic(volumeLevel);

    init_music_sound();

    Mix_AllocateChannels(32);
}

void init_music_sound()
{
    // Initialise les musiques
    musics[0] = Mix_LoadMUS("assets/music/menu.mp3");
    musics[1] = Mix_LoadMUS("assets/music/game.mp3");
    musics[2] = Mix_LoadMUS("assets/music/end.mp3");
    musics[3] = Mix_LoadMUS("assets/music/fastgame.mp3");

    // Initialise les sons
    sounds[0] = Mix_LoadWAV("assets/music/moove.wav");
    sounds[1] = Mix_LoadWAV("assets/music/rotate.wav");
    sounds[2] = Mix_LoadWAV("assets/music/line.wav");
    sounds[3] = Mix_LoadWAV("assets/music/tetris.wav");
    sounds[4] = Mix_LoadWAV("assets/music/levelup.wav");
    sounds[5] = Mix_LoadWAV("assets/music/selection.wav");
    sounds[6] = Mix_LoadWAV("assets/music/load.wav");
    sounds[7] = Mix_LoadWAV("assets/music/fall.wav");
    sounds[8] = Mix_LoadWAV("assets/music/gameover.wav");
    sounds[9] = Mix_LoadWAV("assets/music/highscore.wav");
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

// GET & UTILS

SDL_Texture *get_background(char *imageName)
{
    // Le nom de notre image de fond ( assets/image/fond.bmp )
    char *imagePath = get_image_path(imageName);

    // On trouve notre surface
    SDL_Surface *imageSurface = SDL_LoadBMP(imagePath);
    if (!imageSurface)
    {
        fprintf(stderr, "Erreur lors du chargement de l'image '%s': %s\n", imageName, SDL_GetError());
        close_SDL();
        free(imagePath);
        exit(EXIT_FAILURE);
    }

    // On redimenssione par rapport à la taille de l'écran
    SDL_Surface *resizedSurface = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32, 0, 0, 0, 0);
    if (!resizedSurface)
    {
        fprintf(stderr, "Erreur lors de la création de la surface redimensionnée pour l'image '%s': %s\n", imageName, SDL_GetError());
        SDL_FreeSurface(imageSurface);
        close_SDL();
        free(imagePath);
        exit(EXIT_FAILURE);
    }

    if (SDL_BlitScaled(imageSurface, NULL, resizedSurface, NULL) != 0)
    {
        fprintf(stderr, "Erreur lors du redimensionnement de l'image '%s': %s\n", imageName, SDL_GetError());
        SDL_FreeSurface(imageSurface);
        SDL_FreeSurface(resizedSurface);
        close_SDL();
        free(imagePath);
        exit(EXIT_FAILURE);
    }

    // On crée la texture par rapport à la surface redimenssionner
    SDL_Texture *imageTexture = SDL_CreateTextureFromSurface(renderer, resizedSurface);
    if (!imageTexture)
    {
        fprintf(stderr, "Erreur lors de la création de la texture pour l'image '%s': %s\n", imageName, SDL_GetError());
        SDL_FreeSurface(imageSurface);
        SDL_FreeSurface(resizedSurface);
        close_SDL();
        free(imagePath);
        exit(EXIT_FAILURE);
    }

    // On libère ce qu'on a utiliser
    SDL_FreeSurface(imageSurface);
    SDL_FreeSurface(resizedSurface);
    free(imagePath);

    return imageTexture;
}

int get_indice_by_color(color Couleur)
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

char *color_to_string(color color)
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

char *get_image_path(char *texte)
{
    // On calcule la taille de la chaîne de caractère pour obtenir le chemin vers l'image
    size_t imagePathSize = strlen("assets/images/") + strlen(texte) + strlen(".bmp") + 1;

    // On alloue la mémoire
    char *imagePath = (char *)malloc(imagePathSize);
    if (!imagePath)
    {
        fprintf(stderr, "Erreur d'allocation mémoire pour le chemin de l'image\n");
        free(imagePath);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // On construit le chemin complet vers l'image
    snprintf(imagePath, imagePathSize, "assets/images/%s.bmp", texte);

    return imagePath;
}

// DISPLAY TOOLS MENU & SELECTIONS

void display_image_button(Button *button)
{
    // Définir le rectangle du bouton
    SDL_Rect buttonRect = {button->rect.x, button->rect.y, button->rect.w, button->rect.h};

    char *imagePath = get_image_path(button->text);
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

    // Pour obtenir l'effet de transparence quand le bouton est selectionné ou non
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

void display_button(Button *button)
{
    // Définir la couleur du texte
    SDL_Color textColor = {255, 255, 255};

    // Créer la surface du texte du bouton
    SDL_Surface *buttonTextSurface = TTF_RenderText_Solid(ButtonFont, button->text, textColor);
    if (!buttonTextSurface)
    {
        fprintf(stderr, "Erreur : texte du bouton non rendu : %s\n", SDL_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // Créer la texture du texte du bouton à partir de la surface
    SDL_Texture *buttonTextTexture = SDL_CreateTextureFromSurface(renderer, buttonTextSurface);
    if (!buttonTextTexture)
    {
        fprintf(stderr, "Erreur lors de la création de la texture du bouton : %s\n", SDL_GetError());
        SDL_FreeSurface(buttonTextSurface);
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
    SDL_RenderFillRect(renderer, &button->rect);

    if (SDL_RenderCopy(renderer, buttonTextTexture, NULL, &button->rect) != 0)
    {
        fprintf(stderr, "Erreur ButtonTexture ( display_button ) : %s \n", SDL_GetError());
        SDL_FreeSurface(buttonTextSurface);
        SDL_DestroyTexture(buttonTextTexture);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // Libérer la surface et la texture du texte du bouton
    SDL_FreeSurface(buttonTextSurface);
    SDL_DestroyTexture(buttonTextTexture);
}

SDL_Texture *display_title(char *texte, SDL_Color textColor)
{
    // Titre Surface
    SDL_Surface *titleSurface = TTF_RenderText_Solid(ButtonFont, texte, textColor);
    if (!titleSurface)
    {
        fprintf(stderr, "Erreur : texte du titre non rendu : %s\n", SDL_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }

    SDL_Texture *titleTexture = SDL_CreateTextureFromSurface(renderer, titleSurface);
    if (!titleTexture)
    {
        fprintf(stderr, "Erreur lors de la création de la texture du titre : %s\n", SDL_GetError());
        SDL_FreeSurface(titleSurface);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    SDL_FreeSurface(titleSurface);

    return titleTexture;
}

void create_level_buttons(Button *levelButtons, int numLevels, int buttonWidth, int buttonHeight, int buttonSpacing, int levelsPerRow)
{
    for (int i = 0; i < numLevels; i++)
    {
        int row = i / levelsPerRow;
        int col = i % levelsPerRow;
        int x = col * (buttonWidth + buttonSpacing) + 450;
        int y = row * (buttonHeight + buttonSpacing) + 250;

        levelButtons[i].rect = (SDL_Rect){x, y, buttonWidth, buttonHeight};
        levelButtons[i].selected = 0;
        int textLength = snprintf(NULL, 0, "%d", i);

        char *texte = (char *)malloc((textLength + 1) * sizeof(char));
        if (texte == NULL)
        {
            fprintf(stderr, "Erreur d'allocation de mémoire.\n");
            exit(EXIT_FAILURE);
        }

        snprintf(texte, textLength + 1, "%d", i);
        levelButtons[i].text = texte;
    }
}

// DISPLAY TOOLS GAME

void display_txt(char *texte, SDL_Rect rect, SDL_Color textColor)
{
    // On crée notre surface ou l'on peut écrire
    SDL_Surface *textSurface = TTF_RenderText_Solid(font, texte, textColor);
    if (!textSurface)
    {
        fprintf(stderr, "Erreur Surface (Affichage Texte) : %s\n", SDL_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // On crée notre texture
    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture)
    {
        fprintf(stderr, "Erreur Texture (Affichage Texte) : %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // Un rectangle qui permet de définir une marge pour le rendu
    SDL_Rect textRect = {rect.x + 10, rect.y + 10, textSurface->w, textSurface->h};

    // On affiche le texte sur le renderer
    if (SDL_RenderCopy(renderer, textTexture, NULL, &textRect) != 0)
    {
        fprintf(stderr, "Erreur de chargement Texture Fonction : Display_txt : %s \n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // On libère nos ressources
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}

void display_next_piece(Tetris *tetris)
{
    SDL_Rect nextpiece = {SCREEN_WIDTH - 780, 0, 400, SCREEN_HEIGHT / 4};

    int offsetX = (nextpiece.w - CELL_SIZE) / 2 + 250;
    int offsetY = (nextpiece.h - CELL_SIZE) / 2 + 50;

    for (int j = 0; j < 4; j++)
    {
        int cellX = nextpiece.x + offsetX - tetris->nextPiece->coords[j][1] * CELL_SIZE;
        int cellY = nextpiece.y + offsetY - tetris->nextPiece->coords[j][0] * CELL_SIZE;
        int indice = get_indice_by_color(tetris->nextPiece->c);
        SDL_Rect cellRect = {cellX, cellY, CELL_SIZE, CELL_SIZE};

        if (SDL_RenderCopy(renderer, imageTexture[indice], NULL, &cellRect) != 0)
        {
            fprintf(stderr, "Erreur imageTexture ( display_next_piece ) : %s \n", SDL_GetError());
            close_SDL();
            exit(EXIT_FAILURE);
        }
    }
}

void display_stat_piece(Tetris *tetris, SDL_Rect rect, SDL_Color textColor)
{
    int offsetY = 10;
    int offsetX = 50;
    int textOffsetX = 150;

    // Parcours de nos 7 pièces du tetris
    for (int i = 0; i < 7; i++)
    {
        // Parcours les 4 cellules de chaque pièce
        for (int j = 0; j < 4; j++)
        {
            // On calcul les coordonnées
            int cellX = rect.x - offsetX + tetris->tmpPiece[i]->coords[j][1] * (CELL_SIZE / 2);
            int cellY = rect.y + offsetY + tetris->tmpPiece[i]->coords[j][0] * (CELL_SIZE / 2);
            int indice = get_indice_by_color(tetris->tmpPiece[i]->c);
            SDL_Rect cellRect = {cellX, cellY, CELL_SIZE / 2, CELL_SIZE / 2};

            // On affiche chaque cellule ( ce qui donne la pièce )
            if (SDL_RenderCopy(renderer, imageTexture[indice], NULL, &cellRect) != 0)
            {
                fprintf(stderr, "Erreur imageTexture ( Affiche PieceStat ) : %s \n", SDL_GetError());
                close_SDL();
                exit(EXIT_FAILURE);
            }
        }

        // On alloue dynamiquement pour la chaîne de caractères
        char *numberString = int_to_str(tetris->pieceStats[i]);

        // On crée la surface
        SDL_Surface *textSurface = TTF_RenderText_Solid(ButtonFont, numberString, textColor);
        if (!textSurface)
        {
            fprintf(stderr, "Erreur Surface (Affichage PieceSat) : %s\n", SDL_GetError());
            free(numberString);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        // On crée la texture
        SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!textTexture)
        {
            fprintf(stderr, "Erreur Texture (Affichage PieceStats) : %s\n", SDL_GetError());
            free(numberString);
            SDL_FreeSurface(textSurface);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        // Afficher le texte à côté de notre pièce
        int textX = rect.x + offsetX + textOffsetX;
        int textY = rect.y + offsetY;

        SDL_Rect textRect = {textX, textY, textSurface->w, textSurface->h};

        if (SDL_RenderCopy(renderer, textTexture, NULL, &textRect) != 0)
        {
            fprintf(stderr, "Erreur Affiche pieceStat : %s \n", SDL_GetError());
            free(numberString);
            SDL_DestroyTexture(textTexture);
            SDL_FreeSurface(textSurface);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        // On libère nos ressources
        free(numberString);
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);

        // On met à jour pour faire le décalage
        offsetY += 80;
    }
}

char *int_to_str(int num)
{
    // Taille maximale d'un int en décimal + 1 pour le caractère nul
    int taille_max = 11;

    // Allouer de la mémoire pour la chaîne de caractères
    char *str = (char *)malloc(taille_max * sizeof(char));

    // Vérifier si l'allocation de mémoire a réussi
    if (str == NULL)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }

    // Utiliser la fonction sprintf pour convertir l'int en char*
    snprintf(str, taille_max, "%d", num);

    return str;
}

char *int_to_str_with_prefix(char *prefix, int num)
{
    // Taille maximale d'un int en décimal + la longueur du préfixe + 1
    int taille_max = 11 + strlen(prefix) + 1;

    // Allouer de la mémoire pour la chaîne de caractères
    char *str = (char *)malloc(taille_max * sizeof(char));

    // Vérifier si l'allocation de mémoire a réussi
    if (str == NULL)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire\n");
        exit(EXIT_FAILURE);
    }

    // Utiliser la fonction snprintf pour concaténer le préfixe et convertir l'int en char*
    snprintf(str, taille_max, "%s%d", prefix, num);

    return str;
}

// EVENTS GAME

char input_SDL(Tetris *tetris)
{
    // Si tetris est null, on sort
    if (tetris == NULL)
    {
        fprintf(stderr, "Erreur : pointeur Tetris NULL dans display_info_SDL ");
        close_SDL();
        exit(EXIT_FAILURE);
    }

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            close_SDL();
            exit(EXIT_SUCCESS);
        }
        else if (event.type == SDL_KEYDOWN)
        {
            switch (event.key.keysym.sym)
            {
                // On va à gauche
            case SDLK_LEFT:
            case SDLK_q:
                return 'q';

                // On va à droite
            case SDLK_RIGHT:
            case SDLK_d:
                return 'd';

                // On descend la pièce
            case SDLK_DOWN:
            case SDLK_s:
                return 's';

                // On fait une rotation gauche
            case SDLK_UP:
            case SDLK_a:
                return 'a';

                // On fait une rotation droite
            case SDLK_e:
                return 'e';

                // On switch de Vue ( Ncurses )
            case SDLK_m:
                return 'm';
            }
        }
    }

    return ' ';
}

// DISPLAY GAME

void display_SDL(Tetris *tetris)
{
    // On vérifie tetris
    if (tetris == NULL)
    {
        fprintf(stderr, "Erreur : Pointeur Tetris NULL dans display_SDL");
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // On efface notre écran
    SDL_RenderClear(renderer);

    // On dessine notre fond
    if (SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL) != 0)
    {
        fprintf(stderr, "Erreur lors du rendu de l'image de fond : %s \n", SDL_GetError());
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // On vérifie si la musique est activé ou non
    if (checkmusics != true)
    { // On met la musique de jeu si elle n'est pas déjà en train de jouer (en fonction du mode panic)
        if (tetris->isPanic)
        {
            if (musics[3] == NULL)
            {
                fprintf(stderr, "Erreur : la musique panic (musics[3]) est NULL.\n");
            }
            else if (currentMusic != musics[3])
            {
                Mix_HaltMusic();
                Mix_PlayMusic(musics[3], -1);
                currentMusic = musics[3];
            }
        }
        else
        {
            if (musics[1] == NULL)
            {
                fprintf(stderr, "Erreur : la musique normale (musics[1]) est NULL.\n");
            }
            else if (currentMusic != musics[1])
            {
                Mix_HaltMusic();
                Mix_PlayMusic(musics[1], -1);
                currentMusic = musics[1];
            }

            // Si la musique n'est pas lancée, on la lance
            if (musics[1] != NULL && !Mix_PlayingMusic())
            {
                Mix_PlayMusic(musics[1], -1);
                currentMusic = musics[1];
            }
        }
    }

    int offsetX = (SCREEN_WIDTH - tetris->column * CELL_SIZE) / 2;
    int offsetY = (SCREEN_HEIGHT - tetris->line * CELL_SIZE) / 2;

    offsetY -= 30;

    // On affiche notre plateau et les pièces qui descendent au fur et à mesure
    for (int i = 0; i < tetris->line; i++)
    {
        for (int j = 0; j < tetris->column; j++)
        {
            SDL_Rect rect = {(offsetX + j * CELL_SIZE),
                             (offsetY + i * CELL_SIZE),
                             CELL_SIZE, CELL_SIZE};
            int indice = get_indice_by_color(tetris->board[i][j].c);

            if (SDL_RenderCopy(renderer, imageTexture[indice], NULL, &rect) != 0)
            {
                fprintf(stderr, "Impossible de charger les textures des tuiles : %s \n", SDL_GetError());
                close_SDL();
                exit(EXIT_FAILURE);
            }
        }
    }
    // On affiche le reste des informations
    display_info_SDL(tetris);

    // On l'affiche sur notre rendu
    SDL_RenderPresent(renderer);
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
    display_txt("Score", scoreRect, textColor);

    // Affiche le score du joueur
    SDL_Rect scoreP = {SCREEN_WIDTH - 650, SCREEN_HEIGHT - 280, 200, 50};
    char *scoreString = int_to_str(tetris->score);
    display_txt(scoreString, scoreP, textColor);
    free(scoreString);

    // Affichage du nom HighScore
    SDL_Rect pseudoTextRect = {SCREEN_WIDTH - 690, SCREEN_HEIGHT - 205, 200, 50};
    display_txt("Highscore", pseudoTextRect, textColor);

    // Affichage du plus haut Score atteint
    SDL_Rect scoreH = {SCREEN_WIDTH - 650, SCREEN_HEIGHT - 150, 200, 50};
    char *highScoreText = int_to_str(tetris->highscores[0].score);
    display_txt(highScoreText, scoreH, textColor);
    free(highScoreText);

    // Affiche le titre Statistics
    SDL_Rect Stats = {450, 20, 100, 40};
    display_txt("Statistics", Stats, textColor);

    // Affiche les Statistique de chaque pièce
    SDL_Rect PieceStats = {450, 80, 400, (SCREEN_HEIGHT * 2 / 4) + 120};
    display_stat_piece(tetris, PieceStats, textColor);

    // Affiche du titre du niveau
    SDL_Rect NameLevel = {450, SCREEN_HEIGHT - 330, 100, 40};
    display_txt("Level", NameLevel, textColor);

    // Affichage du Niveau du joueur
    SDL_Rect rectLevel = {550, SCREEN_HEIGHT - 220, 100, 40};
    char *levelString = int_to_str(tetris->level);
    display_txt(levelString, rectLevel, textColor);
    free(levelString);

    //  Affiche le nombre de ligne clear par le joueur
    SDL_Rect linesRect = {450, SCREEN_HEIGHT - 150, 100, 40};
    char *lineString = int_to_str_with_prefix("Line clear : ", tetris->nbLines);
    display_txt(lineString, linesRect, textColor);
    free(lineString);

    // Affiche le titre Next
    SDL_Rect nextP = {SCREEN_WIDTH - 680, 20, 100, 40};
    display_txt("Next", nextP, textColor);

    // Affiche la prochaine pièce
    display_next_piece(tetris);
}

// EVENTS MENU & SELECTION

int home_page_events(Button *play, Button *settings, Button *exit, Tetris *tetris)
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
            case SDLK_RETURN:
            case SDLK_RETURN2:
            case SDLK_KP_ENTER:
                if (play->selected == 1)
                {
                    level_selection_SDL(tetris);
                }
                else if (exit->selected == 1)
                {
                    tetris->state = CLOSE;
                }
                else if (settings->selected == 1)
                {
                    play_sound_SDL(6);
                    settings_SDL(tetris);
                }
                return 1; // Evènement traités
            case SDLK_s:
            case SDLK_DOWN:
                play_sound_SDL(5);
                if (play->selected == 1)
                {
                    settings->selected = 1;
                    play->selected = 0;
                }
                else if (settings->selected == 1)
                {
                    exit->selected = 1;
                    settings->selected = 0;
                }
                else if (exit->selected == 1)
                {
                    play->selected = 1;
                    exit->selected = 0;
                }
                break;
            case SDLK_z:
            case SDLK_UP:
                play_sound_SDL(5);
                if (play->selected == 1)
                {
                    exit->selected = 1;
                    play->selected = 0;
                }
                else if (settings->selected == 1)
                {
                    play->selected = 1;
                    settings->selected = 0;
                }
                else if (exit->selected == 1)
                {
                    settings->selected = 1;
                    exit->selected = 0;
                }
                break;
            }
            break;
        }
    }
    return 0;
}

int level_selection_events(int *selectedLevel, Button *backButton, Button levelButtons[], int numLevels)
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
            case SDLK_ESCAPE:
                return 1; // Retour au home
            case SDLK_s:
            case SDLK_DOWN:
                if (backButton->selected == 0)
                {
                    play_sound_SDL(5);
                    *selectedLevel = (*selectedLevel + 5) % numLevels;
                }
                break;
            case SDLK_z:
            case SDLK_UP:
                if (backButton->selected == 0)
                {
                    play_sound_SDL(5);
                    *selectedLevel = (*selectedLevel - 5 + numLevels) % numLevels;
                }
                break;
            case SDLK_d:
            case SDLK_RIGHT:
                play_sound_SDL(5);
                if (*selectedLevel == numLevels - 1)
                {
                    backButton->selected = 1;
                    levelButtons[*selectedLevel].selected = 0;
                    *selectedLevel = numLevels;
                }
                else if (*selectedLevel == numLevels)
                {
                    backButton->selected = 0;
                    levelButtons[0].selected = 1;
                    *selectedLevel = 0;
                }
                else
                {
                    backButton->selected = 0;
                    levelButtons[*selectedLevel].selected = 0;
                    *selectedLevel = (*selectedLevel + 1) % numLevels;
                }
                break;
            case SDLK_q:
            case SDLK_LEFT:
                play_sound_SDL(5);
                if (*selectedLevel == numLevels)
                {
                    backButton->selected = 0;
                    levelButtons[numLevels - 1].selected = 1;
                    *selectedLevel = numLevels - 1;
                }
                else if (*selectedLevel == 0)
                {
                    backButton->selected = 1;
                    levelButtons[0].selected = 0;
                    *selectedLevel = numLevels;
                }
                else
                {
                    backButton->selected = 0;
                    levelButtons[*selectedLevel].selected = 0;
                    *selectedLevel = (*selectedLevel - 1 + numLevels) % numLevels;
                }
                break;
            case SDLK_RETURN:
            case SDLK_RETURN2:
            case SDLK_KP_ENTER:
                if (*selectedLevel == numLevels)
                {
                    return 1; // Retour
                }
                else
                {
                    return 2; // Niveau selectionner
                }
                break;
            }
        }
    }
    return 0;
}

int settings_events(Button *backButton, Button *musicButton, Button *soundButton, Button *volumeDownButton, Button *volumeUpButton, int *pourcentVolume)
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
            case SDLK_ESCAPE:
                play_sound_SDL(6);
                return 1;
                break;
            case SDLK_RETURN:
            case SDLK_RETURN2:
            case SDLK_KP_ENTER:
                if (backButton->selected == 1)
                {
                    play_sound_SDL(6);
                    return 1;
                }
                else if (musicButton->selected == 1)
                {
                    play_sound_SDL(5);
                    checkmusics = !checkmusics;
                }
                else if (soundButton->selected == 1)
                {
                    play_sound_SDL(5);
                    checksounds = !checksounds;
                }
                else if (volumeDownButton->selected == 1)
                {
                    play_sound_SDL(5);
                    if (volumeLevel > 0)
                    {
                        volumeLevel -= 1;
                        *pourcentVolume = (volumeLevel * 100) / MIX_MAX_VOLUME;
                        Mix_Volume(-1, volumeLevel);
                        Mix_VolumeMusic(volumeLevel);
                    }
                }
                else if (volumeUpButton->selected == 1)
                {
                    play_sound_SDL(5);
                    if (volumeLevel < MIX_MAX_VOLUME)
                    {
                        volumeLevel += 1;
                        *pourcentVolume = (volumeLevel * 100) / MIX_MAX_VOLUME;

                        Mix_Volume(-1, volumeLevel);
                        Mix_VolumeMusic(volumeLevel);
                    }
                }
                break;
            case SDLK_q:
            case SDLK_LEFT:
                play_sound_SDL(5);
                if (volumeUpButton->selected == 1)
                {
                    volumeDownButton->selected = 1;
                    volumeUpButton->selected = 0;
                }
                else if (volumeDownButton->selected == 1)
                {
                    musicButton->selected = 1;
                    volumeDownButton->selected = 0;
                }
                else if (musicButton->selected == 1)
                {
                    musicButton->selected = 0;
                    soundButton->selected = 1;
                }
                else if (soundButton->selected == 1)
                {
                    soundButton->selected = 0;
                    backButton->selected = 1;
                }
                else if (backButton->selected == 1)
                {
                    backButton->selected = 0;
                    volumeUpButton->selected = 1;
                }
                break;
            case SDLK_z:
            case SDLK_UP:
                play_sound_SDL(5);
                if (backButton->selected == 1)
                {
                    backButton->selected = 0;
                    soundButton->selected = 1;
                }
                else if (soundButton->selected == 1)
                {
                    soundButton->selected = 0;
                    musicButton->selected = 1;
                }
                else if (musicButton->selected == 1)
                {
                    volumeDownButton->selected = 1;
                    musicButton->selected = 0;
                }
                else if (volumeDownButton->selected == 1)
                {
                    volumeDownButton->selected = 0;
                    volumeUpButton->selected = 1;
                }
                else if (volumeUpButton->selected == 1)
                {
                    backButton->selected = 1;
                    volumeUpButton->selected = 0;
                }
                break;
            case SDLK_d:
            case SDLK_RIGHT:
            case SDLK_s:
            case SDLK_DOWN:
                play_sound_SDL(5);
                if (backButton->selected == 1)
                {
                    backButton->selected = 0;
                    volumeDownButton->selected = 1;
                }
                else if (volumeDownButton->selected == 1)
                {
                    volumeUpButton->selected = 1;
                    volumeDownButton->selected = 0;
                }
                else if (volumeUpButton->selected == 1)
                {
                    musicButton->selected = 1;
                    volumeUpButton->selected = 0;
                }
                else if (soundButton->selected == 1)
                {
                    soundButton->selected = 0;
                    backButton->selected = 1;
                }
                else if (musicButton->selected == 1)
                {
                    soundButton->selected = 1;
                    musicButton->selected = 0;
                }
                break;
            }
        }
    }
    return 0;
}

// DISPLAY MENU & SELECTION

void settings_SDL(Tetris *tetris)
{
    // La couleur de notre font
    SDL_Color textColor = {255, 255, 255};

    // On charge la texture de notre fond
    SDL_Texture *MenuTexture = get_background("menu");

    // On crée le rectangle et la texture de notre titre
    SDL_Rect titleRect = {SCREEN_WIDTH / 2 - 800, 50, 600, 200};
    SDL_Texture *TitleTexture = display_title("Settings", textColor);

    // On crée nos 5 boutons
    Button backButton = {{SCREEN_WIDTH - 400, SCREEN_HEIGHT - 200, 300, 100}, "Back", 1};
    Button musicButton = {{SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2, 400, 100}, "Muted Musics", 0};
    Button soundButton = {{SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT / 2 + 100, 400, 100}, "Muted Sounds", 0};

    // On va calculer le pourcentage de volume
    int pourcentVolume = (volumeLevel * 100) / MIX_MAX_VOLUME;

    SDL_Rect volumeRect = {SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 - 75, 100, 100};
    Button volumeDownButton = {{SCREEN_WIDTH / 2 - 300, SCREEN_HEIGHT / 2 - 100, 200, 100}, "<<<", 0};
    Button volumeUpButton = {{SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT / 2 - 100, 200, 100}, ">>>", 0};

    int indice = 0;

    bool run = true;
    while (run)
    {
        SDL_RenderClear(renderer);
        // On affiche notre fond sur le rendu
        if (SDL_RenderCopy(renderer, MenuTexture, NULL, NULL) != 0)
        {
            fprintf(stderr, "Impossible de mettre le fond ( SETTINGS ) : %s \n", SDL_GetError());
            SDL_DestroyTexture(MenuTexture);
            SDL_DestroyTexture(TitleTexture);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        // On affiche notre titre sur le rendu
        if (SDL_RenderCopy(renderer, TitleTexture, NULL, &titleRect) != 0)
        {
            fprintf(stderr, "Impossible de mettre le titre ( SETTINGS ) : %s \n", SDL_GetError());
            SDL_DestroyTexture(MenuTexture);
            SDL_DestroyTexture(TitleTexture);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        // On affiche le pourcentage de volume entre les flèches
        char *volumeText = int_to_str(pourcentVolume);
        display_txt(volumeText, volumeRect, textColor);
        free(volumeText);

        // On affiche nos 5 boutons
        display_button(&backButton);
        display_button(&musicButton);
        display_button(&soundButton);
        display_button(&volumeDownButton);
        display_button(&volumeUpButton);

        // Gestion du bouton check ( Mute la musique )
        if (checkmusics)
        {
            indice = get_indice_by_color(GREEN);
            Mix_PauseMusic();
        }
        else
        {
            indice = get_indice_by_color(NOTHING);
            Mix_ResumeMusic();
        }

        // On affiche le rendu si c'est selectionner ou non
        SDL_Rect imageRect = {musicButton.rect.x + musicButton.rect.w + 100, musicButton.rect.y, 100, 100};
        if (SDL_RenderCopy(renderer, imageTexture[indice], NULL, &imageRect) != 0)
        {
            fprintf(stderr, "Impossible de mettre Musics : %s \n", SDL_GetError());
            SDL_DestroyTexture(MenuTexture);
            SDL_DestroyTexture(TitleTexture);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        // Gestion du bouton check (Mute les sons)
        if (checksounds)
        {
            indice = get_indice_by_color(GREEN);
            Mix_Volume(-1, 0);
        }
        else
        {
            indice = get_indice_by_color(NOTHING);
            Mix_Volume(-1, MIX_MAX_VOLUME);
        }

        // On affiche si c'est selectionner ou non
        SDL_Rect soundRect = {soundButton.rect.x + soundButton.rect.w + 100, soundButton.rect.y, 100, 100};
        if (SDL_RenderCopy(renderer, imageTexture[indice], NULL, &soundRect) != 0)
        {
            fprintf(stderr, "Impossible de mettre Sounds : %s \n", SDL_GetError());
            SDL_DestroyTexture(MenuTexture);
            SDL_DestroyTexture(TitleTexture);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        // On gère les évènements sur la page
        int event = settings_events(&backButton, &musicButton, &soundButton, &volumeDownButton, &volumeUpButton, &pourcentVolume);

        // si on selectionne le bouton retour, on reviens dans le menu
        if (event == 1)
        {
            run = false;
            home_page_SDL(tetris);
        }
        SDL_RenderPresent(renderer);
    }

    // On libère nos ressources
    SDL_DestroyTexture(MenuTexture);
    SDL_DestroyTexture(TitleTexture);
}

void level_selection_SDL(Tetris *tetris)
{
    if (checksounds == false)
    {
        play_sound_SDL(6);
    }

    SDL_Color textColor = {255, 255, 255};

    // Créer le fond d'écran
    SDL_Texture *MenuTexture = get_background("menu");

    // Créer le titre
    SDL_Texture *TitleTexture = display_title("Level Selection", textColor);

    Button backButton = {{SCREEN_WIDTH - 400, SCREEN_HEIGHT - 200, 300, 100}, "Back", 0};

    const int numLevels = 20;
    Button levelButtons[numLevels];
    int buttonWidth = 100;
    int buttonHeight = 100;
    int buttonSpacing = 100;
    int levelsPerRow = 5;

    create_level_buttons(levelButtons, numLevels, buttonWidth, buttonHeight, buttonSpacing, levelsPerRow);

    int selectedLevel = 0;

    bool run = true;
    while (run)
    {
        SDL_RenderClear(renderer);
        // Affiche le fond
        if (SDL_RenderCopy(renderer, MenuTexture, NULL, NULL) != 0)
        {
            fprintf(stderr, "Erreur Impossible de charger le fond ( Level ): %s \n", SDL_GetError());
            SDL_DestroyTexture(MenuTexture);
            SDL_DestroyTexture(TitleTexture);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        // Afficher le titre
        SDL_Rect titleRect = {SCREEN_WIDTH / 2 - 800, 50, 600, 200};
        if (SDL_RenderCopy(renderer, TitleTexture, NULL, &titleRect) != 0)
        {
            fprintf(stderr, "Erreur Impossible de charger le titre ( Level ) : %s \n", SDL_GetError());
            SDL_DestroyTexture(TitleTexture);
            SDL_DestroyTexture(MenuTexture);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        // Afficher le bouton de retour
        display_button(&backButton);

        // Affiche les boutons selectionnés ( Niveaux )
        for (int i = 0; i < numLevels; i++)
        {
            levelButtons[i].selected = (i == selectedLevel) ? 1 : 0;
            display_button(&levelButtons[i]);
        }

        // Gère les évènements de l'utilisateur
        int eventResult = level_selection_events(&selectedLevel, &backButton, levelButtons, numLevels);

        if (eventResult == 1)
        {
            run = false;
            tetris->state = MENU;
        }
        else if (eventResult == 2)
        {
            run = false;
            tetris->level = selectedLevel;
            tetris->state = GAME;
        }

        // Mettre à jour l'affichage
        SDL_RenderPresent(renderer);
    }

    // Libérer les ressources
    free_levels(levelButtons, numLevels);
    SDL_DestroyTexture(MenuTexture);
    SDL_DestroyTexture(TitleTexture);
}

void home_page_SDL(Tetris *tetris)
{
    if (tetris->state != MENU)
    {
        tetris->state = MENU;
    }

    SDL_Texture *MenuTexture = get_background("menu");

    if (!Mix_PlayingMusic() && checkmusics == false)
    {
        Mix_PlayMusic(musics[0], -1);
        currentMusic = musics[0];
    }

    SDL_Color textColor = {255, 255, 255};

    SDL_Texture *tetrisTextTexture = display_title("TETRIS", textColor);

    // Afficher les boutons
    Button play = {{SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 100, 400, 150}, "JOUER", 1};
    Button settings = {{SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 + 100, 400, 150}, "OPTIONS", 0};
    Button quit = {{SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 + 300, 400, 150}, "EXIT", 0};

    bool run = true;
    while (run)
    {
        SDL_RenderClear(renderer);

        // Affiche le fond
        if (SDL_RenderCopy(renderer, MenuTexture, NULL, NULL) != 0)
        {
            fprintf(stderr, "Impossible de mettre le fond (MENU) : %s \n", SDL_GetError());
            SDL_DestroyTexture(tetrisTextTexture);
            SDL_DestroyTexture(MenuTexture);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        // Affiche le titre TETRIS
        SDL_Rect tetrisTextRect = {(SCREEN_WIDTH / 2) - 450, SCREEN_HEIGHT / 4 - 200, 1000, 400};
        if (SDL_RenderCopy(renderer, tetrisTextTexture, NULL, &tetrisTextRect) != 0)
        {
            fprintf(stderr, "Impossible de mettre le titre ( TETRIS HOME ) : %s \n", SDL_GetError());
            SDL_DestroyTexture(tetrisTextTexture);
            SDL_DestroyTexture(MenuTexture);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        // Afficher les boutons à chaque itération
        display_image_button(&play);
        display_image_button(&settings);
        display_image_button(&quit);

        if (home_page_events(&play, &settings, &quit, tetris))
        {
            run = false;
        }

        // Mettre à jour l'affichage
        SDL_RenderPresent(renderer);
    }
    // On libère le Menutexture
    SDL_DestroyTexture(tetrisTextTexture);
    SDL_DestroyTexture(MenuTexture);
}

// DISPLAY END & HIGHSCORE
void display_highscores(Tetris *tetris)
{
    SDL_Color textColor = {255, 255, 255};
    // Position initiale pour le tableau de highscores
    int tableX = SCREEN_WIDTH / 2 - 800;
    int tableY = 250;

    // Afficher le titre du tableau Highscores
    SDL_Rect tableTitleRect = {tableX, tableY, 0, 0};
    display_txt("Highscores", tableTitleRect, textColor);

    // Afficher les en-têtes du tableau
    // RANG
    SDL_Rect rankHeaderRect = {tableX, tableY + 50, 0, 0};
    display_txt("Rank", rankHeaderRect, textColor);

    // PSEUDO
    SDL_Rect UserHeaderRect = {tableX + 200, tableY + 50, 0, 0};
    display_txt("User ID", UserHeaderRect, textColor);

    // SCORE
    SDL_Rect scoreHeaderRect = {tableX + 600, tableY + 50, 0, 0};
    display_txt("Score", scoreHeaderRect, textColor);

    for (int i = 0; i < 10; i++)
    {
        SDL_Rect rankTextRect = {tableX, tableY + 100 + i * 50, 0, 0};
        char *rankText = int_to_str(i + 1);
        display_txt(rankText, rankTextRect, textColor);
        free(rankText);

        SDL_Rect pseudoTextRect = {tableX + 200, tableY + 100 + i * 50, 0, 0};
        display_txt(tetris->highscores[i].name, pseudoTextRect, textColor);

        SDL_Rect scoreRect = {tableX + 600, tableY + 100 + i * 50, 0, 0};
        char *scoreText = int_to_str(tetris->highscores[i].score);
        display_txt(scoreText, scoreRect, textColor);
        free(scoreText);
    }
}

void update_highscores(Tetris *tetris, char *playerName)
{
    if (save == true)
    {
        return;
    }

    int numHighscores = 10;
    int positionPlayer;

    // Trouver la position dans le tableau du nouveau score du joueur
    for (positionPlayer = 0; positionPlayer < numHighscores; positionPlayer++)
    {
        if (tetris->score > tetris->highscores[positionPlayer].score)
        {
            break;
        }
    }

    // On doit décaler les éléments
    for (int i = numHighscores; i > positionPlayer; i--)
    {
        tetris->highscores[i] = tetris->highscores[i - 1];
    }

    tetris->highscores[positionPlayer].name = playerName;
    tetris->highscores[positionPlayer].score = tetris->score;

    save = true;
    save_highscores(tetris, numHighscores);
}

void save_highscores(Tetris *tetris, int numHighscores)
{
    // On est en r+ (lecture/écriture)

    // On vérifie que le fichier est pas NULL
    if (tetris->file == NULL)
    {
        fprintf(stderr, "Erreur d'ouverture du fichier ( tetris->file ) NULL");
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // Supprimer les anciens scores

    // Remettre le curseur de fichier au début
    rewind(tetris->file);

    // Tronquer le fichier à la taille 0 pour supprimer les anciens scores
    if (ftruncate(fileno(tetris->file), 0) == -1)
    {
        fprintf(stderr, "Erreur lors de la troncature du fichier");
        close_SDL();
        exit(EXIT_FAILURE);
    }

    // On écrit les nouveaux scores
    for (int i = 0; i < numHighscores; i++)
    {
        fprintf(tetris->file, "%s,%d\n", tetris->highscores[i].name, tetris->highscores[i].score);
    }
}

void end_screen_button_events(SDL_Keycode key, Tetris *tetris, char *playerName, Button *replayButton, Button *quitButton, Button *saveButton, int *quit)
{
    switch (key)
    {
    case SDLK_RETURN:
        if (saveButton->selected == 1 && strlen(playerName) > 0)
        {
            update_highscores(tetris, playerName);
        }
        else if (quitButton->selected == 1)
        {
            *quit = 1;
            close_SDL();
            tetris->state = CLOSE;
            exit(EXIT_SUCCESS);
        }
        else if (replayButton->selected == 1)
        {
            *quit = 1;
            tetris->state = RESTART;
            Mix_HaltMusic();
        }
        break;
    case SDLK_RIGHT:
    case SDLK_d:
        if (replayButton->selected == 1)
        {
            replayButton->selected = 0;
            quitButton->selected = 1;
        }
        else if (quitButton->selected == 1)
        {
            saveButton->selected = 1;
            quitButton->selected = 0;
        }
        else if (saveButton->selected == 1)
        {
            replayButton->selected = 1;
            saveButton->selected = 0;
        }
        break;
    case SDLK_LEFT:
    case SDLK_q:
        if (replayButton->selected == 1)
        {
            replayButton->selected = 0;
            saveButton->selected = 1;
        }
        else if (quitButton->selected == 1)
        {
            replayButton->selected = 1;
            quitButton->selected = 0;
        }
        else if (saveButton->selected == 1)
        {
            quitButton->selected = 1;
            saveButton->selected = 0;
        }
        break;
    default:
        break;
    }
}

void end_screen_text_input(SDL_Event *event, char *playerName)
{
    switch (event->type)
    {
    case SDL_TEXTINPUT:
        if (strlen(playerName) < 20)
        {
            strcat(playerName, event->text.text);
        }
        break;
    case SDL_KEYDOWN:
        switch (event->key.keysym.sym)
        {
        case SDLK_BACKSPACE:
            if (strlen(playerName) > 0)
            {
                playerName[strlen(playerName) - 1] = '\0';
            }
            break;
        }
        break;
    }
}

void display_player_name(SDL_Rect textInputRect, char *playerName, SDL_Color textColor)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &textInputRect);

    TTF_Font *textFont = TTF_OpenFont("assets/ttf/Tetris.ttf", 70);
    if (!textFont)
    {
        fprintf(stderr, "Error opening font for the title: %s\n", TTF_GetError());
        TTF_CloseFont(textFont);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    SDL_Surface *textSurface = TTF_RenderText_Solid(textFont, playerName, textColor);
    if (!textSurface)
    {
        fprintf(stderr, "Error rendering text: %s\n", TTF_GetError());
        TTF_CloseFont(textFont);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture)
    {
        fprintf(stderr, "Error creating texture: %s\n", SDL_GetError());
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(textFont);
        close_SDL();
        exit(EXIT_FAILURE);
    }

    SDL_Rect textRect = {textInputRect.x + 20, textInputRect.y + 20, textSurface->w, textSurface->h};
    if (SDL_RenderCopy(renderer, textTexture, NULL, &textRect) != 0)
    {
        fprintf(stderr, "Error rendering texture: %s\n", SDL_GetError());
        SDL_DestroyTexture(textTexture);
        SDL_FreeSurface(textSurface);
        TTF_CloseFont(textFont);
        close_SDL();
        exit(EXIT_FAILURE);
    }
    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(textFont);
}

int is_highscore(Tetris *tetris)
{
    for (int i = 0; i < 10; i++)
    {
        if (tetris->score > tetris->highscores[i].score)
        {
            return 1;
        }
    }

    return 0;
}

void end_screen_SDL(Tetris *tetris)
{
    tetris->state = END;
    SDL_Color textColor = {255, 255, 255};

    SDL_Texture *menuTexture = get_background("menu");
    SDL_Texture *titleTexture = display_title("End", textColor);

    Button replayButton = {{SCREEN_WIDTH - 1000, 50, 300, 100}, "Restart", 0};
    Button quitButton = {{SCREEN_WIDTH - 400, 50, 300, 100}, "Quit", 0};
    Button saveButton = {{SCREEN_WIDTH - 500, SCREEN_HEIGHT - 150, 400, 100}, "Register", 0};
    saveButton.selected = 1;

    if (checkmusics == false)
    {
        if (currentMusic != musics[2])
        {
            Mix_PlayMusic(musics[2], -1);
            currentMusic = musics[2];
        }
    }

    char playerName[30];
    SDL_Rect textInputRect = {SCREEN_WIDTH / 2 - 800, SCREEN_HEIGHT - 150, 1000, 100};

    int quit = 0;
    SDL_Event event;
    SDL_StartTextInput();

    // Variable pour déterminer le mode d'entrée (0 pour boutons, 1 pour texte)
    int inputMode = 0;

    while (!quit)
    {
        SDL_RenderClear(renderer);

        if (SDL_RenderCopy(renderer, menuTexture, NULL, NULL) != 0)
        {
            fprintf(stderr, "Error applying MenuTexture (END): %s ", SDL_GetError());
            SDL_DestroyTexture(menuTexture);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        SDL_Rect titleRect = {SCREEN_WIDTH / 2 - 800, 100, 400, 100};
        if (SDL_RenderCopy(renderer, titleTexture, NULL, &titleRect) != 0)
        {
            fprintf(stderr, "Error applying TitleTexture (END): %s ", SDL_GetError());
            SDL_DestroyTexture(titleTexture);
            SDL_DestroyTexture(menuTexture);
            close_SDL();
            exit(EXIT_FAILURE);
        }

        display_button(&replayButton);
        display_button(&quitButton);
        display_button(&saveButton);

        if (is_highscore(tetris))
        {
            SDL_Rect highscoreRect = {SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT - 450, 400, 50};
            display_txt("Well done, you've set a new record !", highscoreRect, textColor);
        }
        else
        {
            SDL_Rect noHighscoreRect = {SCREEN_WIDTH / 2 + 200, SCREEN_HEIGHT - 450, 400, 50};
            display_txt("you didn't break the record :( ", noHighscoreRect, textColor);
        }
        SDL_Rect ScorePlayer = {SCREEN_WIDTH - 600, SCREEN_HEIGHT - 400, 400, 50};
        char *scoreText = int_to_str_with_prefix("Your score :", tetris->score);
        display_txt(scoreText, ScorePlayer, textColor);
        free(scoreText);

        display_highscores(tetris);

        if (strlen(playerName) > 0)
        {
            display_player_name(textInputRect, playerName, textColor);
        }

        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                close_SDL();
                exit(EXIT_SUCCESS);
                break;
            case SDL_TEXTINPUT:
            case SDL_KEYDOWN:
                if (inputMode == 0)
                {
                    // Traitement des événements de boutons
                    end_screen_button_events(event.key.keysym.sym, tetris, playerName, &replayButton, &quitButton, &saveButton, &quit);
                }
                else
                {
                    // Traitement des événements de texte
                    end_screen_text_input(&event, playerName);
                }
                break;
            }

            // Changer le mode d'entrée avc la touche Tab
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_TAB)
            {
                inputMode = 1 - inputMode;
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_StopTextInput();
    SDL_DestroyTexture(menuTexture);
    SDL_DestroyTexture(titleTexture);
}

// SOUNDS
void play_sound_SDL(int i)
{
    // Vérifier si la bibliothèque SDL Mixer est initialisée
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        fprintf(stderr, "Erreur lors de l'initialisation de SDL Mixer : %s\n", Mix_GetError());
        return;
    }

    // Vérifier si l'indice est dans une plage valide
    if (i < 0 || i >= 10)
    {
        fprintf(stderr, "Indice de son invalide : %d\n", i);
        return;
    }

    // Jouer le son dans le tableau d'indice i
    if (Mix_PlayChannel(-1, sounds[i], 0) == -1)
    {
        fprintf(stderr, "Erreur lors de la lecture du son : %s\n", Mix_GetError());
    }
}

// CLOSE & CLEAR
void cleanup_audio()
{
    // Libérer les musiques
    for (int i = 0; i < 4; ++i)
    {
        if (musics[i] != NULL)
        {
            Mix_FreeMusic(musics[i]);
            musics[i] = NULL;
        }
    }

    // Libérer les sons
    for (int i = 0; i < 10; ++i)
    {
        if (sounds[i] != NULL)
        {
            Mix_FreeChunk(sounds[i]);
            sounds[i] = NULL;
        }
    }
}

void clear_img_textures()
{
    for (color current = NOTHING; current <= GREEN; current++)
    {
        int indice = get_indice_by_color(current);
        SDL_DestroyTexture(imageTexture[indice]);
        imageTexture[indice] = NULL;
    }
}

void free_levels(Button levelButton[], int numLevels)
{
    for (int i = 0; i < numLevels; i++)
    {
        free(levelButton[i].text);
    }
}

void close_SDL()
{
    // Libérer les textures
    clear_img_textures();

    // Libérer les musiques et les sons
    cleanup_audio();

    // Libérer la texture du fond
    if (backgroundTexture != NULL)
    {
        SDL_DestroyTexture(backgroundTexture);
        backgroundTexture = NULL;
    }

    // Fermer l'audio
    Mix_CloseAudio();

    // Fermer le renderer SDL
    if (renderer != NULL)
    {
        SDL_DestroyRenderer(renderer);
        renderer = NULL;
    }

    // Fermer la fenêtre SDL
    if (window != NULL)
    {
        SDL_DestroyWindow(window);
        window = NULL;
    }

    // Fermer la font
    if (ButtonFont != NULL)
    {
        TTF_CloseFont(ButtonFont);
        ButtonFont = NULL;
    }

    if (font != NULL)
    {
        TTF_CloseFont(font);
        font = NULL;
    }

    // Fermer la librairie SDL_ttf
    TTF_Quit();

    // Fermer la librairie SDL_mixer
    Mix_Quit();

    // Fermer la librairie SDL
    SDL_Quit();
}
