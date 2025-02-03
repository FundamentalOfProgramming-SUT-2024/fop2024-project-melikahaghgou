#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include "signup.h"

#define MIN_PASSWORD_LENGTH 7
#define FILENAME "users.txt"

int username_exists(const char *username) {
    FILE *file = fopen(FILENAME, "r");
    if (!file) return 0;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char stored_username[50];
        sscanf(line, "%s", stored_username);
        if (strcmp(stored_username, username) == 0) {
            fclose(file);
            return 1; 
        }
    }
    fclose(file);
    return 0; 
}

int is_valid_email(const char *email) {
    const char *at = strchr(email, '@');
    if (!at) return 0;
    const char *dot = strchr(at, '.');
    return dot != NULL && dot > at + 1 && at != email && dot < at + 64; 
}

int is_valid_password(const char *password) {
    if (strlen(password) < MIN_PASSWORD_LENGTH) return 0;

    int has_digit = 0, has_upper = 0, has_lower = 0;
    for (int i = 0; password[i]; i++) {
        if (isdigit(password[i])) has_digit = 1;
        if (isupper(password[i])) has_upper = 1;
        if (islower(password[i])) has_lower = 1;
    }
    return has_digit && has_upper && has_lower;
}

WINDOW *create_newwin_sign_up(int height, int width, int starty, int startx, const char *title) {
    WINDOW *local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);
    mvwprintw(local_win, 0, (width - strlen(title)) / 2, "%s", title);
    wrefresh(local_win);
    return local_win;
}

void create_user() {
    char username[50], password[50], email[100];

    initscr(); 
    noecho();
    cbreak();  
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE); 
    init_pair(2, COLOR_BLACK, COLOR_MAGENTA); 
    int startx = (COLS - 50) / 2;
    int starty = (LINES - 15) / 2;
    WINDOW *signup_win = create_newwin_sign_up(15, 50, starty, startx, "Sign Up");

    wattron(signup_win, COLOR_PAIR(1));
    while (1) {
        mvwprintw(signup_win, 2, 1, "Enter username: ");
        wrefresh(signup_win);
        wgetstr(signup_win, username);
        if (!username_exists(username)) {
            break; 
        }
        mvwprintw(signup_win, 3, 1, "Username already exists! Please enter a different username.");
        wrefresh(signup_win);
    }

    while (1) {
        mvwprintw(signup_win, 4, 1, "Enter email: ");
        wrefresh(signup_win);
        wgetstr(signup_win, email);
        if (is_valid_email(email)) {
            break; 
        }
        mvwprintw(signup_win, 5, 1, "Invalid email format! Please enter again (xxx@y.zzz).");
        wrefresh(signup_win);
    }

    while (1) {
        mvwprintw(signup_win, 6, 1, "Enter password: ");
        wrefresh(signup_win);
        wgetstr(signup_win, password);
        if (is_valid_password(password)) {
            break; 
        }
        mvwprintw(signup_win, 7, 1, "Password must be at least 7 characters long, contain a digit, an uppercase and a lowercase letter.");
        wrefresh(signup_win);
    }

    FILE *file = fopen(FILENAME, "a");
    if (file) {
        fprintf(file, "%s %s %s\n", username, password, email);
        fclose(file);
        mvwprintw(signup_win, 8, 1, "User created successfully!");
    } else {
        mvwprintw(signup_win, 8, 1, "Error saving user data!");
    }

    wrefresh(signup_win);
    wgetch(signup_win);   
    delwin(signup_win);  
    endwin();  
}
