#include <stdlib.h>
#include <ncurses.h>

#include "modele.h"
#include "nCursesInterface.h"

// Initialise nCurses
void init_nCurses()
{
    // Lance NCurses
    initscr();
    // Pas besoin d'appuyer sur entrée
    cbreak();
    // N'écrit pas les caracteres tapés
    noecho();
    // Enleve le curseur (pas besoin pour le tetris)
    curs_set(FALSE);
    // Enleve le buffering
    raw();
    // Initialise les paires de couleurs pour nos pièces.
    start_color();
    if (!has_colors())
    {
        endwin();
        printf("Votre terminal ne supporte pas les couleurs\n");
        exit(EXIT_FAILURE);
    }
    init_pair(CYAN, COLOR_CYAN, COLOR_BLACK);      // Cyan
    init_pair(YELLOW, COLOR_YELLOW, COLOR_BLACK);  // Yellow
    init_pair(PURPLE, COLOR_MAGENTA, COLOR_BLACK); // Purple
    init_pair(ORANGE, COLOR_WHITE, COLOR_BLACK);   // Orange (ncurses n'a pas de couleur orange, donc on utilise blanc à la place)
    init_pair(BLUE, COLOR_BLUE, COLOR_BLACK);      // Blue
    init_pair(RED, COLOR_RED, COLOR_BLACK);        // Red
    init_pair(GREEN, COLOR_GREEN, COLOR_BLACK);    // Green
}

// Ferme nCurses
void close_nCurses()
{
    endwin();
}

// Récupère un input depuis NCurses et retourne le char correspondant dans le modele
char input_nCurses()
{
    char c = getch();
    switch (c)
    {
    case 'q':
        return 'q';
    case 'd':
        return 'd';
    case 's':
        return 's';
    case 'z':
        return 'z';
    case 'a':
        return 'a';
    case 'e':
        return 'e';
    default:
        return ' ';
    }
}

// Affiche le tetris dans la console
void display_nCurses(Tetris *tetris)
{
    WINDOW *win;

    // Crée la taille de la fenetre
    win = newwin(tetris->ligne + 2, tetris->colonne * 2 + 2, 0, 0);
    // Le cadre autour du jeu
    box(win, 0, 0);
    wrefresh(win);

    for (int i = 0; i < tetris->ligne; i++)
    {
        for (int j = 0; j < tetris->colonne; j++)
        {
            if (tetris->board[i][j].isFull)
            {
                // Utilisez une paire de couleurs en fonction de la couleur de la cellule
                attron(COLOR_PAIR(tetris->board[i][j].c));
                mvwprintw(win, i + 1, j * 2 + 1, "[]");
                attroff(COLOR_PAIR(tetris->board[i][j].c));
            }
            else
            {
                mvwprintw(win, i + 1, j * 2 + 1, "  ");
            }
        }
    }

    wrefresh(win);
}