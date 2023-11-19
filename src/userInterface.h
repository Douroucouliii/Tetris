#pragma once

typedef struct Tetris Tetris;

//Structure de pointeur de fonctions
typedef struct{
    void (*affiche)(Tetris*);
}Fonction;

//Structure qui prend une instance et des pointeurs de fonctions
typedef struct{
    char *instance;
    Fonction* fonctions;
}userInterface;

userInterface textInterface_init_();
userInterface sdlInterface_init_();
userInterface nCursesInterface_init_();
