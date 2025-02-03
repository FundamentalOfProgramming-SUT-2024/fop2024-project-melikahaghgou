#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_USERNAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50
#define MAX_EMAIL_LENGTH 100
#define USER_FILE "users.txt"

void clear_screen() {
    clear();
    refresh();
}

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

bool username_exists(const char *username) {
    FILE *fp = fopen(USER_FILE, "r");
    if (fp == NULL) {
        return false;
    }

    char line[MAX_USERNAME_LENGTH + MAX_PASSWORD_LENGTH + 2];
    while (fgets(line, sizeof(line), fp) != NULL) {
        char file_username[MAX_USERNAME_LENGTH];
        if (sscanf(line, "%s", file_username) == 1) {
            if (strcmp(username, file_username) == 0) {
                fclose(fp);
                return true;
            }
        }
    }
    fclose(fp);
    return false;
}

bool is_valid_email(const char *email) {
    const char *at_sign = strchr(email, '@');
    if (!at_sign || at_sign == email || !strchr(at_sign, '.')) {
        return false;
    }
    return true;
}

bool is_valid_password(const char *password) {
    if (strlen(password) < 7) return false;

    bool has_upper = false, has_lower = false, has_digit = false;
    
    for (int i = 0; password[i] != '0'; i++) {
        if (isupper(password[i])) has_upper = true;
        else if (islower(password[i])) has_lower = true;
        else if (isdigit(password[i])) has_digit = true;
    }
    
    return has_upper && has_lower && has_digit;
}

void get_password(char *password) {
    int ch;
    int i = 0;
    while ((ch = getch()) != '\n' && i < MAX_PASSWORD_LENGTH - 1) {
        if (ch == KEY_BACKSPACE || ch == 127) {
            if (i > 0) {
                i--;
                printw("\b \b");
            }
        } else if (ch >= 32 && ch <= 126) { 
            password[i] = ch;
            printw("*");
            i++;
        }
    }
    password[i] = '0'; 
}

void signup_menu() {
    char username[MAX_USERNAME_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    clear_screen();
    printw("Sign Up Menu\n");

    while (true) {
        printw("Enter your username: ");
        refresh();
        getstr(username);

        if (username_exists(username)) {
            printw("Username already exists! Please choose another one.\n");
            getch();
        } else {
            break;
        }
    }

    while (true) {
        printw("Enter your email: ");
        refresh();
        getstr(email);

        if (!is_valid_email(email)) {
            printw("Invalid email format! Please enter a valid email.\n");
            getch();
        } else {
            break;
        }
    }

    while (true) {
        printw("Enter your password (at least 7 characters, including upper/lowercase and digits): ");
        refresh();
        get_password(password);

        if (!is_valid_password(password)) {
            printw("Password must be at least 7 characters long and include upper/lowercase letters and digits!\n");
            getch();
        } else {
            break;
        }
    }

    FILE *fp = fopen(USER_FILE, "a");
    if (fp != NULL) {
        fprintf(fp, "%s %s\n", username, password);
        fclose(fp);
        printw("Sign up successful! You can now log in.\n");
    } else {
        printw("Error saving user information!\n");
    }

    getch();
}

void login_menu() {
    char username[MAX_USERNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];

    clear_screen();
    printw("Login Menu\n");
    
    printw("Enter your username: ");
    refresh();
    getstr(username);

    printw("Enter your password: ");
    refresh();
    get_password(password);

    if (authenticate_user(username, password)) {
        printw("Login successful! Welcome, %s!\n", username);
    } else {
        printw("Invalid username or password.\n");
    }

    getch();
}

void guest_menu() {
    clear_screen();
    printw("Welcome Guest! Enjoy the game!\n");
    getch();
}

int main() {
    initscr();
    cbreak();

    int choice;

    while (true) {
        clear_screen();
        printw("Main Menu\n");
        printw("1. Login\n");
        printw("2. Guest Login\n");
        printw("3. Sign Up\n");
        printw("4. Forgot Password\n");
        printw("5. Exit \n ");
        printw("Choose an option: ");
        
        choice = getch() - '0'; 
        
        switch (choice) {
            case 1:
                login_menu();
                break;
            case 2:
                guest_menu();
                break;
            case 3:
                signup_menu();
                break;
            case 4:
                break;
            case 5:
                endwin();
                exit(0);
            default:
                printw("Invalid choice! Please try again.\n");
                getch();
                break;
        }
    }

    endwin();
    return 0;
}

