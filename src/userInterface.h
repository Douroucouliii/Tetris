#pragma once

typedef struct Tetris Tetris;

// Structure de pointeur de fonctions
typedef struct
{
    void (*init_interface)(void);  // Permet d'initialiser l'interface
    void (*close_interface)(void); // Permet de fermer l'interface
    void (*display)(Tetris *);     // Permet d'afficher le tetris
    char (*input)(void);           // Permet de récupérer un input selon si c'est SDL ou NCurses
    // void (*menu)(void);     //Plus tard pour le menu   ??
    // void (*display_info)(Tetris *);    // Permet d'afficher les informations annexe (score/niveau etc...)
    // void redim(Tetris *);     // Permet de redimensionner le tetris ??
} Fonction;

// Structure qui prend une instance et des pointeurs de fonctions
typedef struct
{
    char *instance;
    Fonction *fonctions;
} userInterface;

userInterface textInterface_init_();
userInterface sdlInterface_init_();
userInterface nCursesInterface_init_();
