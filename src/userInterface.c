#include <stdio.h>
#include <stdlib.h>

#include "userInterface.h"
#include "sdlInterface.h"
#include "textInterface.h"
#include "nCursesInterface.h"

userInterface textInterface_init_()
{
    userInterface interface;
    Fonction *f = (Fonction *)malloc(sizeof(Fonction));
    if (!f)
    {
        perror("malloc()\n");
        exit(EXIT_FAILURE);
    }
    interface.fonctions = f;
    interface.instance = "NCurses";
    interface.fonctions->init_interface = init_text;
    interface.fonctions->close_interface = close_text;
    interface.fonctions->display = display_text;
    interface.fonctions->input = input_text;
    interface.fonctions->display_info = NULL;
    return interface;
}

userInterface nCursesInterface_init_()
{
    userInterface interface;
    Fonction *f = (Fonction *)malloc(sizeof(Fonction));
    if (!f)
    {
        perror("malloc()\n");
        exit(EXIT_FAILURE);
    }
    interface.fonctions = f;
    interface.instance = "NCurses";
    interface.fonctions->init_interface = init_nCurses;
    interface.fonctions->close_interface = close_nCurses;
    interface.fonctions->display = display_nCurses;
    interface.fonctions->input = input_nCurses;
    interface.fonctions->display_info = display_info_nCurses;
    return interface;
}

userInterface sdlInterface_init_()
{
    userInterface interface;
    Fonction *f = (Fonction *)malloc(sizeof(Fonction));
    if (!f)
    {
        perror("malloc()\n");
        exit(EXIT_FAILURE);
    }
    interface.fonctions = f;
    interface.instance = "SDL";
    // Pour l'instant à NULL, je fais d'abord l'affichage text/Ncurses
    interface.fonctions->init_interface = NULL;
    interface.fonctions->close_interface = NULL;
    interface.fonctions->display = display_sdl;
    interface.fonctions->input = NULL;
    interface.fonctions->display_info = NULL;
    return interface;
}
