#ifndef BEFORE_GAME_MENU_H
#define BEFORE_GAME_MENU_H

#include <ncurses.h>

void show_before_game_menu();
void show_settings_menu();
void show_submenu();

#include "before_game_menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>

extern char state[100];

void show_before_game_menu() {
    int choice;
    int highlight = 0;

    const char choices[100][100] = {
        "New Game",
        "Continue Previous Game",
        "Settings",
        "Profile",
        "Scoreboard"
    };

    WINDOW *menuwin = newwin(7, 30, 4, 4);
    box(menuwin, 0, 0);
    keypad(menuwin, TRUE);

    while (1) {
        for (int i = 0; i < 5; ++i) {
            if (i == highlight)
                wattron(menuwin, A_REVERSE);
            mvwprintw(menuwin, i + 1, 1, "%s", choices[i]);
            wattroff(menuwin, A_REVERSE);
        }
        choice = wgetch(menuwin);

        switch (choice) {
            case KEY_UP:
                if (highlight > 0) highlight--;
                break;
            case KEY_DOWN:
                if (highlight < 4) highlight++;
                break;
            case 10: // Enter key
                switch (highlight) {
                    case 0:
                        strcpy(state, "new game");
                        break;
                    case 1:
                        strcpy(state, "continue previous game");
                        break;
                    case 2:
                        strcpy(state, "settings");
                        show_settings_menu();
                        break;
                    case 3:
                        strcpy(state, "profile");
                        break;
                    case 4:
                        strcpy(state, "scoreboard");
                        break;
                }
                if (highlight != 2) {
                    delwin(menuwin);
                    clear();
                    refresh();
                    return;
                }
                break;
            default:
                break;
        }
    }
}

void show_settings_menu() {
    int choice;
    int highlight = 0;

    const char *choices[] = {
        "Game Difficulty",
        "Character Color",
        "Select Music",
        "Exit"
    };

    const char *difficulties[] = { "Easy", "Medium", "Hard" };
    const char *colors[] = { "Red", "Green", "Blue" };
    const char *music[] = { "Track 1", "Track 2", "Track 3" };

    WINDOW *menuwin = newwin(7, 30, 4, 4);
    box(menuwin, 0, 0);
    keypad(menuwin, TRUE);

    while (1) {
        for (int i = 0; i < 4; ++i) {
            if (i == highlight)
                wattron(menuwin, A_REVERSE);
            mvwprintw(menuwin, i + 1, 1, "%s", choices[i]);
            wattroff(menuwin, A_REVERSE);
        }
        choice = wgetch(menuwin);

        switch (choice) {
            case KEY_UP:
                if (highlight > 0) highlight--;
                break;
            case KEY_DOWN:
                if (highlight < 3) highlight++;
                break;
            case 10: // Enter key
                switch (highlight) {
                    case 0:
                        show_submenu("Select Difficulty", difficulties, 3);
                        break;
                    case 1:
                        show_submenu("Select Character Color", colors, 3);
                        break;
                    case 2:
                        show_submenu("Select Music", music, 3);
                        break;
                    case 3:
                        return;
                }
                break;
            default:
                break;
        }
        // Added clear and refresh after menu selection
        clear();
        refresh();
    }
    delwin(menuwin);
}

void show_submenu(const char *title, const char **options, int n_options) {
    int choice;
    int highlight = 0;

    WINDOW *submenuwin = newwin(n_options + 2, 30, 4, 4);
    box(submenuwin, 0, 0);
    mvwprintw(submenuwin, 0, 1, "%s", title);
    keypad(submenuwin, TRUE);

    while (1) {
        for (int i = 0; i < n_options; ++i) {
            if (i == highlight)
                wattron(submenuwin, A_REVERSE);
            mvwprintw(submenuwin, i + 1, 1, "%s", options[i]);
            wattroff(submenuwin, A_REVERSE);
        }
        choice = wgetch(submenuwin);

        switch (choice) {
            case KEY_UP:
                if (highlight > 0) highlight--;
                break;
            case KEY_DOWN:
                if (highlight < n_options - 1) highlight++;
                break;
            case 10: // Enter key
                delwin(submenuwin);
                clear();
                refresh();
                return;
            default:
                break;
        }
    }
}

#endif
