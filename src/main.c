#include <stdio.h>

#include "modele.h"
#include "userInterface.h"

int main(){

    Tetris *game = tetris_init_();

    //On devra faire en sorte de pouvoir switch entre les 3 depuis le jeu :)
    userInterface ui = textInterface_init_();
    //userInterface ui = sdlInterface_init_();
    //userInterface ui = nCursesInterface_init_();

    tetris_playGame(game, ui);

    return 0;
}  //main