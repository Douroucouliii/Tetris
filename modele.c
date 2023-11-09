//Faudra pas oublier les commentaires :D  (attendons le cours dessus pour les faire plus propre)

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

enum color{NOTHING, CYAN, YELLOW, PURPLE, ORANGE, BLUE, RED, GREEN };
typedef enum color color;

typedef struct{
    unsigned i : 12; // Coordonnée x de la cellule
    unsigned j : 12; // Coordonnée y de la cellule
    bool isFull;     // Boolean si la cell est occupée par une pièce
    color c;         // Couleur de la cellule
}cell;

typedef struct {
    char *nom;    // Nom de la pièce
    cell p[4];    // Pièce (tableau de 4 cellules)
    color c;      // Couleur de la pièce
} piece;

const char HAUTEUR = 20;
const char LARGEUR = 10;

piece **init_tmp_piece() {
    char *noms_pieces[] = {"I", "O", "T", "L", "J", "Z", "S"};
    piece **tmpPiece = (piece **)malloc(7 * sizeof(piece *));
    if (!tmpPiece) {
        perror("Erreur malloc()\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 7; i++) {
        tmpPiece[i] = (piece *)malloc(sizeof(piece));
        if (!tmpPiece[i]) {
            perror("Erreur malloc()\n");
            exit(EXIT_FAILURE);
        }
        tmpPiece[i]->nom = noms_pieces[i];
    }

    return tmpPiece;
}

void clear(cell **c) {
    if (!c) return;

    for (int i = 0; i < HAUTEUR; i++) {
        free(c[i]);
    }
    free(c);
}

int main() {
    srand(time(NULL));

    // Tableau de pointeur de pièce (les 7 pièces du jeu), qui serviront pour memcpy. Évite de devoir regénérer une pièce à chaque fois.
    piece **tmpPiece = init_tmp_piece();

    // Tableau de pointeur de pièce qui stocke toutes les pièces qui ont été dans le jeu.
    piece **boardPiece = NULL;

    // Tableau de cellule, c'est notre jeu
    cell **board = (cell **)malloc(HAUTEUR * sizeof(cell *));
    if (!board) {
        perror("Erreur malloc()\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < HAUTEUR; i++) {
        board[i] = (cell *)malloc(LARGEUR * sizeof(cell));
        if (!board[i]) {
            perror("Erreur malloc()\n");
            exit(EXIT_FAILURE);
        }
        for (int j = 0; j < LARGEUR; j++) {
            board[i][j].i = i;
            board[i][j].j = j;
            board[i][j].isFull = false;
            board[i][j].c = NOTHING;
        }
    }

    // Afficher le plateau
    for (int i = 0; i < HAUTEUR; i++) {
        for (int j = 0; j < LARGEUR; j++) {
            if (!board[i][j].isFull)
                printf(" ");
            else
                printf("#");
        }
        printf("\n");
    }
    printf("\n");

    clear(board);
    for (int i = 0; i < 7; i++) {
        free(tmpPiece[i]);
    }
    free(tmpPiece);

    return EXIT_SUCCESS;
}
