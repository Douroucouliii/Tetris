#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include <unistd.h>

#include "modele.h"
#include "nCursesInterface.h"

WINDOW *home_page;
WINDOW *win;
WINDOW *score;
WINDOW *piece_stats;
WINDOW *level;
WINDOW *next_piece;
WINDOW *end_screen;

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
char input_nCurses(Tetris *tetris)
{
    // On crée un timeout pour la fenetre (permet de sortir de la fonction au bout d'une frame (case ERR))
    timeout(0);

    char c = getch();
    switch (c)
    {
    // Déplacement/rotation de la pièce
    case 'q':
    case 'd':
    case 's':
    case 'a':
    case 'e':
        return c;
        break;
    // Déplacement/rotation de la pièce
    case 'Q':
    case 'D':
    case 'S':
    case 'A':
    case 'E':
        return c + 'A' - 'a';
        break;
    // ' ' quand on sort du timeout (permet de sortir de l'attente de l'input et de faire descendre la pièce)
    case ERR:
        return ' ';
        break;
    default:
        return ' ';
    }
}

// Affiche le tetris dans la console
void display_nCurses(Tetris *tetris)
{
    // On supprime le menu
    wclear(home_page);

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

void display_info_nCurses(Tetris *tetris)
{
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

    // Afficher un fenetre à gauche pour les statistiques des pieces
    piece_stats = newwin((int)(game_rows + 2) * 3 / 4, 20, game_starty, game_startx - 20);
    box(piece_stats, 0, 0);

    // Afficher chaque pièce avec sa statistique dans la fenêtre pieceStats
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            wattron(piece_stats, COLOR_PAIR(tetris->tmpPiece[i]->c));
            // Le I est plus grand que les autres pièces donc on le décale un peu plus
            if (i == 6)
            {
                mvwprintw(piece_stats, i * 4 + 3 + tetris->tmpPiece[i]->coords[j][0], 2 + tetris->tmpPiece[i]->coords[j][1] * 2 - 3, "  ");
            }
            else
            {
                mvwprintw(piece_stats, i * 4 + 3 + tetris->tmpPiece[i]->coords[j][0], 2 + tetris->tmpPiece[i]->coords[j][1] * 2 - 5, "  ");
            }
            wattroff(piece_stats, COLOR_PAIR(tetris->tmpPiece[i]->c));
        }
        // Ajouter la stat apres la piece
        mvwprintw(piece_stats, i * 4 + 3, 14, "%d", tetris->pieceStats[i]);
    }

    // Afficher le niveau en bas à gauche
    level = newwin(game_rows / 4 + 1, 20, game_starty + game_rows - 9, game_startx - 20);
    box(level, 0, 0);
    mvwprintw(level, 5, 5, "Niveau : %d", tetris->level);

    // Afficher la prochaine piece
    next_piece = newwin(game_rows / 4 + 1, 20, game_starty, game_startx + game_cols + 2);
    box(next_piece, 0, 0);
    for (int j = 0; j < 4; j++)
    {
        wattron(next_piece, COLOR_PAIR(tetris->nextPiece->c));
        // Si la piece est un I ou un O on doit l'afficher différent car elle prend pas la meme place
        if (tetris->nextPiece->name == 'I')
        {
            mvwprintw(next_piece, tetris->nextPiece->coords[j][0] * 2 + 4, tetris->nextPiece->coords[j][1] * 4 - 10, "    ");
            mvwprintw(next_piece, tetris->nextPiece->coords[j][0] * 2 + 5, tetris->nextPiece->coords[j][1] * 4 - 10, "    ");
        }
        else if (tetris->nextPiece->name == 'O')
        {
            mvwprintw(next_piece, tetris->nextPiece->coords[j][0] * 2 + 3, tetris->nextPiece->coords[j][1] * 4 - 10, "    ");
            mvwprintw(next_piece, tetris->nextPiece->coords[j][0] * 2 + 4, tetris->nextPiece->coords[j][1] * 4 - 10, "    ");
        }
        else
        {
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

void home_page_nCurses(Tetris *tetris)
{
    // La taille du terminal
    int term_rows, term_cols;
    getmaxyx(stdscr, term_rows, term_cols);

    // Afficher un menu, avec la possibilité de commencer entre le niveau 0 et 9
    home_page = newwin(term_rows, term_cols, 0, 0);
    box(home_page, 0, 0);

    // Affichage du texte "Bienvenue dans Tetris" en plus grand
    mvwprintw(home_page, term_rows / 4, term_cols / 2 - 18, "MMMMMM MMMMMM MMMMMM MMMML  MMMMM MMMMMM");
    mvwprintw(home_page, term_rows / 4 + 1, term_cols / 2 - 18, "  MM   MM       MM   M   M    M   MM    ");
    mvwprintw(home_page, term_rows / 4 + 2, term_cols / 2 - 18, "  MM   MMMM     MM   MMMMP    M   MMMMMM");
    mvwprintw(home_page, term_rows / 4 + 3, term_cols / 2 - 18, "  MM   MM       MM   M  M,    M       MM");
    mvwprintw(home_page, term_rows / 4 + 4, term_cols / 2 - 18, "  MM   MM       MM   M  \"M,   M       MM");
    mvwprintw(home_page, term_rows / 4 + 5, term_cols / 2 - 18, "  MM   MMMMMM   MM   M   \"M MMMMM MMMMMM");

    // Affichage du reste du menu
    mvwprintw(home_page, term_rows / 2, term_cols / 2 - 13, "Select a level : (0-19)");
    mvwprintw(home_page, term_rows / 2 + 2, term_cols / 2 - 22, "(write a number between 0 and 19 and press enter)");

    char number[2];
    do
    {
        mvwgetnstr(home_page, term_rows / 2 + 4, term_cols / 2, number, sizeof(number));
    } while (atoi(number) < 0 || atoi(number) > 19);

    wrefresh(home_page);

    tetris->level = atoi(number);

    tetris->state = GAME;
}

void end_screen_nCurses(Tetris *tetris)
{
    // On supprime le jeu
    wclear(win);
    wclear(score);
    wclear(piece_stats);
    wclear(level);
    wclear(next_piece);

    // La taille du terminal
    int term_rows, term_cols;
    getmaxyx(stdscr, term_rows, term_cols);

    // Créer la fenêtre de fin de partie
    end_screen = newwin(term_rows, term_cols, 0, 0);
    box(end_screen, 0, 0);

    int isHighscore = 0;

    // Affichage END !
    mvwprintw(end_screen, term_rows / 4 - 5, term_cols / 2 - 15, "  ______           _   _ ");
    mvwprintw(end_screen, term_rows / 4 - 4, term_cols / 2 - 15, " |  ____|         | | | |");
    mvwprintw(end_screen, term_rows / 4 - 3, term_cols / 2 - 15, " | |__   _ __   __| | | |");
    mvwprintw(end_screen, term_rows / 4 - 2, term_cols / 2 - 15, " |  __| | '_ \\ / _` | | |");
    mvwprintw(end_screen, term_rows / 4 - 1, term_cols / 2 - 15, " | |____| | | | (_| | |_|");
    mvwprintw(end_screen, term_rows / 4, term_cols / 2 - 15, " |______|_| |_|\\__,_| (_)");

    // Affichage du titre des high scores
    mvwprintw(end_screen, term_rows / 4 + 4, term_cols / 2 - 10, "=== Highscore ===");

    int numHighscores = 10;
    for (int i = 0; i < numHighscores; i++)
    {
        if (tetris->score > tetris->highscores[i].score)
        {
            isHighscore = 1;
        }
        mvwprintw(end_screen, term_rows / 4 + i + 6, term_cols / 2 - 6, "%s %d", tetris->highscores[i].name, tetris->highscores[i].score);
    }

    // Si le score du joueur est parmi les meilleurs scores, lui demander son nom
    if (isHighscore)
    {
        mvwprintw(end_screen, term_rows / 2 + 4, term_cols / 2 - 11, "It's a highscore !");
        mvwprintw(end_screen, term_rows / 2 + 6, term_cols / 2 - 24, "Enter your name : (write your name and press enter)");
        char player_name[10];
        mvwgetnstr(end_screen, term_rows / 2 + 4, term_cols / 2, player_name, sizeof(player_name) - 1);
        // Afficher le nom
        mvwprintw(end_screen, term_rows / 2 + 8, term_cols / 2 - 11, "Your name is : %s", player_name);
        // Mettre à jour les meilleurs scores avec le nouveau score
        // Effacer tout le contenu du fichier
        tetris->file = freopen("data/highscore.txt", "w", tetris->file);
        if (tetris->file == NULL)
        {
            fprintf(stderr, "Erreur lors de la réouverture du fichier.\n");
            exit(EXIT_FAILURE);
        }

        int position = 0;
        for (int i = 0; i < 10; i++)
        {
            if (tetris->score > tetris->highscores[i].score)
            {
                position = i;
                break;
            }
        }
        for (int i = 0; i < position; i++)
        {
            fprintf(tetris->file, "%s,%d\n", tetris->highscores[i].name, tetris->highscores[i].score);
        }
        fprintf(tetris->file, "%s,%d\n", player_name, tetris->score);
        for (int i = position; i < 10 - 1; i++)
        {
            fprintf(tetris->file, "%s,%d\n", tetris->highscores[i].name, tetris->highscores[i].score);
        }
        fclose(tetris->file);
    }
    else
    {
        mvwprintw(end_screen, term_rows / 2 + 8, term_cols / 2 - 20, "This is not an highscore, try again !");
    }

    // Affichage des options pour rejouer ou quitter
    mvwprintw(end_screen, term_rows / 2 + 12, term_cols / 2 - 12, "Press R to play");
    mvwprintw(end_screen, term_rows / 2 + 13, term_cols / 2 - 12, "Press Q to quit");

    wrefresh(end_screen);

    // Attendre que l'utilisateur appuie sur R ou Q
    char c;
    do
    {
        c = getch();
    } while (c != 'r' && c != 'R' && c != 'q' && c != 'Q');

    if (c == 'Q' || c == 'q')
        tetris->state = CLOSE;
}