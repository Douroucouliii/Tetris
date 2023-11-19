#include "modele.h"
#include "textInterface.h"

void affiche_text(Tetris* tetris){
    printf("┌");
    for (int i = 0; i < WIDTH * 2; i++) {
        printf("─");
    }
    printf("┐\n");
    for (int i = 0; i < HEIGHT; i++) {
        printf("│");
        for (int j = 0; j < WIDTH; j++) {
            if (!tetris->board[i][j].isFull)
                printf("  ");
            else {
                // Utiliser des codes ANSI de couleur pour chaque pièce
                switch (tetris->board[i][j].c) {
                    case NOTHING:
                        printf("  ");
                        break;
                    case CYAN:
                        printf("\033[0;36m[]\033[0m");
                        break;
                    case YELLOW:
                        printf("\033[1;33m[]\033[0m");
                        break;
                    case PURPLE:
                        printf("\033[0;35m[]\033[0m");
                        break;
                    case ORANGE:
                        printf("\033[0;33m[]\033[0m");
                        break;
                    case BLUE:
                        printf("\033[0;34m[]\033[0m");
                        break;
                    case RED:
                        printf("\033[0;31m[]\033[0m");
                        break;
                    case GREEN:
                        printf("\033[0;32m[]\033[0m");
                        break;
                    default:
                        printf("[]");
                }
            }
        }
        printf("│\n");
    }
    printf("└");
    for (int i = 0; i < WIDTH * 2; i++) {
        printf("─");
    }
    printf("┘\n\n");
    return;
}