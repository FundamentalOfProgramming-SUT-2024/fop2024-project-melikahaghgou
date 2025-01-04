// // #ifndef USER_MANAGER_H
// // #define USER_MANAGER_H

// // #include <ncurses.h>
// // #include <stdbool.h>

// // #define MAX_USERNAME_LENGTH 50
// // #define MAX_PASSWORD_LENGTH 50
// // #define USER_FILE "users.txt"

// // // User structure
// // typedef struct {
// //   char username[MAX_USERNAME_LENGTH];
// //   char password[MAX_PASSWORD_LENGTH];
// // } User;

// // // Function declarations
// // void clear_screen();
// // bool create_user(const char *username, const char *password);
// // bool authenticate_user(const char *username, const char *password);
// // bool user_exists(const char *username);
// // void get_password(char *password);
// // void forgot_password_menu();

// // #endif

// #ifndef USER_MANAGER_H
// #define USER_MANAGER_H

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>
// #include <ncurses.h>

// #define minpasswordlength 7
// #define FILENAME "users.txt"

// int username_exists(const char *username);
// int is_valid_email(const char *email);
// int is_valid_password(const char *password);
// void create_user();

// #endif // USER_MANAGER_H

#ifndef USER_MANAGER_H
#define USER_MANAGER_H

#include <stdio.h>   // اضافه کردن این خط
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>

#define minpasswordlength 7
#define FILENAME "users.txt"

int username_exists(const char *username);
int is_valid_email(const char *email);
int is_valid_password(const char *password);
void create_user();

#endif // USER_MANAGER_H