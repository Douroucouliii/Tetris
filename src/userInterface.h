#pragma once

#include <stdio.h>

typedef struct Tetris Tetris;

// Structure de pointeur de fonctions
typedef struct
{
    void (*init_interface)(void);  // Permet d'initialiser l'interface
    void (*close_interface)(void); // Permet de fermer l'interface
    void (*display)(Tetris *);     // Permet d'afficher le tetris
    char (*input)(Tetris *);           // Permet de récupérer un input selon si c'est SDL ou NCurses
    void (*home_page)(Tetris *);   // Pour le menu
    void (*end_screen)(Tetris *);      // Permet d'afficher la fin de partie (highscore, nom, etc)
    void (*display_info)(Tetris *);// Permet d'afficher les informations annexe (score/niveau etc...)
    void (*play_sound)(int);    // Permet de jouer un son du tableau de son (seulement pour SDL)
} Function;

// Structure qui prend une instance et des pointeurs de fonctions
typedef struct
{
    char *instance;
    Function *functions;
} userInterface;

userInterface sdlInterface_init_();
userInterface nCursesInterface_init_();
