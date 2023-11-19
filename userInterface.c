#include "userInterface.h"
#include "textInterface.h"
#include "sdlInterface.h"
#include "nCursesInterface.h"

userInterface textInterface_init_(){
    userInterface interface;
    interface.instance = NULL;
    interface.fonctions.affiche = *affiche_text;
    return interface;
}

userInterface sdlInterface_init_(){
    userInterface interface;
    interface.instance = NULL;
    interface.fonctions.affiche = *affiche_sdl;
    return interface;
}

userInterface nCursesInterface_init_(){
    userInterface interface;
    interface.instance = NULL;
    interface.fonctions.affiche = *affiche_nCurses;
    return interface;
}

