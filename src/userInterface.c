#include <stdio.h>
#include <stdlib.h>

#include "userInterface.h"
#include "sdlInterface.h"
#include "textInterface.h"
#include "nCursesInterface.h"

userInterface textInterface_init_()
{
    userInterface interface;
    Function *f = (Function *)malloc(sizeof(Function));
    if (!f)
    {
        perror("malloc()\n");
        exit(EXIT_FAILURE);
    }
    interface.functions = f;
    interface.instance = "NCurses";
    interface.functions->init_interface = init_text;
    interface.functions->close_interface = close_text;
    interface.functions->display = display_text;
    interface.functions->input = input_text;
    interface.functions->display_info = NULL;
    interface.functions->home_page = NULL;
    interface.functions->end_screen = NULL;
    return interface;
}

userInterface nCursesInterface_init_()
{
    userInterface interface;
    Function *f = (Function *)malloc(sizeof(Function));
    if (!f)
    {
        perror("malloc()\n");
        exit(EXIT_FAILURE);
    }
    interface.functions = f;
    interface.instance = "NCurses";
    interface.functions->init_interface = init_nCurses;
    interface.functions->close_interface = close_nCurses;
    interface.functions->display = display_nCurses;
    interface.functions->input = input_nCurses;
    interface.functions->display_info = display_info_nCurses;
    interface.functions->home_page = home_page_nCurses;
    interface.functions->end_screen = end_screen_nCurses;
    return interface;
}

userInterface sdlInterface_init_()
{
    userInterface interface;
    Function *f = (Function *)malloc(sizeof(Function));
    if (!f)
    {
        perror("malloc()\n");
        exit(EXIT_FAILURE);
    }
    interface.functions = f;
    interface.instance = "SDL";
    // Pour l'instant à NULL, je fais d'abord l'affichage text/Ncurses
    interface.functions->init_interface = init_SDL;
    interface.functions->close_interface = close_SDL;
    interface.functions->display = display_SDL;
    interface.functions->input = input_SDL;
    interface.functions->display_info = NULL;
    interface.functions->home_page = home_page_SDL;
    interface.functions->end_screen = NULL;
    return interface;
}
