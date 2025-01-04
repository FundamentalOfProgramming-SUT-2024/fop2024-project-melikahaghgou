#include<stdio.h>
#include "user_manager.h"

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
    if (strlen(password) < minpasswordlength) return 0;

    int has_digit = 0, has_upper = 0, has_lower = 0;
    for (int i = 0; password[i]; i++) {
        if (isdigit(password[i])) has_digit = 1;
        if (isupper(password[i])) has_upper = 1;
        if (islower(password[i])) has_lower = 1;
    }
    return has_digit && has_upper && has_lower;
}

void create_user() {
    char username[50], password[50], email[100];

    initscr(); 
    cbreak();  

    while (1) {
        printw("Enter username: ");
        getstr(username);
        if (!username_exists(username)) {
            break; 
        }
        printw("Username already exists! Please enter a different username.\n");
    }

    while (1) {
        printw("Enter email: ");
        getstr(email);
        if (is_valid_email(email)) {
            break; 
        }
        printw("Invalid email format! Please enter again (xxx@y.zzz).\n");
    }

    while (1) {
        printw("Enter password: ");
        getstr(password);
        if (is_valid_password(password)) {
            break; 
        }
        printw("Password must be at least 7 characters long, contain a digit, an uppercase and a lowercase letter.\n");
    }

    FILE *file = fopen(FILENAME, "a");
    if (file) {
        fprintf(file, "%s %s %s\n", username, password, email);
        fclose(file);
        printw("User created successfully!\n");
    } else {
        printw("Error saving user data!\n");
    }

    refresh();
    getch();   
    endwin();  
}