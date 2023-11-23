#include "modele.h"
#include "textInterface.h"

void init_text()
{
    return;
}

void close_text()
{
    return;
}

char input_text()
{
    char car;
    do
    {
        scanf("%c", &car);
    } while (car != 'q' && car != 's' && car != 'd' && car != 'a' && car != 'e');
    return car;
}

void display_text(Tetris *tetris)
{
    printf("┌");
    for (int i = 0; i < tetris->colonne * 2; i++)
    {
        printf("─");
    }
    printf("┐\n");
    for (int i = 0; i < tetris->ligne; i++)
    {
        printf("│");
        for (int j = 0; j < tetris->colonne; j++)
        {
            if (!tetris->board[i][j].isFull)
                printf("  ");
            else
            {
                // Utiliser des codes ANSI de couleur pour chaque pièce
                switch (tetris->board[i][j].c)
                {
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
    for (int i = 0; i < tetris->colonne * 2; i++)
    {
        printf("─");
    }
    printf("┘\n\n");
    return;
}