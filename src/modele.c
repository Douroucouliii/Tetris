#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "modele.h"

PieceConfig pieces[7] = {
    {'T', 4, {{0, 4}, {0, 5}, {0, 6}, {1, 5}}, GREEN},
    {'J', 4, {{0, 4}, {0, 5}, {0, 6}, {1, 6}}, CYAN},
    {'Z', 4, {{0, 4}, {0, 5}, {1, 5}, {1, 6}}, BLUE},
    {'O', 4, {{0, 4}, {0, 5}, {1, 4}, {1, 5}}, ORANGE},
    {'S', 4, {{0, 5}, {0, 6}, {1, 4}, {1, 5}}, PURPLE},
    {'L', 4, {{0, 4}, {0, 5}, {0, 6}, {1, 4}}, YELLOW},
    {'I', 4, {{0, 3}, {0, 4}, {0, 5}, {0, 6}}, RED}};

Tetris *tetris_init_()
{

    Tetris *tetris = (Tetris *)malloc(sizeof(Tetris));
    if (!tetris)
    {
        perror("malloc()\n");
        exit(EXIT_FAILURE);
    }

    // Nb ligne et colonne du jeu
    tetris->line = 20;
    tetris->column = 10;

    // Init du tableau de cellule, c'est notre jeu
    init_board(tetris);

    // init du tableau de pointeur de piece (les 7 pieces du jeu), qui serviront pour memcpy. Evite de devoir regénérer une piece à chaque fois.
    init_tmpPiece(tetris);

    // Tableau de pointeur de piece qui stocke toutes les pièces qui ont été dans le jeu (avec des realloc).
    PieceConfig **boardPiece = NULL;
    tetris->boardPiece = boardPiece;

    // Compteur de piece dans le boardPiece
    tetris->nbBoardPiece = 0;

    tetris->state = MENU;

    tetris->nbLines = 0;
    tetris->score = 0;
    tetris->level = 0;
    tetris->nextPiece = NULL;

    for (int i = 0; i < 7; i++)
    {
        tetris->pieceStats[i] = 0;
    }

    return tetris;
}

void init_board(Tetris *tetris)
{
    cell **board = (cell **)malloc(tetris->line * sizeof(cell *));
    if (!board)
    {
        perror("malloc()\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < tetris->line; i++)
    {
        board[i] = (cell *)malloc(tetris->column * sizeof(cell));
        if (!board[i])
        {
            perror("malloc()\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < tetris->column; j++)
        {
            board[i][j].isFull = false;
            board[i][j].c = NOTHING;
        }
    }
    tetris->board = board;
}

void init_tmpPiece(Tetris *tetris)
{
    PieceConfig **tmpPiece = (PieceConfig **)malloc(7 * sizeof(PieceConfig *));
    if (!tmpPiece)
    {
        perror("Erreur malloc()\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 7; i++)
    {
        tmpPiece[i] = (PieceConfig *)malloc(sizeof(PieceConfig));
        if (!tmpPiece[i])
        {
            perror("Erreur malloc()\n");
            exit(EXIT_FAILURE);
        }
        // Initialise à vide
        tmpPiece[i]->name = pieces[i].name;
        tmpPiece[i]->num_cells = pieces[i].num_cells;
        tmpPiece[i]->c = pieces[i].c;
        for (int j = 0; j < 4; j++)
        {
            tmpPiece[i]->coords[j][0] = 0;
            tmpPiece[i]->coords[j][1] = 0;
        }
        // On lui attributs les coordonnées de chaque piece ( avec le tableau pieces )
        for (int j = 0; j < pieces[i].num_cells; j++)
        {
            tmpPiece[i]->coords[j][0] = pieces[i].coords[j][0];
            tmpPiece[i]->coords[j][1] = pieces[i].coords[j][1];
        }
    }
    tetris->tmpPiece = tmpPiece;
}

void tetris_playGame(Tetris *tetris, userInterface ui)
{
    srand(time(NULL));

    // On initialise l'interface (ouvrir Ncurses ou SDL)
    ui.functions->init_interface();

    homescreen(tetris, ui);

    // On lance le menu, il change l'état du jeu en fonction de ce qu'on fait, ça permet d'intéragir entre les états du jeu
    while (1)
    {
        if (tetris->state == MENU)
        {
            homescreen(tetris, ui);
        }
        else if (tetris->state == GAME)
        {
            game(tetris, ui);
        }
        else if (tetris->state == END)
        {
            endscreen(tetris, ui);
        }
        else if(tetris->state == CLOSE){
            // On ferme l'interface (fermer Ncurses ou SDL)
            ui.functions->close_interface();
            clear_tetris(tetris, ui);
            clear_pointeur_fct(ui);
            return;
        }
        else if (tetris->state == OPTION)
        {
            return;
        }
        else
        {
            perror("Erreur state\n");
            exit(EXIT_FAILURE);
        }
    }
}

void homescreen(Tetris *tetris, userInterface ui)
{

    ui.functions->home_page(tetris);

    // Il faut essayer de déplacer ce bout de code qui suit dans le home page de ncurses pour gérer les input ncurses qui sont différent de SDL
    // Maintenant on récupère l'input de l'utilisateur pour choisir le niveau
    /*char input;
    do
    {
        input = ui.functions->input(tetris);
        // On affiche le menu
        ui.functions->home_page(tetris);
    } while (input != '0' && input != '1' && input != '2' && input != '3' && input != '4' && input != '5' && input != '6' && input != '7' && input != '8' && input != '9');
    tetris->level = atoi(&input);
    */
    tetris->state = GAME;
}

void game(Tetris *tetris, userInterface ui)
{

    tetris->state = GAME;

    // je prend une piece aléatoire avec le get (memcpy etc), ça l'ajoute dans la grille
    get_piece(tetris);

    // On affiche le jeu et les infos du jeu
    ui.functions->display(tetris);
    ui.functions->display_info(tetris);

    char input;
    while (tetris->state != END)
    {

        // On récupère l'input selon l'interface (SDL ou NCurses)
        input = ui.functions->input(tetris);

        switch (input)
        {
        case 'q':
            move_left_piece(tetris);
            break;
        case 's':
        case 'x':
            if (!move_down_piece(tetris))
            {
                // Quand une pièce arrive à destination, on enleve les lignes pleines, on fait un
                // petit sleep  (voir détail fonction sleep) et on prend une nouvelle pièce
                refresh_board(tetris);
                delete_all_line(tetris);
                // sleep_NES(tetris);
                get_piece(tetris);
            }
            break;
        case 'd':
            move_right_piece(tetris);
            break;
        case 'a':
            rotate_left(tetris);
            break;
        case 'e':
            rotate_right(tetris);
            break;
        default:
            break;
        }

        refresh_board(tetris);
        ui.functions->display(tetris);
        ui.functions->display_info(tetris);
    }
}

void endscreen(Tetris *tetris, userInterface ui)
{

    tetris->state = END;

    FILE *f = fopen("data/highscore.txt", "a");
    if (!f)
    {
        perror("Erreur fopen(), impossible de sauvegarder le score\n");
        exit(EXIT_FAILURE);
    }

    ui.functions->end_screen(tetris, f);

    //Bout de code à faire dans le ncurses pour gérer les input
    /*char input;
    // Ecran de fin de partie (q pour quitter la partie et r pour rejouer)
    do
    {
        input = ui.functions->input(tetris);
        // On affiche la fin de partie
        ui.functions->end_screen(tetris, f);
    } while (input != 'q' && input != 'r');*/

    if (fclose(f))
    {
        perror("Erreur fclose()\n");
        exit(EXIT_FAILURE);
    }
}

PieceConfig *get_next_piece(Tetris *tetris)
{
    int randomIndex = rand() % 7;

    PieceConfig *nextPiece = (PieceConfig *)malloc(sizeof(PieceConfig));
    if (!nextPiece)
    {
        perror("Erreur Malloc (NextPiece) \n");
        exit(EXIT_FAILURE);
    }
    memcpy(nextPiece, tetris->tmpPiece[randomIndex], sizeof(PieceConfig));

    return nextPiece;
}

void update_stats(Tetris *tetris, char name)
{
    switch (name)
    {
    case 'T':
        tetris->pieceStats[0]++;
        break;
    case 'J':
        tetris->pieceStats[1]++;
        break;
    case 'Z':
        tetris->pieceStats[2]++;
        break;
    case 'O':
        tetris->pieceStats[3]++;
        break;
    case 'S':
        tetris->pieceStats[4]++;
        break;
    case 'L':
        tetris->pieceStats[5]++;
        break;
    case 'I':
        tetris->pieceStats[6]++;
        break;
    default:
        perror("Erreur update_stats()\n");
        exit(EXIT_FAILURE);
    }
}

void update_piece(Tetris *tetris, PieceConfig *piece)
{
    for (int ind = 0; ind < piece->num_cells; ind++)
    {
        int coord_x = piece->coords[ind][0];
        int coord_y = piece->coords[ind][1];
        if (coord_x >= tetris->line || coord_y < 0 || coord_y >= tetris->column)
        {
            // La pièce est en dehors des limites du plateau, donc on a perdu :(
            tetris->state = END;
            return;
        }
        if (tetris->board[coord_x][coord_y].isFull == true)
        {
            // La case est déjà occupée, donc on a perdu :(
            tetris->state = END;
            return;
        }
        else
        {
            tetris->board[coord_x][coord_y].isFull = true;
            tetris->board[coord_x][coord_y].c = piece->c;
        }
    }
}

// On ajoute une piece de tmpPiece dans le tableau
void get_piece(Tetris *tetris)
{
    if (!tetris->nextPiece)
    {
        tetris->nextPiece = get_next_piece(tetris);
    }

    tetris->nbBoardPiece++;

    tetris->boardPiece = (PieceConfig **)realloc(tetris->boardPiece, tetris->nbBoardPiece * sizeof(PieceConfig *));
    if (!tetris->boardPiece)
    {
        perror("Erreur realloc()\n");
        exit(EXIT_FAILURE);
    }

    tetris->boardPiece[tetris->nbBoardPiece - 1] = tetris->nextPiece;
    update_stats(tetris, tetris->nextPiece->name);
    update_piece(tetris, tetris->nextPiece);

    tetris->nextPiece = get_next_piece(tetris);
}

void restore_board_state(Tetris *tetris, bool temp_cells[][4][2])
{
    for (int i = 0; i < tetris->boardPiece[tetris->nbBoardPiece - 1]->num_cells; i++)
    {
        int curr_x = temp_cells[tetris->nbBoardPiece - 1][i][0];
        int curr_y = temp_cells[tetris->nbBoardPiece - 1][i][1];

        // Restaurer l'état initial de la cellule sur le plateau
        tetris->board[curr_x][curr_y].isFull = true;
    }
}

bool can_move(Tetris *tetris, int varX, int varY)
{
    // Stockage temporaire des cellules actuelles de la pièce
    bool temp_cells[tetris->nbBoardPiece][4][2];

    // Copie des cellules actuelles de la pièce avant déplacement
    for (int i = 0; i < tetris->boardPiece[tetris->nbBoardPiece - 1]->num_cells; i++)
    {
        int curr_x = tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][0];
        int curr_y = tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][1];

        temp_cells[tetris->nbBoardPiece - 1][i][0] = curr_x;
        temp_cells[tetris->nbBoardPiece - 1][i][1] = curr_y;
        if ((curr_x >= 0 && curr_x < tetris->line) && (curr_y >= 0 && curr_y < tetris->column))
        {
            tetris->board[curr_x][curr_y].isFull = false;
        }
    }

    // Vérification si les nouvelles coordonnées sont valides
    for (int i = 0; i < tetris->boardPiece[tetris->nbBoardPiece - 1]->num_cells; i++)
    {
        int coord_x = tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][0] + varX;
        int coord_y = tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][1] + varY;

        if (coord_x >= tetris->line || coord_y < 0 || coord_y >= tetris->column)
        {
            // Restaurer l'état initial du plateau
            restore_board_state(tetris, temp_cells);
            return false;
        }
        if ((coord_x >= 0 && coord_x < tetris->line) && (coord_y >= 0 && coord_y < tetris->column))
        {
            if (tetris->board[coord_x][coord_y].isFull)
            {
                // Restaurer l'état initial du plateau
                restore_board_state(tetris, temp_cells);
                return false;
            }
        }
    }

    // Restaurer l'état initial du plateau
    restore_board_state(tetris, temp_cells);
    return true;
}

bool move_down_piece(Tetris *tetris)
{
    if (!can_move(tetris, 1, 0))
    {
        // déplacement pas possible
        return false;
    }
    // On déplace la piece vers le bas
    for (int i = 0; i < tetris->boardPiece[tetris->nbBoardPiece - 1]->num_cells; i++)
    {
        tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][0]++;
    }
    return true;
}

bool move_left_piece(Tetris *tetris)
{
    if (!can_move(tetris, 0, -1))
    {
        // déplacement pas possible
        return false;
    }
    // On déplace la piece vers la gauche
    for (int i = 0; i < tetris->boardPiece[tetris->nbBoardPiece - 1]->num_cells; i++)
    {
        tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][1]--;
    }
    return true;
}

bool move_right_piece(Tetris *tetris)
{
    if (!can_move(tetris, 0, 1))
    {
        // déplacement pas possible
        return false;
    }
    // On déplace la piece vers la droite
    for (int i = 0; i < tetris->boardPiece[tetris->nbBoardPiece - 1]->num_cells; i++)
    {
        tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][1]++;
    }
    return true;
}

/*Si ça bug pour le 'I' : De base le pivot est à [2][0]*/
int get_pivot_X(PieceConfig *p)
{
    if ((p->name == 'J') || (p->name == 'T') || (p->name == 'Z') || (p->name == 'L') || (p->name == 'I'))
    {
        return p->coords[1][0];
    }
    else if (p->name == 'S')
    {
        return p->coords[0][0];
    }
    return -1;
}

int get_pivot_Y(PieceConfig *p)
{
    if ((p->name == 'J') || (p->name == 'T') || (p->name == 'Z') || (p->name == 'L') || (p->name == 'I'))
    {
        return p->coords[1][1];
    }
    else if (p->name == 'S')
    {
        return p->coords[0][1];
    }
    return -1;
}

// Fonction pour vérifier si la position spécifiée est la même que les anciennes coordonnées
bool is_same_as_old_coords(Tetris *tetris, int x, int y, int oldX, int oldY)
{
    for (int i = 0; i < tetris->boardPiece[tetris->nbBoardPiece - 1]->num_cells; i++)
    {
        if (x == tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][0] && y == tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][1])
        {
            return true;
        }
    }
    return false;
}
/*
Fonction qui permet de rotate une piece standard.
    Direction : -1 //Rotation Gauche
    Direction :  1 //Rotation Droites
*/
bool can_rotate(Tetris *tetris, int rotationDirection)
{
    int newCoords[4][2];

    int pivotX = get_pivot_X(tetris->boardPiece[tetris->nbBoardPiece - 1]);
    int pivotY = get_pivot_Y(tetris->boardPiece[tetris->nbBoardPiece - 1]);

    for (int i = 0; i < tetris->boardPiece[tetris->nbBoardPiece - 1]->num_cells; i++)
    {
        int oldX = tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][0];
        int oldY = tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][1];

        int deltaX = oldX - pivotX;
        int deltaY = oldY - pivotY;

        if (rotationDirection == 1)
        { // Rotation Droite
            newCoords[i][0] = pivotX + deltaY;
            newCoords[i][1] = pivotY - deltaX;
        }
        else if (rotationDirection == -1)
        { // Rotation Gauche
            newCoords[i][0] = pivotX - deltaY;
            newCoords[i][1] = pivotY + deltaX;
        }

        int coord_x = newCoords[i][0];
        int coord_y = newCoords[i][1];

        if (coord_x >= tetris->line || coord_y < 0 || coord_y >= tetris->column)
        {
            return false;
        }
        else if ((coord_x >= 0 && coord_x < tetris->line) && (coord_y >= 0 && coord_y < tetris->column))
        {
            if (tetris->board[coord_x][coord_y].isFull && !is_same_as_old_coords(tetris, coord_x, coord_y, oldX, oldY))
            {
                return false;
            }
        }
    }

    return true;
}

void rotate_left(Tetris *tetris)
{
    if ((!can_rotate(tetris, -1)) || (tetris->boardPiece[tetris->nbBoardPiece - 1]->name == 'O'))
    {
        return;
    }
    int pivotX = get_pivot_X(tetris->boardPiece[tetris->nbBoardPiece - 1]);
    int pivotY = get_pivot_Y(tetris->boardPiece[tetris->nbBoardPiece - 1]);

    for (int i = 0; i < tetris->boardPiece[tetris->nbBoardPiece - 1]->num_cells; i++)
    {
        int oldX = tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][0];
        int oldY = tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][1];
        int deltaX = oldX - pivotX;
        int deltaY = oldY - pivotY;

        // Mise à jour des coordonnées après la rotation à gauche
        tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][0] = pivotX - deltaY;
        tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][1] = pivotY + deltaX;
    }
}

void rotate_right(Tetris *tetris)
{
    if ((!can_rotate(tetris, 1)) || (tetris->boardPiece[tetris->nbBoardPiece - 1]->name == 'O'))
    {
        return;
    }
    int pivotX = get_pivot_X(tetris->boardPiece[tetris->nbBoardPiece - 1]);
    int pivotY = get_pivot_Y(tetris->boardPiece[tetris->nbBoardPiece - 1]);

    for (int i = 0; i < tetris->boardPiece[tetris->nbBoardPiece - 1]->num_cells; i++)
    {
        int oldX = tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][0];
        int oldY = tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][1];
        int deltaX = oldX - pivotX;
        int deltaY = oldY - pivotY;

        // Mise à jour des coordonnées après la rotation à droite
        tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][0] = pivotX + deltaY;
        tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][1] = pivotY - deltaX;
    }
}

void refresh_board(Tetris *tetris)
{
    // On efface le tableau
    for (int i = 0; i < tetris->line; i++)
    {
        for (int j = 0; j < tetris->column; j++)
        {
            tetris->board[i][j].isFull = false;
            tetris->board[i][j].c = NOTHING; // pas oublier NOTHING
        }
    }
    for (int i = 0; i < tetris->nbBoardPiece; i++)
    {
        PieceConfig *p = tetris->boardPiece[i];
        for (int ind = 0; ind < p->num_cells; ind++)
        {
            int coord_x = p->coords[ind][0];
            int coord_y = p->coords[ind][1];
            if ((coord_x >= 0 && coord_x < tetris->line) && (coord_y >= 0 && coord_y < tetris->column))
            {
                tetris->board[coord_x][coord_y].isFull = true;
                tetris->board[coord_x][coord_y].c = p->c;
            }
        }
    }
}

bool is_full_line(Tetris *tetris, int ligne)
{
    for (int i = 0; i < tetris->column; i++)
    {
        if (!tetris->board[ligne][i].isFull)
        {
            return false;
        }
    }
    return true;
}

void delete_line(Tetris *tetris, int ligne)
{
    // Mettre à jour les coordonnées des cellules dans les pièces
    for (int i = 0; i < tetris->nbBoardPiece; i++)
    {
        PieceConfig *p = tetris->boardPiece[i];
        for (int ind = 0; ind < p->num_cells; ind++)
        {
            if (p->coords[ind][0] == ligne)
            {
                // Si la cellule est sur la ligne supprimée, mettre ses coordonnées à -1
                p->coords[ind][0] = -1;
                p->coords[ind][1] = -1;
            }
            else if (p->coords[ind][0] < ligne)
            {
                // Si la cellule est au-dessus de la ligne supprimée, décrémenter sa coordonnée x
                p->coords[ind][0]++;
            }
        }
    }
    refresh_board(tetris);
}

void delete_all_line(Tetris *tetris)
{
    int cpt = 0;
    for (int i = tetris->line - 1; i >= 0; i--)
    {
        if (is_full_line(tetris, i))
        {
            delete_line(tetris, i);
            cpt++;
            tetris->nbLines++;
            if (tetris->nbLines % 10 == 0)
            {
                tetris->level++;
            }
            i++;
        }
    }
    switch (cpt)
    {
    case 0:
        break;
    case 1:
        add_score(tetris, 40);
        break;
    case 2:
        add_score(tetris, 100);
        break;
    case 3:
        add_score(tetris, 300);
        break;
    case 4:
        add_score(tetris, 1200);
        break;
    default:
        perror("Error delete_all_line()\n");
        exit(EXIT_FAILURE);
    }
}

void add_score(Tetris *tetris, int score_line)
{
    tetris->score += score_line * (tetris->level + 1);
}

void sleep_NES(Tetris *tetris)
{
    int nb = 0;
    int frame = 0;
    // On compte le nombre de ligne qui contienne au minimum 1 cellule pleine
    for (int i = 0; i < tetris->line; i++)
    {
        for (int j = 0; j < tetris->column; j++)
        {
            if (tetris->board[i][j].isFull)
            {
                nb++;
                break;
            }
        }
    }
    // Calcule des frames qu'on va devoir attendre
    if (nb <= 2)
    {
        frame = 10 * nb;
    }
    else
    {
        frame = 10 * 2;
        nb -= 2;
        while (nb >= 4)
        {
            frame += 2;
            nb -= 4;
        }
    }

    // Transformation des frames en milliseconde
    int sleep_time = frame * 1000 / 60;

    // On sleep le programme (on convertit de milliseconde en microseconde)
    usleep(sleep_time * 1000);
}

void clear_board(Tetris *tetris)
{
    if (!tetris->board)
        return;
    for (int i = 0; i < tetris->line; i++)
    {
        free(tetris->board[i]);
    }
    free(tetris->board);
}

void clear_boardPiece(Tetris *tetris)
{
    if (!tetris->boardPiece)
        return;
    for (int i = 0; i < tetris->nbBoardPiece; i++)
    {
        free(tetris->boardPiece[i]);
    }
    free(tetris->boardPiece);
}

void clear_tmpPiece(Tetris *tetris)
{
    if (!tetris->tmpPiece)
        return;
    for (int i = 0; i < 7; i++)
    {
        free(tetris->tmpPiece[i]);
    }
    free(tetris->tmpPiece);
}

void clear_pointeur_fct(userInterface ui)
{
    free(ui.functions);
}

void clear_tetris(Tetris *t, userInterface ui)
{
    clear_board(t);
    clear_boardPiece(t);
    clear_tmpPiece(t);
    free(t->nextPiece);
    free(t);
}