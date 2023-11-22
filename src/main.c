#include <stdio.h>

#include "modele.h"
#include "userInterface.h"

int main(){

    Tetris *game = tetris_init_();

    //On devra faire en sorte de pouvoir switch entre les 2 (SDL/NCurses) depuis le jeu :)
    //Pour l'instant j'ai créé une interface Text en plus, c'est juste pour tester en console, mais on l'enlevera
    userInterface ui = textInterface_init_();
    //userInterface ui = sdlInterface_init_();
    //userInterface ui = nCursesInterface_init_();

    tetris_playGame(game, ui);

    return 0;
}  //main