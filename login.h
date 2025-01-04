#ifndef LOGIN_H
     #define LOGIN_H

     void login(); // تابعی برای ورود
void clear_screen();
bool signup(const char *username, const char *password);
bool authenticate_user(const char *username, const char *password);
bool user_exists(const char *username);
void get_password(WINDOW *win, char *password) ;
bool isStrongPassword(const char *password);

void forgot_password_menu() ;
void nemidoonam();
     #endif