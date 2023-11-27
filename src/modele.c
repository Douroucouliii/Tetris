#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "modele.h"

PieceConfig pieces[7] = {
    {'I', 4, {{0, 3}, {0, 4}, {0, 5}, {0, 6}}, RED},
    {'O', 4, {{0, 4}, {0, 5}, {1, 4}, {1, 5}}, ORANGE},
    {'T', 4, {{0, 4}, {0, 5}, {0, 6}, {1, 5}}, GREEN},
    {'J', 4, {{0, 4}, {0, 5}, {0, 6}, {1, 6}}, CYAN},
    {'L', 4, {{0, 4}, {0, 5}, {0, 6}, {1, 4}}, YELLOW},
    {'Z', 4, {{0, 4}, {0, 5}, {1, 5}, {1, 6}}, BLUE},
    {'S', 4, {{0, 5}, {0, 6}, {1, 4}, {1, 5}}, PURPLE}};

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

    tetris->end = false;

    tetris->nbLines = 0;
    tetris->score = 0;
    tetris->level = 0;
    tetris->nextPiece = NULL;

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
    ui.fonctions->init_interface();

    // je prend une piece aléatoire avec le get (memcpy etc), ça l'ajoute dans la grille
    get_piece(tetris);

    // On affiche le jeu
    ui.fonctions->display(tetris);

    // Boucle pour jouer à notre jeu
    while (!tetris->end)
    {
        // On récupère l'input selon l'interface (SDL ou NCurses)
        char input = ui.fonctions->input();
        if (input == 'q')
        {
            move_left_piece(tetris);
        }
        if (input == 's')
        {
            // Si la piece ne peut pas aller plus bas, alors on génère une nouvelle piece
            if (!move_down_piece(tetris))
            {
                // La ligne ne s'éfface qu'apres que la prochaine piece soit descendue, bizarre
                refresh_board(tetris);
                delete_all_line(tetris);
                get_piece(tetris);
            }
        }
        if (input == 'd')
        {
            move_right_piece(tetris);
        }
        if (input == 'a')
        {
            rotate_left(tetris);
        }
        if (input == 'e')
        {
            rotate_right(tetris);
        }
        refresh_board(tetris);
        ui.fonctions->display(tetris);
    }

    printf("\nTu as perdu la partie, dommage !\n\n");

    // On ferme l'interface (fermer Ncurses ou SDL)
    ui.fonctions->close_interface();

    clear_all(tetris, ui);
    free(tetris);
    return;
}

PieceConfig *get_next_piece(Tetris *tetris)
{
    int randomIndex = rand() % 7;

    PieceConfig *nextPiece = (PieceConfig*)malloc(sizeof(PieceConfig));
    if(!nextPiece){
        perror("Erreur Malloc (NextPiece) \n");
        exit(EXIT_FAILURE);
    }  
    memcpy(nextPiece,tetris->tmpPiece[randomIndex],sizeof(PieceConfig));

    nextPiece->c=(color)(rand() % 7 + 1); //C'est pour pas avoir NOTHING
    return nextPiece;
}

void update_piece(Tetris *tetris,PieceConfig *piece)
{
     for (int ind = 0; ind < piece->num_cells; ind++)
    {
        int coord_x = piece->coords[ind][0];
        int coord_y = piece->coords[ind][1];
        if (coord_x < 0 || coord_x >= tetris->line || coord_y < 0 || coord_y >= tetris->column)
        {
            // La pièce est en dehors des limites du plateau, donc on a perdu :(
            tetris->end = true;
            return;
        }
        if (tetris->board[coord_x][coord_y].isFull == true)
        {
            // La case est déjà occupée, donc on a perdu :(
            tetris->end = true;
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
    //Si la pièce suivante n'est pas initialisé, on en génère une.
    if( tetris->nextPiece == NULL ){
        tetris->nextPiece = get_next_piece(tetris);
    }

    // on memcpy sur une pièce de tmpPiece aléatoirement et on l'ajoute dans le table des pieces

    PieceConfig *newPiece = (PieceConfig *)malloc(sizeof(PieceConfig));
    if (!newPiece)
    {
        perror("Erreur malloc()\n");
        exit(EXIT_FAILURE);
    }
    memcpy(newPiece,tetris->nextPiece, sizeof(PieceConfig));
    
    tetris->nbBoardPiece++;
    if (!tetris->boardPiece)
    {
        tetris->boardPiece = (PieceConfig **)malloc(tetris->nbBoardPiece * sizeof(PieceConfig *));
    }
    else
    {
        tetris->boardPiece = (PieceConfig **)realloc(tetris->boardPiece, tetris->nbBoardPiece * sizeof(PieceConfig *));
    }
    if (!tetris->boardPiece)
    {
        perror("Erreur malloc()/realloc()\n");
        exit(EXIT_FAILURE);
    }
    tetris->boardPiece[tetris->nbBoardPiece - 1] = newPiece;
    
    //On met à jour la grille 
    update_piece(tetris,newPiece);

    free(tetris->nextPiece);
    tetris->nextPiece = get_next_piece(tetris);
}

void copy_piece_cells(Tetris *tetris, bool*** temp_cells) {
    int piece_index = tetris->nbBoardPiece - 1;

    temp_cells[piece_index] = (bool**)malloc(4 * sizeof(bool*));
    for (int i = 0; i < 4; i++) {
        temp_cells[piece_index][i] = (bool*)malloc(2 * sizeof(bool));
    }

    for (int i = 0; i < tetris->boardPiece[piece_index]->num_cells; i++) {
        int curr_x = tetris->boardPiece[piece_index]->coords[i][0];
        int curr_y = tetris->boardPiece[piece_index]->coords[i][1];
        temp_cells[piece_index][i][0] = curr_x;
        temp_cells[piece_index][i][1] = curr_y;
        tetris->board[curr_x][curr_y].isFull = false;
    }
}


void free_temp_cells(Tetris *tetris,bool*** temp_cells) {
    // Libération de la mémoire allouée pour le tableau temporaire
    for (int i = 0; i < 4; i++) {
        free(temp_cells[tetris->nbBoardPiece - 1][i]);
    }
    free(temp_cells[tetris->nbBoardPiece - 1]);
}


/*
varX et varY : l'orientation de la piece ( Bas , Gauche , Droite , Haut )
Cette fonction ressor un type boolean qui nous permet de savoir si nous pouvons ou non aller dans une direction.
*/
bool can_move(Tetris *tetris, int varX, int varY)
{
    // stockage temporaire des cellules actuelles de la pièce
    bool*** temp_cells = (bool***)malloc(tetris->nbBoardPiece * sizeof(bool**));

    copy_piece_cells(tetris,temp_cells);
    // vérification si les nouvelles coordonnées sont valides
    for (int i = 0; i < tetris->boardPiece[tetris->nbBoardPiece - 1]->num_cells; i++)
    {
        int coord_x = tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][0] + varX;
        int coord_y = tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][1] + varY;

        if (coord_x < 0 || coord_x >= tetris->line || coord_y < 0 || coord_y >= tetris->column || tetris->board[coord_x][coord_y].isFull)
        {
            // restaurer l'état initial du plateau
            for (int j = 0; j < tetris->boardPiece[tetris->nbBoardPiece - 1]->num_cells; j++)
            {
                int curr_x = temp_cells[tetris->nbBoardPiece - 1][j][0];
                int curr_y = temp_cells[tetris->nbBoardPiece - 1][j][1];
                tetris->board[curr_x][curr_y].isFull = true;
            }
            free_temp_cells(tetris,temp_cells);
            return false;
        }
    }

    // restaurer l'état initial du plateau
    for (int i = 0; i < tetris->boardPiece[tetris->nbBoardPiece - 1]->num_cells; i++)
    {
        int curr_x = temp_cells[tetris->nbBoardPiece - 1][i][0];
        int curr_y = temp_cells[tetris->nbBoardPiece - 1][i][1];
        tetris->board[curr_x][curr_y].isFull = true;
    }
    free_temp_cells(tetris,temp_cells);
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
        if (rotationDirection == 1)
        { // Rotation Droite
            newCoords[i][0] = pivotX - (oldY - pivotY);
            newCoords[i][1] = pivotY + (oldX - pivotX);
        }
        else if (rotationDirection == -1)
        { // Rotation Gauche
            newCoords[i][0] = pivotX + (oldY - pivotY);
            newCoords[i][1] = pivotY - (oldX - pivotX);
        }
        int coord_x = newCoords[i][0];
        int coord_y = newCoords[i][1];
        if (coord_x < 0 || coord_x >= tetris->line || coord_y < 0 || coord_y >= tetris->column)
        {
            return false;
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
        // Mise à jour des coordonnées après la rotation à gauche
        tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][0] = pivotX + (oldY - pivotY);
        tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][1] = pivotY - (oldX - pivotX);
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
        // Mise à jour des coordonnées après la rotation à gauche
        tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][0] = pivotX + (oldY - pivotY);
        tetris->boardPiece[tetris->nbBoardPiece - 1]->coords[i][1] = pivotY - (oldX - pivotX);
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
            if (!(coord_x == -1 || coord_y == -1))
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
    free(ui.fonctions);
}

void clear_all(Tetris *t, userInterface ui)
{
    clear_board(t);
    clear_boardPiece(t);
    clear_tmpPiece(t);
    clear_pointeur_fct(ui);
}