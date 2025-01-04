#include <ncurses.h>
#include <string.h>
#include"menu.h"
#define MENU_WIDTH 40
#define MENU_HEIGHT 10

WINDOW *create_newwin_for_menu(int height, int width, int starty, int startx, const char *title) {
    WINDOW *local_win = newwin(height, width, starty, startx);
    wbkgd(local_win, COLOR_PAIR(1));
    box(local_win, 0, 0);
    mvwprintw(local_win, 0, (width - strlen(title)) / 2, "%s", title);
    wrefresh(local_win);
    return local_win;
}

char main_menu() {
    initscr(); // Initialize ncurses
    cbreak(); // Disable line buffering
    noecho(); // Turn off echoing of characters
    keypad(stdscr, TRUE); // Enable arrow keys

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE); // White text on blue background
    init_pair(2, COLOR_BLACK, COLOR_MAGENTA); // Black text on pink background

    int startx = (COLS - MENU_WIDTH) / 2;
    int starty = (LINES - MENU_HEIGHT) / 2;
    WINDOW *menu_win = create_newwin(MENU_HEIGHT, MENU_WIDTH, starty, startx, "Main Menu");

    char *choices[] = {"Login", "Sign Up", "Settings", "Exit"};
    int n_choices = sizeof(choices) / sizeof(char *);
    int highlight = 0;

    while (true) {
        for (int i = 0; i < n_choices; ++i) {
            if (i == highlight)
                wattron(menu_win, A_REVERSE | COLOR_PAIR(2));
            else
                wattron(menu_win, COLOR_PAIR(1));
            mvwprintw(menu_win, i + 2, 1, "%s", choices[i]);
            wattroff(menu_win, A_REVERSE | COLOR_PAIR(2));
        }
        wrefresh(menu_win);

        int ch = wgetch(menu_win);
        switch (ch) {
            case KEY_UP:
                highlight = (highlight - 1 + n_choices) % n_choices;
                break;
            case KEY_DOWN:
                highlight = (highlight + 1) % n_choices;
                break;
            case 10: // Enter key
                delwin(menu_win);
                endwin();
                return choices[highlight];
            default:
                break;
        }
    }

    delwin(menu_win);
    endwin(); // End ncurses
    return 0;
}
