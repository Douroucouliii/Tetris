/**
 * @file main.c
 * @author Perin Clément (https://github.com/Douroucouliii)
 * @author Mackowiak Carole (https://github.com/Carole-prog)
 * @brief Main du projet tetris qui crée le modèle, l'interface et lance le jeu
 * @date 2023-11-24
 */

#include <stdio.h>

#include "modele.h"
#include "userInterface.h"

/**
 * @brief Lance le jeu
 *
 * @return int statut de sortie
 */
int main()
{

    Tetris *game = tetris_init_();

    // On devra faire en sorte de pouvoir switch entre les 2 (SDL/NCurses) depuis le jeu :)
    // Pour l'instant j'ai créé une interface nCurses, ensuite on fera la SDL

    // userInterface ui = textInterface_init_();
    userInterface ui = nCursesInterface_init_();
    // userInterface ui = sdlInterface_init_();

    tetris_playGame(game, ui);

    return 0;
} // main