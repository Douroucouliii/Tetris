#include <stdlib.h>
#include <ncurses.h>
#include <string.h>

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

// Fonction pour calculer le délai en fonction du niveau (en utilisant les frames ou cellules de grille)
int delay(int niveau) {
    int frames;

    // le nombre de frames/cellules de grille par pièce en fonction du niveau
    switch (niveau) {
        case 0:
            frames = 48;
            break;
        case 1:
            frames = 43;
            break;
        case 2:
            frames = 38;
            break;
        case 3:
            frames = 33;
            break;
        case 4:
            frames = 28;
            break;
        case 5:
            frames = 23;
            break;
        case 6:
            frames = 18;
            break;
        case 7:
            frames = 13;
            break;
        case 8:
            frames = 8;
            break;
        case 9:
            frames = 6;
            break;
        case 10: case 11: case 12:
            frames = 5;
            break;
        case 13: case 14: case 15:
            frames = 4;
            break;
        case 16: case 17: case 18:
            frames = 3;
            break;
        case 19: case 20: case 21: case 22: case 23: case 24: case 25: case 26: case 27: case 28:
            frames = 2;
            break;
        //Niveau >28 : Killscreen : seul mattéo pourrait survivre ici mais bon on sait jamais :)
        default:
            frames = 1; //C'est la mort
            break;
    }

    // Calculer le délai en fonction du nombre de frames par pièce
    int delai = (frames * 1000) / 60; // Convertir les frames en millisecondes (60 frames par seconde)
    return delai;
}

// Récupère un input depuis NCurses et retourne le char correspondant dans le modele
char input_nCurses(Tetris *tetris)
{
    int delai = delay(tetris->level);

    //Si le jeu a commencé
    if(tetris->start){
        //On crée un timeout pour la fenetre pour faire descendre nos pieces
        timeout(delai);
    }

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
    case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
        return c;
    case ERR:
    default:
        return ' ';
    }
}

// Affiche le tetris dans la console
void display_nCurses(Tetris *tetris)
{
    //On supprime le menu
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

void home_page_nCurses(Tetris *tetris) {
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
    mvwprintw(home_page, term_rows / 2, term_cols / 2 - 13, "Choisissez un niveau : (0-9)");
    mvwprintw(home_page, term_rows / 2 + 2, term_cols / 2 - 16, "Entrez au clavier /!\\ verr num /!\\");

    wrefresh(home_page);
}

void end_screen_nCurses(Tetris *tetris){
    //On supprime le menu
    wclear(win);
    wclear(score);
    wclear(piece_stats);
    wclear(level);
    wclear(next_piece);

    // La taille du terminal
    int term_rows, term_cols;
    getmaxyx(stdscr, term_rows, term_cols);

    //Créer la fenêtre de fin de partie
    end_screen = newwin(term_rows, term_cols, 0, 0);
    box(end_screen, 0, 0);

    mvwprintw(end_screen, term_rows / 2, term_cols / 2 - 13, "q pour quitter le jeu");
    mvwprintw(end_screen, term_rows / 2 + 2, term_cols / 2 - 16, "et oui c'est fini :(");

    wrefresh(end_screen);
}