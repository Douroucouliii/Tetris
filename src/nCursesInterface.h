#pragma once

#include "modele.h"

void init_nCurses();
void close_nCurses();
void display_nCurses(Tetris *);
char input_nCurses();
void display_info_nCurses(Tetris *);
void menu_nCurses(Tetris *);