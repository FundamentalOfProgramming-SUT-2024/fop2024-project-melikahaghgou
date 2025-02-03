#ifndef LOGIN_H
#define LOGIN_H

void login();
void clear_screen();
bool signup(const char *username, const char *password);
bool authenticate_user(const char *username, const char *password);
bool user_exists(const char *username);
void get_password(WINDOW *win, char *password) ;
bool isStrongPassword(const char *password);

void forgot_password_menu() ;
void nemidoonam();

#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <ctype.h>
#include"login.h"

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define USER_FILE "users.txt"
#define MIN_PASSWORD_LENGTH 7

typedef struct {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
} User;

// Function to clear the screen
void clear_screen() {
    clear();
    refresh();
}

// Function to create a new window and display a title
WINDOW *create_newwin(int height, int width, int starty, int startx, char *title) {
    WINDOW *local_win;

    local_win = newwin(height, width, starty, startx);
    box(local_win, 0, 0);
    mvwprintw(local_win, 0, (width - strlen(title)) / 2, "%s", title);
    wrefresh(local_win);

    return local_win;
}

// Function to authenticate a user
bool authenticate_user(const char *username, const char *password) {
    FILE *fp = fopen(USER_FILE, "r");
    if (fp == NULL) {
        return false;
    }

    char line[MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH + 2];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char file_username[MAX_USERNAME_LENGTH];
        char file_password[MAX_PASSWORD_LENGTH];
        if (sscanf(line, "%s %s", file_username, file_password) == 2) {
            if (strcmp(username, file_username) == 0 && strcmp(password, file_password) == 0) {
                fclose(fp);
                return true;
            }
        }
    }
    fclose(fp);
    return false;
}

// Function to check if a user exists
bool user_exists(const char *username) {
    FILE *fp = fopen(USER_FILE, "r");
    if (fp == NULL) {
        return false;
    }
    char line[MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH + 2];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char file_username[MAX_USERNAME_LENGTH];
        char file_password[MAX_PASSWORD_LENGTH];
        if (sscanf(line, "%s %s", file_username, file_password) == 2) {
            if (strcmp(username, file_username) == 0) {
                fclose(fp);
                return true;
            }
        }
    }
    fclose(fp);
    return false;
}

// Function to get password with input masking
void get_password(WINDOW *win, char *password) {
    int ch;
    int i = 0;
    wrefresh(win);
    while ((ch = wgetch(win)) != '\n' && i < MAX_PASSWORD_LENGTH - 1) {
        if (ch == KEY_BACKSPACE || ch == 127) {
            if (i > 0) {
                i--;
                wprintw(win, "\b \b");
            }
        } else if (ch >= 32 && ch <= 126) {
            password[i] = ch;
            wprintw(win, "*");
            i++;
        }
    }
    password[i] = '\0';
    wrefresh(win);
}

// Function to check password strength
bool isStrongPassword(const char *password) {
    if (strlen(password) < MIN_PASSWORD_LENGTH)
        return false;
    bool hasLower = false, hasUpper = false, hasDigit = false;
    for (int i = 0; password[i]; i++) {
        if (islower(password[i])) hasLower = true;
        if (isupper(password[i])) hasUpper = true;
        if (isdigit(password[i])) hasDigit = true;
    }
    return hasLower && hasUpper && hasDigit;
}

// Function for the forgot password menu
void forgot_password_menu() {
    char username[MAX_USERNAME_LENGTH];
    char new_password[MAX_PASSWORD_LENGTH];

    int startx = (COLS - 40) / 2;
    int starty = (LINES - 10) / 2;
    WINDOW *fp_win = create_newwin(10, 40, starty, startx, "Forgot Password");

    mvwprintw(fp_win, 2, 1, "Enter your username: ");
    wrefresh(fp_win);
    wgetstr(fp_win, username);

    if (user_exists(username)) {
        do {
            mvwprintw(fp_win, 3, 1, "Enter your new password: ");
            wrefresh(fp_win);
            get_password(fp_win, new_password);
            if (!isStrongPassword(new_password)) {
                mvwprintw(fp_win, 4, 1, "Password is not strong enough.");
                wrefresh(fp_win);
                wgetch(fp_win);
                wclear(fp_win);
                box(fp_win, 0, 0);
                mvwprintw(fp_win, 0, (40 - strlen("Forgot Password")) / 2, "Forgot Password");
                mvwprintw(fp_win, 2, 1, "Enter your username: %s", username);
                wrefresh(fp_win);
            }
        } while (!isStrongPassword(new_password));

        FILE *fp = fopen(USER_FILE, "r+");
        if (fp == NULL) {
            mvwprintw(fp_win, 4, 1, "Error opening users file!");
            wrefresh(fp_win);
            wgetch(fp_win);
            delwin(fp_win);
            return;
        }
        char line[MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH + 2];
        FILE *temp_file = tmpfile();
        if (temp_file == NULL) {
            mvwprintw(fp_win, 4, 1, "Error creating temporary file!");
            wrefresh(fp_win);
            fclose(fp);
            wgetch(fp_win);
            delwin(fp_win);
            return;
        }
        bool user_found = false;
        while (fgets(line, sizeof(line), fp) != NULL) {
            char file_username[MAX_USERNAME_LENGTH];
            char file_password[MAX_PASSWORD_LENGTH];
            if (sscanf(line, "%s %s", file_username, file_password) == 2) {
                if (strcmp(username, file_username) == 0) {
                    fprintf(temp_file, "%s %s\n", username, new_password);
                    user_found = true;
                } else {
                    fprintf(temp_file, "%s", line);
                }
            } else {
                fprintf(temp_file, "%s", line);
            }
        }
        if (user_found) {
            fseek(temp_file, 0, SEEK_SET);
            rewind(fp);
            while (fgets(line, sizeof(line), temp_file) != NULL) {
                fputs(line, fp);
            }
            ftruncate(fileno(fp), ftell(fp));
            fclose(fp);
            fclose(temp_file);
            mvwprintw(fp_win, 4, 1, "Password changed successfully.");
            wrefresh(fp_win);
            wgetch(fp_win);
        } else {
            fclose(fp);
            fclose(temp_file);
            mvwprintw(fp_win, 4, 1, "User with this username does not exist.");
            wrefresh(fp_win);
            wgetch(fp_win);
        }
    } else {
        mvwprintw(fp_win, 4, 1, "User with this username does not exist.");
        wrefresh(fp_win);
        wgetch(fp_win);
    }

    delwin(fp_win);
}

void nemidoonam() {
    initscr(); // Initialize ncurses
    // noecho(); // Turn off echoing of characters
    cbreak(); // Disable line buffering
    keypad(stdscr, TRUE); // Enable arrow keys

    int startx = (COLS - 40) / 2;
    int starty = (LINES - 10) / 2;
    WINDOW *menu_win = create_newwin(10, 40, starty, startx, "User Authentication");

    char choice;
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    while (true) {
        mvwprintw(menu_win, 2, 1, "1. Login");
        mvwprintw(menu_win, 3, 1, "2. Forgot Password");
        mvwprintw(menu_win, 4, 1, "3. Exit");
        mvwprintw(menu_win, 6, 1, "Enter your choice: ");
        wrefresh(menu_win);
        choice = wgetch(menu_win);

        switch (choice) {
            case '1': {
                clear_screen();
                WINDOW *login_win = create_newwin(10, 40, starty, startx, "Login");
                mvwprintw(login_win, 2, 1, "Enter your username: ");
                wrefresh(login_win);
                wgetstr(login_win, username);

                mvwprintw(login_win, 3, 1, "Enter your password: ");
                wrefresh(login_win);
                get_password(login_win, password);

                if (authenticate_user(username, password)) {
                    mvwprintw(login_win, 4, 1, "Login successful.");
                    return;
                } else {
                    mvwprintw(login_win, 4, 1, "Invalid username or password.");
                }
                wrefresh(login_win);
                wgetch(login_win);
                delwin(login_win);
                wrefresh(login_win);
                break;
            }
            case '2':
                forgot_password_menu();
                break;

            case '3':
                clear();
                endwin();
                break;

            default:
                mvwprintw(menu_win, 8, 1, "Invalid choice. Try again.");
                wrefresh(menu_win);
                break;
        }
    }

    endwin(); // End ncurses
}

#endif