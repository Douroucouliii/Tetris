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

    // Création du modèle
    Tetris *game = tetris_init_();

    //Création des interfaces
    userInterface nCurses = nCursesInterface_init_();
    userInterface SDL = sdlInterface_init_();

    //On lance la boucle du jeu
    tetris_playGame(game, nCurses, SDL);

    return 0;
} // main