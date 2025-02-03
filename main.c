#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include "before_game_menu.h"
#include "signup.h"
#include "login.h"
#include "main_menu.h"
#include "generate_map.h"

#define COLOR_GOLD 10
#define COLOR_GREY 11
#define COLOR_ORANGE 12

char state[100] = "menu";

void initialize_colors(){
    start_color();
    init_color(COLOR_GOLD, 1000, 843, 0);
    init_color(COLOR_GREY, 500, 500, 500);
    init_color(COLOR_ORANGE, 900, 400, 0);
    init_pair(10, COLOR_GOLD, COLOR_BLACK);
    init_pair(11, COLOR_GREY, COLOR_BLACK);
    init_pair(12, COLOR_WHITE, COLOR_BLACK);
    init_pair(13, COLOR_YELLOW, COLOR_BLACK);
    init_pair(14, COLOR_GREEN, COLOR_BLACK);
    init_pair(15, COLOR_RED, COLOR_BLACK);
    init_pair(16, COLOR_ORANGE, COLOR_BLACK);
    init_pair(17, COLOR_GOLD, COLOR_BLACK);
}

int main() {
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    initialize_colors();

    while (1) {
        if (strcmp(state, "menu") == 0) {
            clear();
            refresh();
            strcpy(state, main_menu());
            //strcpy(state, "new game");
        } else if (strcmp(state, "Login") == 0) {
            clear();
            refresh();
            nemidoonam();
            //strcpy(state, "before game menu");
        } else if (strcmp(state, "Sign Up") == 0) {
            clear();
            refresh();
            create_user();
            strcpy(state, "menu");
        } else if (strcmp(state, "before game menu") == 0) {
            clear();
            refresh();
            show_before_game_menu();
        } else if (strcmp(state, "settings") == 0 || strcmp(state, "Settings") == 0) {
            clear();
            refresh();
            show_settings_menu();
        } else if (strcmp(state, "New Game") == 0 || strcmp(state, "Resume Game") == 0) {
            clear();
            refresh();
            generate_map();
            move_character(); 
            strcpy(state, "menu");
        } else if (strcmp(state, "Exit") == 0) {
            endwin();
            return 0;
        }
    }
}