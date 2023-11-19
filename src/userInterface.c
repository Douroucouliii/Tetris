#include <stdlib.h>

#include "userInterface.h"
#include "textInterface.h"
#include "sdlInterface.h"
#include "nCursesInterface.h"

userInterface textInterface_init_(){
    userInterface interface;
    Fonction* f = (Fonction *) malloc(sizeof(Fonction));
    if(!f){
        perror("malloc()\n");
        exit(EXIT_FAILURE);
    }
    interface.fonctions = f;
    interface.instance = NULL;
    interface.fonctions->affiche = affiche_text;
    return interface;
}

userInterface sdlInterface_init_(){
    userInterface interface;
    Fonction* f = (Fonction *) malloc(sizeof(Fonction));
    if(!f){
        perror("malloc()\n");
        exit(EXIT_FAILURE);
    }
    interface.fonctions = f;
    interface.instance = NULL;
    interface.fonctions->affiche = affiche_sdl;
    return interface;
}

userInterface nCursesInterface_init_(){
    userInterface interface;
    Fonction* f = (Fonction *) malloc(sizeof(Fonction));
    if(!f){
        perror("malloc()\n");
        exit(EXIT_FAILURE);
    }
    interface.fonctions = f;
    interface.instance = NULL;
    interface.fonctions->affiche = affiche_nCurses;
    return interface;
}

