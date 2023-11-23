#include <ncurses.h>

#include "modele.h"
#include "nCursesInterface.h"

// Initialise nCurses
void init_nCurses()
{
    initscr();
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

    // Pas besoin d'appuyer sur entrée
    cbreak();
    // N'écrit pas les caracteres tapés
    noecho();
    // Enleve le curseur (pas besoin pour le tetris)
    curs_set(FALSE);
    // Crée la taille de la fenetre
    win = newwin(tetris->ligne, tetris->colonne * 2, 0, 0);
    // Le cadre autour du jeu
    box(win, 0, 0);
    wrefresh(win);

    for (int i = 0; i < tetris->ligne; i++)
    {
        for (int j = 0; j < tetris->colonne; j++)
        {
            if (tetris->board[i][j].isFull)
            {
                mvwprintw(win, i, j * 2, "[]");
            }
            else
            {
                // Utiliser des codes ANSI de couleur pour chaque pièce
                switch (tetris->board[i][j].c)
                {
                case NOTHING:
                    mvwprintw(win, i, j * 2, "  ");
                    break;
                case CYAN:
                    mvwprintw(win, i, j * 2, "\033[0;36m[]\033[0m");
                    break;
                case YELLOW:
                    mvwprintw(win, i, j * 2, "\033[1;33m[]\033[0m");
                    break;
                case PURPLE:
                    mvwprintw(win, i, j * 2, "\033[0;35m[]\033[0m");
                    break;
                case ORANGE:
                    mvwprintw(win, i, j * 2, "\033[0;33m[]\033[0m");
                    break;
                case BLUE:
                    mvwprintw(win, i, j * 2, "\033[0;34m[]\033[0m");
                    break;
                case RED:
                    mvwprintw(win, i, j * 2, "\033[0;31m[]\033[0m");
                    break;
                case GREEN:
                    mvwprintw(win, i, j * 2, "\033[0;32m[]\033[0m");
                    break;
                default:
                    mvwprintw(win, i, j * 2, "[]");
                }
                mvwprintw(win, i, j * 2, "  ");
            }
        }
        return;
    }
}