#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "tetris.h"

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
    tetris->line_until_first_level_up = 10;
    tetris->nextPiece = NULL;

    for (int i = 0; i < 7; i++)
    {
        tetris->pieceStats[i] = 0;
    }

    tetris->file = fopen("data/highscore.txt", "r");
    if (!tetris->file)
    {
        perror("Erreur fopen(), impossible de sauvegarder le score\n");
        exit(EXIT_FAILURE);
    }

    // Initialisation des highscores par rapport à tetris->file
    init_highscore(tetris);

    fclose(tetris->file);

    tetris->isPanic = false;

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

void init_highscore(Tetris *tetris)
{
    int numHighscores = 10;
    tetris->highscores = (Highscore *)malloc(numHighscores * sizeof(Highscore));

    if (!tetris->highscores)
    {
        fprintf(stderr, "Erreur d'allocation de mémoire pour les highscores.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numHighscores; i++)
    {
        tetris->highscores[i].name = (char *)malloc(sizeof(char) * 21);
        if (!tetris->highscores[i].name)
        {
            fprintf(stderr, "Erreur d'allocation de mémoire pour le nom.\n");
            clear_highscores(tetris);
            exit(EXIT_FAILURE);
        }
        tetris->highscores[i].score = 0; // Initialise le score à zéro
    }

    for (int i = 0; i < numHighscores; i++)
    {
        int pos = 0;
        char c;
        // Lecture du nom
        while ((c = fgetc(tetris->file)) != ',' && c != EOF)
        {
            tetris->highscores[i].name[pos++] = c;
        }
        tetris->highscores[i].name[pos] = '\0';

        // Lecture du score
        fscanf(tetris->file, "%d", &tetris->highscores[i].score);

        // On passe le saut à la ligne
        if (i < numHighscores - 1)
        {
            while ((c = fgetc(tetris->file)) != '\n' && c != EOF)
            {
            }
        }
    }
}

void tetris_playGame(Tetris *tetris, userInterface nCurses, userInterface SDL)
{
    srand(time(NULL));

    // SDL de base
    userInterface ui = SDL;

    // On initialise l'interface (ouvrir Ncurses ou SDL)
    if (tetris->state != GAME)
        ui.functions->init_interface();

    // On lance le menu
    homescreen(tetris, ui);
    // On actualise la variable pour savoir au bout de combien de temps le premier level up se fera
    line_until_first_level_up(tetris);

    // On lance le menu, il change l'état du jeu en fonction de ce qu'on fait, ça permet d'intéragir entre les états du jeu
    while (1)
    {
        if (tetris->state == MENU)
        {
            if (ui.functions->play_sound)
                ui.functions->play_sound(6);
            homescreen(tetris, ui);
            line_until_first_level_up(tetris);
        }
        else if (tetris->state == GAME)
        {
            if (ui.functions->play_sound)
                ui.functions->play_sound(6);
            game(tetris, &ui, nCurses, SDL);
        }
        else if (tetris->state == END)
        {
            endscreen(tetris, ui);
        }
        else if (tetris->state == CLOSE)
        {
            // On ferme l'interface (fermer Ncurses ou SDL)
            ui.functions->close_interface();
            // il faut clear les 2 ui et non pas juste la variable ui ;)
            clear_pointeur_fct(SDL);
            clear_pointeur_fct(nCurses);
            clear_tetris(tetris);
            free(tetris);
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
}

void game(Tetris *tetris, userInterface *ui, userInterface nCurses, userInterface SDL)
{
    tetris->state = GAME;

    // je prends une pièce aléatoire avec le get (memcpy etc), ça l'ajoute dans la grille
    get_piece(tetris);

    // On affiche le jeu et les infos du jeu
    ui->functions->display(tetris);
    ui->functions->display_info(tetris);

    // Nombre de frames avant de faire tomber la pièce d'une case
    int fallTime = delay(tetris);
    // Nombre de frames avant de faire apparaître la pièce suivante
    int holdTime = 0;

    char input;

    struct timespec ts1, ts2;
    timespec_get(&ts1, TIME_UTC);

    while (tetris->state != END)
    {
        // On récupère l'input selon l'interface (SDL ou NCurses)
        input = ui->functions->input(tetris);

        if (holdTime > 0)
        {
            holdTime--;
            if (holdTime == 0)
                get_piece(tetris);
        }
        else
        {
            switch (input)
            {
            case 'q':
                if (move_left_piece(tetris) && ui->functions->play_sound)
                    ui->functions->play_sound(0);
                break;
            case 's':
                if (!move_down_piece(tetris))
                {
                    // Quand une pièce arrive à destination, on enlève les lignes pleines, on fait un
                    // petit sleep  (voir détail fonction sleep) et on prend une nouvelle pièce
                    // On vérifie si le joueur est proche de la mort pour jouer l'autre musique
                    refresh_board(tetris);
                    delete_all_line(tetris, *ui);
                    if (ui->functions->play_sound)
                        ui->functions->play_sound(7);
                    is_panic(tetris);
                    // Nombre de frame à attendre avant d'appeler la fonction get_piece
                    holdTime = frame_sleep_NES(tetris);
                }
                fallTime = delay(tetris);
                break;
            case 'd':
                if (move_right_piece(tetris) && ui->functions->play_sound)
                    ui->functions->play_sound(0);
                break;
            case 'a':
                rotate_left(tetris);
                if (ui->functions->play_sound)
                    ui->functions->play_sound(1);
                break;
            case 'e':
                rotate_right(tetris);
                if (ui->functions->play_sound)
                    ui->functions->play_sound(1);
                break;
            case 'm':
                ui->functions->close_interface();

                // Change interface
                if (strcmp(ui->instance, "NCurses") == 0)
                {
                    *ui = SDL;
                }
                else
                {
                    *ui = nCurses;
                }

                // Initialise la nouvelle interface
                ui->functions->init_interface();
                break;
            default:
                break;
            }

            fallTime--;
            if (fallTime == 0)
            {
                if (!move_down_piece(tetris))
                {
                    // Meme chose que le case 's'
                    refresh_board(tetris);
                    delete_all_line(tetris, *ui);
                    if (ui->functions->play_sound)
                        ui->functions->play_sound(7);
                    is_panic(tetris);
                    // Nombre de frame à attendre avant d'appeler la fonction get_piece
                    holdTime = frame_sleep_NES(tetris);
                }
                fallTime = delay(tetris);
            }
        }

        refresh_board(tetris);
        ui->functions->display(tetris);
        ui->functions->display_info(tetris);

        // Setup pour 60 fps (delta time)
        timespec_get(&ts2, TIME_UTC);
        int delta = ((ts2.tv_sec - ts1.tv_sec) * 1000000 + (ts2.tv_nsec - ts1.tv_nsec) / 1000.0);
        if (delta < 16667)
        {
            usleep(16667 - delta);
        }
        timespec_get(&ts1, TIME_UTC);
    }
}

void endscreen(Tetris *tetris, userInterface ui)
{

    tetris->state = END;
    if (ui.functions->play_sound)
        ui.functions->play_sound(8);
    // Attendre 1 sec avant de mettre l'écran de fin (effet sonnore)
    sleep(1);

    ui.functions->end_screen(tetris);
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

void restore_board_state(Tetris *tetris, bool temp_cells[4][2])
{
    for (int i = 0; i < tetris->boardPiece[tetris->nbBoardPiece - 1]->num_cells; i++)
    {
        int curr_x = temp_cells[i][0];
        int curr_y = temp_cells[i][1];

        // Restaurer l'état initial de la cellule sur le plateau
        if ((curr_x >= 0 && curr_x < tetris->line) && (curr_y >= 0 && curr_y < tetris->column))
        {
            tetris->board[curr_x][curr_y].isFull = true;
        }
    }
}

bool can_move(Tetris *tetris, int varX, int varY)
{
    // Stockage temporaire des cellules actuelles de la pièce
    bool temp_cells[4][2];

    // Copie des cellules actuelles de la pièce avant déplacement
    for (int i = 0; i < tetris->boardPiece[tetris->nbBoardPiece - 1]->num_cells; i++)
    {
        int curr_x = tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][0];
        int curr_y = tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][1];

        temp_cells[i][0] = curr_x;
        temp_cells[i][1] = curr_y;

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
    refresh_board(tetris);
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
    refresh_board(tetris);
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

void delete_all_line(Tetris *tetris, userInterface ui)
{
    int cpt = 0;
    for (int i = tetris->line - 1; i >= 0; i--)
    {
        if (is_full_line(tetris, i))
        {
            delete_line(tetris, i);
            cpt++;
            tetris->nbLines++;
            // Si c'est le premier level up, on se réfère à "line_until_first_level_up", ensuite on passe de niveau toutes les 10 lignes
            if (tetris->line_until_first_level_up != -1 && (tetris->nbLines % tetris->line_until_first_level_up == 0))
            {
                tetris->level++;
                switch_color(tetris);
                if (ui.functions->play_sound)
                    ui.functions->play_sound(4);
                tetris->line_until_first_level_up = -1;
            }
            else if (tetris->line_until_first_level_up == -1 && (tetris->nbLines % 10 == 0))
            {
                tetris->level++;
                switch_color(tetris);
                if (ui.functions->play_sound)
                    ui.functions->play_sound(4);
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
    if (cpt == 4 && ui.functions->play_sound)
        ui.functions->play_sound(3);
    else if (cpt > 0 && ui.functions->play_sound)
        ui.functions->play_sound(2);
}

void switch_color(Tetris *tetris)
{
    // Change les couleurs des pieces tmp de notre jeu, et change toutes les couleurs des pieces déjà presente dans le jeu
    // Toujours couleurs différentes, attention à ne pas piocher NOTHING
    color dejaPris[7];
    int cpt = 1;

    dejaPris[0] = rand() % 7 + 1;
    while (cpt < 7)
    {
        color c = rand() % 7 + 1;
        int isDifferent = 1;
        for (int i = 0; i < cpt; i++)
        {
            if (dejaPris[i] == c)
            {
                isDifferent = 0;
                break;
            }
        }
        if (isDifferent)
        {
            dejaPris[cpt] = c;
            cpt++;
        }
    }
    for (int i = 0; i < 7; i++)
    {
        tetris->tmpPiece[i]->c = dejaPris[i];
    }
    // Si les pieces matche avec le nom, on remplace la couleur
    for (int i = 0; i < tetris->nbBoardPiece; i++)
    {
        PieceConfig *p = tetris->boardPiece[i];
        for (int j = 0; j < 7; j++)
        {
            if (p->name == tetris->tmpPiece[j]->name)
            {
                p->c = tetris->tmpPiece[j]->c;
            }
        }
    }
}

void add_score(Tetris *tetris, int score_line)
{
    tetris->score += score_line * (tetris->level + 1);
}

int frame_sleep_NES(Tetris *tetris)
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
        frame = 20;
        nb -= 2;
        while (nb >= 4)
        {
            frame += 2;
            nb -= 4;
        }
    }

    return frame;
}

void is_panic(Tetris *tetris)
{
    // Compte le nombre de ligne pour lesquelles on a au moins 6 cellules pleines
    int nb = 0;
    for (int i = 0; i < tetris->line; i++)
    {
        int cpt = 0;
        for (int j = 0; j < tetris->column; j++)
        {
            if (tetris->board[i][j].isFull)
            {
                cpt++;
            }
        }
        if (cpt >= 4)
        {
            nb++;
        }
    }

    // Si 11 des lignes sont concernées, on passe en mode panic
    if (nb >= 11)
    {
        tetris->isPanic = true;
    }
    else
    {
        tetris->isPanic = false;
    }
}

int delay(Tetris *tetris)
{
    int frames;

    // le nombre de frames/cellules de grille par pièce en fonction du niveau
    switch (tetris->level)
    {
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
    case 10:
    case 11:
    case 12:
        frames = 5;
        break;
    case 13:
    case 14:
    case 15:
        frames = 4;
        break;
    case 16:
    case 17:
    case 18:
        frames = 3;
        break;
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
        frames = 2;
        break;
    // Niveau >28 : Killscreen : seul mattéo pourrait survivre ici mais bon on sait jamais :)
    default:
        frames = 1; // C'est la mort
        break;
    }

    return frames;
}

void line_until_first_level_up(Tetris *tetris)
{
    if (tetris->level >= 0 && tetris->level <= 9)
    {
        tetris->line_until_first_level_up = (tetris->level + 1) * 10;
    }
    else if (tetris->level >= 10 && tetris->level <= 19)
    {
        int var1 = (tetris->level - 5) * 10;
        if (var1 > 100)
            tetris->line_until_first_level_up = var1;
        else
            tetris->line_until_first_level_up = 100;
    }
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
    tetris->board = NULL;
}

void clear_boardPiece(Tetris *tetris)
{

    for (int i = 0; i < tetris->nbBoardPiece; i++)
    {
        free(tetris->boardPiece[i]);
    }
    free(tetris->boardPiece);
    tetris->boardPiece = NULL;
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
    tetris->tmpPiece = NULL;
}

void clear_highscores(Tetris *tetris)
{
    int numHighscores = 10;

    if (tetris->highscores != NULL)
    {
        for (int i = 0; i < numHighscores; i++)
        {
            if (tetris->highscores[i].name != NULL)
            {
                free(tetris->highscores[i].name);
                tetris->highscores[i].name = NULL;
            }
        }
        free(tetris->highscores);
        tetris->highscores = NULL;
    }
}

void clear_pointeur_fct(userInterface ui)
{
    free(ui.functions);
}

void clear_tetris(Tetris *t)
{
    clear_board(t);
    clear_boardPiece(t);
    clear_tmpPiece(t);
    clear_highscores(t);

    if (t->nextPiece != NULL)
    {
        free(t->nextPiece);
        t->nextPiece = NULL;
    }
}