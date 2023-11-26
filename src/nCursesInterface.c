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
    use_default_colors();
    if (!has_colors())
    {
        endwin();
        printf("Votre terminal ne supporte pas les couleurs\n");
        exit(EXIT_FAILURE);
    }
    init_pair(CYAN, COLOR_CYAN, COLOR_CYAN);         // Cyan
    init_pair(YELLOW, COLOR_YELLOW, COLOR_YELLOW);   // Yellow
    init_pair(PURPLE, COLOR_MAGENTA, COLOR_MAGENTA); // Purple
    int COLOR_ORANGE = 8;                            // Initialise la couleur (ncurses n'a pas de couleur orange)
    init_color(COLOR_ORANGE, 1000, 647, 0);          // Création de la couleur orange (ncurses n'a pas de couleur orange)
    init_pair(ORANGE, COLOR_ORANGE, COLOR_ORANGE);   // Orange
    init_pair(BLUE, COLOR_BLUE, COLOR_BLUE);         // Blue
    init_pair(RED, COLOR_RED, COLOR_RED);            // Red
    init_pair(GREEN, COLOR_GREEN, COLOR_GREEN);      // Green
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
    case 'Q':
        return 'q';
    case 'd':
    case 'D':
        return 'd';
    case 's':
    case 'S':
        return 's';
    case 'z':
    case 'Z':
        return 'z';
    case 'a':
    case 'A':
        return 'a';
    case 'e':
    case 'E':
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
    win = newwin(tetris->line + 2, tetris->column * 2 + 2, 0, 0);
    // Le cadre autour du jeu
    box(win, 0, 0);
    wrefresh(win);

    for (int i = 0; i < tetris->line; i++)
    {
        for (int j = 0; j < tetris->column; j++)
        {
            if (tetris->board[i][j].isFull)
            {
                // Utilisez une paire de couleurs en fonction de la couleur de la cellule
                wattron(win, COLOR_PAIR(tetris->board[i][j].c));
                mvwprintw(win, i + 1, j * 2 + 1, "  ");
                wattroff(win, COLOR_PAIR(tetris->board[i][j].c));
            }
            else
            {
                mvwprintw(win, i + 1, j * 2 + 1, "  ");
            }
        }
    }
    // mvwprintw(win, tetris->line + 3, 1, "Nombre de lignes supprimés : %d", tetris->nbLines);
    // mvwprintw(win, tetris->line + 4, 1, "Score : %d", tetris->score);

    wrefresh(win);
}