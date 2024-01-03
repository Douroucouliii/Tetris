#pragma once

/**
 * @file sdlInterface.h
 * @author Perin Clément (https://github.com/Douroucouliii)
 * @author Mackowiak Carole (https://github.com/Carole-prog)
 * @brief Affichage de la Vue du Tetris en SDL
 * @date 2023-11-24
 */

#include "tetris.h"
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"

/**
 * @brief C'est une structure qui permet d'imiter le comportement d'un bouton.
 *
 */
typedef struct
{
    SDL_Rect rect; /**< Rectangle pour la position du bouton sur l'écran*/
    char *text;    /**< Le texte que l'on va afficher sur le bouton*/
    int selected;  /**< soit 0 ou 1 ( 0 pour non selectionner et 1 pour selectionner )*/
} Button;

/**
 * @brief Cette fonction permet d'initialiser la vue SDL, ses deux font, son audio, son icone ainsi que le background récurrent.
 * @param void
 * @return void
 * @see close_SDL()
 * @see init_img_textures()
 * @see init_audio_SDL()
 * @see get_background()
 * @see set_icon()
 */
void init_SDL();

/**
 * @brief Cette fonction permet d'initialiser qu'une seule fois les textures de cases de notre Tetris.
 * @param void
 * @return void
 * @see get_image_path()
 * @see color_to_string()
 * @see get_indice_by_color()
 */
void init_img_textures();

/**
 * @brief Cette fonction permet d'initialiser le système d'audio de SDL, le nombre de canaux, le volume de la musique et elle appelle la fonction init_music_sound().
 * @param void
 * @return void
 * @see init_music_sound()
 * @see close_SDL()
 */
void init_audio_SDL();

/**
 * @brief Cette fonction permet d'attribuer à nos deux tableaux statiques musics et sounds leurs audio.
 * @param void
 * @return void
 */
void init_music_sound();

/**
 * @brief Cette fonction permet d'attribuer à notre fenêtre une icône
 * @param void
 * @return void
 */
void set_icon();

/**
 * @brief Cette fonction retourne la texture du background demander par rapport à son nom et la redimensione à la bonne taille de l'écran.
 * @param char* - Nom de l'image (  "fond" , "menu" )
 * @return SDL_Texture*
 * @see get_image_path()
 */
SDL_Texture *get_background(char *);

/**
 * @brief Cette fonction retourne l'indice de la couleur dans le tableau d'imageTexture ou -1 si il ne l'a pas trouvé.
 * @param color - énumération dans @file tetris.h
 * @return int
 */
int get_indice_by_color(color);

/**
 * @brief Cette fonction passe d'une énumération color à une chaîne de caractère pour facilité la recherche dans le répertoire images
 * @param color - énumération dans @file tetris.h
 * @return char*
 */
char *color_to_string(color);

/**
 * @brief Cette fonction permet de donner le chemin complet d'une image dans notre répertoire assets/images
 * @param char* - Nom de l'image                 ex : BLUE , OPTION , JOUER, ect
 * @return char* - retourne son chemin complet   ex : assets/images/BLUE.bmp
 */
char *get_image_path(char *);

/**
 * @brief Cette fonction permet d'afficher un bouton avec une image sur notre menu.
 * @param Button * - Pointeur de notre structure Button
 * @return void
 */
void display_image_button(Button *);

/**
 * @brief Cette fonction permet d'afficher un bouton avec du texte sur notre menu.
 * @param Button * - Pointeur de notre structure Button
 * @return void
 */
void display_button(Button *);

/**
 * @brief Cette fonction permet d'afficher un titre
 *
 * @param char*          - Le texte du titre à afficher.
 * @param SDL_Color      - La couleur du texte.
 * @return SDL_Texture*  - La texture du titre créée.
 */
SDL_Texture *display_title(char *, SDL_Color);

/**
 * @brief Cette fonction permet d'afficher une grille de level avec plein de paramètres.
 * @param Button *  - Un pointeur de notre structure Button
 * @param int       - Le nombre de level ( 19 pour notre cas )
 * @param int       - La largeur du bouton
 * @param int       - La hauteur du bouton
 * @param int       - L'espacement du bouton
 * @param int       - Nombre de bouton de niveau par ligne
 */
void create_level_buttons(Button *, int, int, int, int, int);

/**
 * @brief Cette fonction permet d'afficher du texte à l'écran.
 * @param char *      - Le texte à afficher
 * @param SDL_Rect    - Le rectangle pour s'avoir la position ainsi que sa taille sur le rendu
 * @param SDL_Color   - La couleur du texte
 */
void display_txt(char *, SDL_Rect, SDL_Color);

/**
 * @brief Cette fonction permet d'afficher la prochaine pièce du tetris.
 * @param Tetris *   - Pointeur de notre jeu Tetris,cela permet de récupéré la prochaine pièce du jeu.
 */
void display_next_piece(Tetris *);

/**
 * @brief Cette fonction permet d'afficher toutes les statistiques de nos pièces.
 * @param Tetris *  - Pointeur de notre jeu Tetris, pou r récupéré nos statistiques
 * @param SDL_Rect  - La position et la taille de notre affichage sur le rendu
 * @param SDL_Color - La couleur du texte
 */
void display_stat_piece(Tetris *, SDL_Rect, SDL_Color);

/**
 * @brief Cette fonction permet de passer un entier en chaîne de caractère.
 * @param int     - L'entier à mettre en chaîne de caractère
 * @return char*  - La chaîne de caractère de notre entier
 */
char *int_to_str(int);

/**
 * @brief Cette fonction permet de prendre un prefixe et un entier pour en faire une seule et même chaîne de caractère.
 * @param char*      - Le prefixe de notre chaîne de caractère
 * @param int        - L'entier qu'on veut mettre en chaîne de caractère @see int_to_str()
 * @return char*     - La chaîne de caractère avec notre prefixe et notre entier
 */
char *int_to_str_with_prefix(char *, int);

/**
 * @brief Cette fonction permet de gérer les évènements clavier du joueur.
 * On peut switch de Vue Ncurses à SDL en tapant sur la touche m.
 * @param Tetris* - Pointeur vers notre jeu Tetris
 * @return char   - Retourne un caractère (voir @file tetris.c pour le traitement) ou renvoie le caractère ' '
 */
char input_SDL(Tetris *);

/**
 * @brief Affiche notre jeu Tetris avec les détails ( niveau, statistiques, ...).
 *
 * Cette fonction efface le rendu précédent, affiche l'arrière-plan, la musique appropriée,
 * puis affiche chaque tuile du tableau Tetris à sa position respective.
 *
 * @param Tetris* Pointeur vers notre jeu Tetris.
 * @return void
 * @see display_info_SDL()
 */
void display_SDL(Tetris *);

/**
 * @brief Cette fonction permet d'afficher les détails de notre tetris sur @see display_SDL.
 * Les statistiques de nos pièce, la prochaine pièce qui va tomber, le niveau actuel du joueur,
 * ,son nombre de ligne compléter, son score et le meilleur score à battre.
 * @param Tetris *    - Pointeur vers notre jeu Tetris pour récupérer les informations
 */
void display_info_SDL(Tetris *);

/**
 * @brief Cette fonction gère les évènements sur notre menu.
 * @param Button *  - Pointeur vers notre structure Button qui gère de passer sur la selection de niveau
 * @param Button *  - Pointeur vers notre structure Button qui gère de passer à la fenêtre des options
 * @param Button *  - Pointeur vers notre structure Button qui gère de quitter notre jeu
 * @param Tetris *  - Pointeur vers notre jeu Tetris pour gérer les états ( énumération dans @file tetris.h )
 *
 * @return int  - Si on a selectionner un bouton on retourne 1
 */
int home_page_events(Button *, Button *, Button *, Tetris *);

/**
 * @brief Cette fonction gère les évènements sur notre selection de niveau.
 *
 * @param int*       - Pointeur vers le niveau qu'on veut selectionner
 * @param Button*    - Pointeur vers notre structure Button qui gère de revenir vers le menu (Bouton retour)
 * @param Button[]   - Grille de structure Button avec tout les niveaux
 * @param int        - Le nombre de niveau que l'on peut selectionner
 *
 * @return int       - Retourne 1 si on appuie sur le bouton retour, retourne 2 dès qu'un niveau est sélectionner.
 */
int level_selection_events(int *, Button *, Button[], int);

/**
 * @brief Cette fonction affiche les oprtions à l'écran,comme arrêter la musique, ou les sons ou encore diminuer le volume.
 * @param Tetris *   -Pointeur vers notre jeu Tetris
 * @return void
 */
void settings_SDL(Tetris *);

/**
 * @brief Cette fonction affiche la selection de niveau à l'écran.
 * @param Tetris *  - Pointeur vers notre jeu Tetris.
 * @return void
 */
void level_selection_SDL(Tetris *);

/**
 * @brief Cette fonction affiche le menu à l'écran.
 * @param Tetris *    - Pointeur vers notre jeu Tetris
 * @return void
 */
void home_page_SDL(Tetris *);

/**
 * @brief Cette fonction permet d'afficher à l'écran, le tableau des scores.
 * @param Tetris *    - Pointeur vers notre jeu Tetris
 * @return void
 */
void display_highscores(Tetris *);

/**
 * @brief Cette fonction permet de trouver la position du joueur si il a fait un highscore et de décaler le reste du tableau avant de sauvegarder dans le fichier. ( @see save_highscores() )
 * @param Tetris *   - Pointeur vers notre jeu Tetris
 * @param char*      - Le nom du joueur
 * @return void
 */
void update_highscores(Tetris *, char *);

/**
 * @brief Cette fonction permet de sauvegarder les nouveaux scores sur highscore.txt dans assets/data.
 * @param Tetris *   - Pointeur vers notre jeu Tetris
 * @param int        - Nombre de score voulu
 * @return void
 */
void save_highscores(Tetris *, int);

/**
 * @brief Cette fonction gère les évènements des boutons de la fin de partie.
 * @param SDL_Keycode     - Touche du clavier pressé
 * @param Tetris *        - Un Pointeur vers notre jeu Tetris
 * @param char*           - Le nom du joueur
 * @param Button*         - Pointeur vers notre structure Button qui gère le relancement du jeu
 * @param Button*         - Pointeur vers noter structure Button qui gère l'évènement de quitter le jeu
 * @param Button*         - Pointeur vers notre structure Button qui gère l'évènement de sauvegarder le score du joueur dans le tetris->highscores ( @file tetris.h )
 * @param int*            - Pointeur pour sortir de notre boucle dans @see end_screen_SDL()
 * @return void
 */
void end_screen_button_events(SDL_Keycode, Tetris *, char *, Button *, Button *, Button *, int *);

/**
 * @brief Cette fonction gère les évènenements de textInput de la fin de partie
 * @param SDL_Event *   - Pointeur vers le type d'évènement
 * @param char*         - Le nom du joueur
 * @return void
 */
void end_screen_text_input(SDL_Event *, char *);

/**
 * @brief Cette fonction permet d'afficher à l'écran la zone ou le joueur peut taper son pseudo.
 * @param SDL_Rect     - Le rectangle ou s'affichera la zone de texte
 * @param char *       - Le nom du joueur
 * @param SDL_Color    - La couleur de la font
 * @return void
 */
void display_player_name(SDL_Rect, char *, SDL_Color);

/**
 * @brief Cette fonction permet de savoir si un score du joueur est un meilleur score ou non
 * @param Tetris *    - Pointeur vers notre jeu Tetris
 * @return void
 */
void is_highscore(Tetris *);

/**
 * @brief Cette fonction permet d'afficher à l'écran la fin de partie.
 * @param Tetris *     - Pointeur vers notre jeu Tetris
 * @return void
 */
void end_screen_SDL(Tetris *);

/**
 * @brief Cette fonction permet de jouer un son par rapport à son indice dans le tableau sounds
 * @param int     - Indice du son voulant être jouer
 * @return void
 */
void play_sound_SDL(int);

/**
 * @brief Cette fonction permet de libéré les sons et musiques des tableaux statiques musics et sounds ( variables globales )
 * @param void
 * @return void
 */
void cleanup_audio();

/**
 * @brief Cette fonction permet de libéré les textures alloué par le tableau statique imageTexture ( variables globales )
 * @param void
 * @return void
 */
void clear_img_textures();

/**
 * @brief Cette fonction permet de libéré la mémoire alloués pour la grille de level @see level_selection_SDL()
 * @param Button[]        - La grille de bouton
 * @param int             - Le nombre de level
 * @return void
 */
void free_levels(Button[], int);

/**
 * @brief Cette fonction permet de libéré le reste des ressources et SDL
 * @param void
 * @return void
 */
void close_SDL();