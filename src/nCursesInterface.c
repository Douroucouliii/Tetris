#include <stdlib.h>
#include <ncurses.h>

#include "modele.h"
#include "nCursesInterface.h"

WINDOW *win;
WINDOW *score;
WINDOW *piece_stats;
WINDOW *level;
WINDOW *next_piece;

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

    if (win != NULL)
    {
        delwin(win);
        win = NULL;
    }

    if (score != NULL)
    {
        delwin(score);
        score = NULL;
    }
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
    // La taille du terminal
    int term_rows, term_cols;
    getmaxyx(stdscr, term_rows, term_cols);

    // Calculez la taille et la position de la fenêtre de jeu
    int game_rows = tetris->line * 2;
    int game_cols = tetris->column * 4;
    int game_starty = (term_rows - game_rows) / 2;
    int game_startx = (term_cols - game_cols) / 2;

    // Crée la fenêtre de jeu
    win = newwin(game_rows + 2, game_cols + 2, game_starty, game_startx);
    // Le cadre autour du jeu
    box(win, 0, 0);
    wrefresh(win);

    // Affiche le plateau de jeu
    for (int i = 0; i < tetris->line; i++)
    {
        for (int j = 0; j < tetris->column; j++)
        {
            if (tetris->board[i][j].isFull)
            {
                // Utilisez une paire de couleurs en fonction de la couleur de la cellule
                wattron(win, COLOR_PAIR(tetris->board[i][j].c));
                mvwprintw(win, i * 2 + 1, j * 4 + 1, "    ");
                mvwprintw(win, i * 2 + 2, j * 4 + 1, "    ");
                wattroff(win, COLOR_PAIR(tetris->board[i][j].c));
            }
            else
            {
                mvwprintw(win, i * 2 + 1, j * 4 + 1, "    ");
            }
        }
    }

    wrefresh(win);
}

void display_info_nCurses(Tetris *tetris){
    // La taille du terminal
    int term_rows, term_cols;
    getmaxyx(stdscr, term_rows, term_cols);
    // Calculez la taille et la position de la fenêtre de jeu
    int game_rows = tetris->line * 2;
    int game_cols = tetris->column * 4;
    int game_starty = (term_rows - game_rows) / 2;
    int game_startx = (term_cols - game_cols) / 2;

    // Crée la fenêtre de score
    score = newwin(game_rows / 4 + 1, 20, game_starty + game_rows - 9, game_startx + game_cols + 2);
    box(score, 0, 0);

    // Affiche le score, le nombre de lignes
    mvwprintw(score, 3, 5, "Score : %d", tetris->score);
    mvwprintw(score, 6, 5, "Lignes : %d", tetris->nbLines);

    //Afficher un fenetre à gauche pour les statistiques des pieces
    piece_stats = newwin((int)(game_rows + 2) * 3/4, 20, game_starty, game_startx - 20);
    box(piece_stats, 0, 0);   

    // Afficher chaque pièce avec sa statistique dans la fenêtre pieceStats
    for (int i = 0; i < 7; i++){
        for (int j = 0; j < 4; j++){
            wattron(piece_stats, COLOR_PAIR(tetris->tmpPiece[i]->c));
            //Le I est plus grand que les autres pièces donc on le décale un peu plus
            if(i==6){
                mvwprintw(piece_stats, i * 4 + 3 + tetris->tmpPiece[i]->coords[j][0], 2 + tetris->tmpPiece[i]->coords[j][1]*2 - 3, "  ");
            }else{
                mvwprintw(piece_stats, i * 4 + 3 + tetris->tmpPiece[i]->coords[j][0], 2 + tetris->tmpPiece[i]->coords[j][1]*2 - 5, "  ");
            }
            wattroff(piece_stats, COLOR_PAIR(tetris->tmpPiece[i]->c));
        }
        //Ajouter la stat apres la piece
        mvwprintw(piece_stats, i * 4 + 3, 14, "%d", tetris->pieceStats[i]);
    }

    // Afficher le niveau en bas à gauche
    level = newwin(game_rows / 4 + 1, 20, game_starty + game_rows - 9, game_startx - 20);
    box(level, 0, 0);
    mvwprintw(level, 5, 5, "Niveau : %d", tetris->level);

    // Afficher la prochaine piece
    next_piece = newwin(game_rows / 4 + 1, 20, game_starty, game_startx + game_cols + 2);
    box(next_piece, 0, 0);
    for (int j = 0; j < 4; j++){
        wattron(next_piece, COLOR_PAIR(tetris->nextPiece->c));
        //Si la piece est un I ou un O on doit l'afficher différent car elle prend pas la meme place
        if(tetris->nextPiece->name == 'I'){
            mvwprintw(next_piece, tetris->nextPiece->coords[j][0] * 2 + 4, tetris->nextPiece->coords[j][1] * 4 - 10, "    ");
            mvwprintw(next_piece, tetris->nextPiece->coords[j][0] * 2 + 5, tetris->nextPiece->coords[j][1] * 4 - 10, "    ");
        }else if(tetris->nextPiece->name == 'O'){
            mvwprintw(next_piece, tetris->nextPiece->coords[j][0] * 2 + 3, tetris->nextPiece->coords[j][1] * 4 - 10, "    ");
            mvwprintw(next_piece, tetris->nextPiece->coords[j][0] * 2 + 4, tetris->nextPiece->coords[j][1] * 4 - 10, "    ");
        }else{
            mvwprintw(next_piece, tetris->nextPiece->coords[j][0] * 2 + 3, tetris->nextPiece->coords[j][1] * 4 - 12, "    ");
            mvwprintw(next_piece, tetris->nextPiece->coords[j][0] * 2 + 4, tetris->nextPiece->coords[j][1] * 4 - 12, "    ");
        }
        wattroff(next_piece, COLOR_PAIR(tetris->nextPiece->c));
    }



    wrefresh(score);
    wrefresh(piece_stats);
    wrefresh(level);
    wrefresh(next_piece);
}