#pragma once

#include "modele.h"

//Structure qui prend une instance et des pointeurs de fonctions
typedef struct{
    char *instance;
    Fonction fonctions;
}userInterface;

//Structure de pointeur de fonctions
typedef struct{
    void(*affiche)(Tetris);
}Fonction;

userInterface textInterface_init_();
userInterface sdlInterface_init_();
userInterface nCursesInterface_init_();
