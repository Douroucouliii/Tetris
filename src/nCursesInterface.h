#pragma once

#include "modele.h"

void init_nCurses();
void close_nCurses();
void display_nCurses(Tetris *);
int delay(int);
char input_nCurses(Tetris *);
void display_info_nCurses(Tetris *);
void home_page_nCurses(Tetris *);
void end_screen_nCurses(Tetris *, FILE*);
