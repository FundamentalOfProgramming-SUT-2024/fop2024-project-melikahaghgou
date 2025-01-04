#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <ncurses.h>
#include"signup.h"
#include"login.h"
#include"menu.h"


char state[100] = "menu";
int main(){
    while(1){
        if(strcmp(state, "menu")==0){
            clear();
            refresh();
            strcpy(state, main_menu());
        }
        else if(strcmp(state, "Login")==0){
            clear();
            refresh();
            nemidoonam();
            strcpy(state, "menu");
        }
        else if(strcmp(state, "Sign Up")==0){
            clear();
            refresh();
            create_user();
            strcpy(state, "menu");
        }
        else if(strcmp(state, "Exit")){
            return 0;
        }
    }
}
