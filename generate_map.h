// generate_map.h
#ifndef GENERATE_MAP_H
#define GENERATE_MAP_H

typedef struct{
    int x;
    int y;
} Point;
void generate_map();
void move_character();

#include "generate_map.h"
#include <ncurses.h>
#include <stdlib.h>
#include <time.h>

char map[200][200];
char color_map[200][200];
int room_positions[6][4]; // Stores the top, left, bottom, right of each room

int hunger = 10;

void generate_map() {
    int width, height;
    getmaxyx(stdscr, height, width);

    srand(time(NULL));

    // Initialize the map array to empty spaces
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            map[i][j] = ' ';
        }
    }

    // Divide screen into 6 sections
    int sections = 6;
    int section_height = height / 2;
    int section_width = width / 3;

    // To store center points of rooms
    int centers[6][2];

    for (int i = 0; i < sections; ++i) {
        int row = (i / 3) * section_height + 5;
        int col = (i % 3) * section_width;

        // Generate random dimensions for the room
        int room_height = (rand() % (section_height / 3 - 2)) + 6;
        int room_width = (rand() % (section_width / 2 - 2)) + 6;

        // Ensure room fits within section
        if (room_height > section_height - 1) room_height = section_height - 1;
        if (room_width > section_width - 1) room_width = section_width - 1;

        // Draw top and bottom borders
        for (int j = 0; j < room_width; ++j) {
            mvaddch(row, col + j, '-');
            map[row][col + j] = '-';
            mvaddch(row + room_height - 1, col + j, '-');
            map[row + room_height - 1][col + j] = '-';
        }

        // Draw left and right borders
        for (int j = 0; j < room_height; ++j) {
            mvaddch(row + j, col, '|');
            map[row + j][col] = '|';
            mvaddch(row + j, col + room_width - 1, '|');
            map[row + j][col + room_width - 1] = '|';
        }

        // Fill the room with points
        for (int j = 1; j < room_height - 1; j++) {
            for (int k = 1; k < room_width - 1; k++) {
                mvaddch(row + j, k + col, '.');
                map[row + j][k + col] = '.';
            }
        }

        // Store center points of the room
        int center_row = row + room_height / 2;
        int center_col = col + room_width / 2;
        centers[i][0] = center_row;
        centers[i][1] = center_col;

        // Store room positions
        room_positions[i][0] = row; // top
        room_positions[i][1] = col; // left
        room_positions[i][2] = row + room_height - 1; // bottom
        room_positions[i][3] = col + room_width - 1; // right
    }

    // Connect rooms with corridors and add branches
    for (int i = 0; i < sections - 1; ++i) {
        int start_row = centers[i][0];
        int start_col = centers[i][1];
        int end_row = centers[i + 1][0];
        int end_col = centers[i + 1][1];

        // Adjust start and end positions to avoid crossing through rooms
        if (start_row > room_positions[i][2]) {
            start_row = room_positions[i][2] + 1;
        } else if (start_row < room_positions[i][0]) {
            start_row = room_positions[i][0] - 1;
        }

        if (start_col > room_positions[i][3]) {
            start_col = room_positions[i][3] + 1;
        } else if (start_col < room_positions[i][1]) {
            start_col = room_positions[i][1] - 1;
        }

        if (end_row > room_positions[i + 1][2]) {
            end_row = room_positions[i + 1][2] + 1;
        } else if (end_row < room_positions[i + 1][0]) {
            end_row = room_positions[i + 1][0] - 1;
        }

        if (end_col > room_positions[i + 1][3]) {
            end_col = room_positions[i + 1][3] + 1;
        } else if (end_col < room_positions[i + 1][1]) {
            end_col = room_positions[i + 1][1] - 1;
        }

        // Draw horizontal corridor with doors
        for (int j = start_col; j != end_col; j += (start_col < end_col ? 1 : -1)) {
            if (mvwinch(stdscr, start_row, j) != '.') {
                if (mvwinch(stdscr, start_row, j) == '|') {
                    mvaddch(start_row, j, '+'); // Door
                    map[start_row][j] = '+';
                } else {
                    mvaddch(start_row, j, '#');
                    map[start_row][j] = '#';
                }
            }
        }
        // Draw vertical corridor with doors
        for (int j = start_row; j != end_row; j += (start_row < end_row ? 1 : -1)) {
            if (mvwinch(stdscr, j, end_col) != '.') {
                if (mvwinch(stdscr, j, end_col) == '-') {
                    mvaddch(j, end_col, '+'); // Door
                    map[j][end_col] = '+';
                } else {
                    mvaddch(j, end_col, '#');
                    map[j][end_col] = '#';
                }
            }
        }

        // Add branches to the corridors
        if (i % 2 == 0) { // Add a branch for every other corridor
            int branch_length = rand() % 5 + 2;
            int branch_dir = rand() % 2; // 0 for horizontal, 1 for vertical

            if (branch_dir == 0) { // Horizontal branch
                int branch_row = (start_row + end_row) / 2;
                int branch_col_start = (start_col + end_col) / 2;
                int branch_col_end = branch_col_start + branch_length * ((rand() % 2) * 2 - 1);

                for (int j = branch_col_start; j != branch_col_end; j += (branch_col_start < branch_col_end ? 1 : -1)) {
                    mvaddch(branch_row, j, '#');
                    map[branch_row][j] = '#';
                }
            } else { // Vertical branch
                int branch_col = (start_col + end_col) / 2;
                int branch_row_start = (start_row + end_row) / 2;
                int branch_row_end = branch_row_start + branch_length * ((rand() % 2) * 2 - 1);

                for (int j = branch_row_start; j != branch_row_end; j += (branch_row_start < branch_row_end ? 1 : -1)) {
                    mvaddch(j, branch_col, '#');
                    map[j][branch_col] = '#';
                }
            }
        }
    }

    // Store map data
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            map[i][j] = mvwinch(stdscr, i, j);
        }
    }
    refresh();
}

void continue_corridor(int y, int x, int a){
    Point next[4];
    next[0].x = x + 1;
    next[0].y = y;
    next[1].x = x;
    next[1].y = y + 1;
    next[2].x = x - 1;
    next[2].y = y;
    next[3].x = x;
    next[3].y = y - 1;
    if(a == 5){
        return;
    }
    else{
        for(int i = 0; i < 4; i++){
            if(map[next[i].y][next[i].x] == '#' || map[next[i].y][next[i].x] == '+'){
                mvprintw(next[i].y, next[i].x, "%c", map[next[i].y][next[i].x]);
                continue_corridor(next[i].y, next[i].x, a + 1);
            }
        }
    }
}

void show_room(int y, int x) {
// Find which room the character is in
    for (int i = 0; i < 6; ++i) {
        if (y >= room_positions[i][0] && y <= room_positions[i][2] && x >= room_positions[i][1] && x <= room_positions[i][3]) {
            for (int row = room_positions[i][0]; row <= room_positions[i][2]; ++row) {
                for (int col = room_positions[i][1]; col <= room_positions[i][3]; ++col) {
                    attron(COLOR_PAIR(color_map[row][col]));
                    mvaddch(row, col, map[row][col]);
                    attroff(COLOR_PAIR(color_map[row][col]));
                }
            }
            break;
        }
    }
    refresh();
}

void create_gold(){
    srand(time(NULL));
    int total_gold = rand() % 11 + 20;
    int gold_per_room = total_gold / 6;

    for (int i = 0; i < 6; ++i) {
        int current_gold = 0;
        while(current_gold < gold_per_room){
            int x_random = room_positions[i][1] + rand() % (room_positions[i][3] - room_positions[i][1] - 1) + 1;
            int y_random = room_positions[i][0] + rand() % (room_positions[i][2] - room_positions[i][0] - 1) + 1;

            if(map[y_random][x_random] == '.'){
                if(rand() % 10 > 0){
                    map[y_random][x_random] = '*';
                    color_map[y_random][x_random] = 10;
                }
                else{
                    map[y_random][x_random] = '$';
                    color_map[y_random][x_random] = 11;
                }
                current_gold++;
            }
        }
    }
}

int gold = 0;

void create_food(){
    int total_food = rand() % 11 + 20;
    int food_per_room = total_food / 6;

    for (int i = 0; i < 6; ++i) {
        int current_food = 0;
        while(current_food < food_per_room){
            int x_random = room_positions[i][1] + rand() % (room_positions[i][3] - room_positions[i][1] - 1) + 1;
            int y_random = room_positions[i][0] + rand() % (room_positions[i][2] - room_positions[i][0] - 1) + 1;
            if(map[y_random][x_random] == '.'){
                int rand_food = rand() % 3 + 1;
                map[y_random][x_random] = '0' + rand_food;
                color_map[y_random][x_random] = 13;
                current_food++;
            }
        }
    }
}

void create_weapon(){
    int total_weapon = rand() % 11 + 20;
    int weapon_per_room = total_weapon / 6;

    for (int i = 0; i < 6; ++i) {
        int current_weapon = 0;
        while(current_weapon < weapon_per_room){
            int x_random = room_positions[i][1] + rand() % (room_positions[i][3] - room_positions[i][1] - 1) + 1;
            int y_random = room_positions[i][0] + rand() % (room_positions[i][2] - room_positions[i][0] - 1) + 1;
            if(map[y_random][x_random] == '.'){
                int rand_weapon = rand() % 4;
                if(rand_weapon == 0){
                    map[y_random][x_random] = 'd';
                }
                if(rand_weapon == 1){
                    map[y_random][x_random] = 'm';
                }
                if(rand_weapon == 2){
                    map[y_random][x_random] = 'n';
                }
                if(rand_weapon == 3){
                    map[y_random][x_random] = 's';
                }
                color_map[y_random][x_random] = 12;
                current_weapon++;
            }
        }
    }
}

void create_spell(){
    int total_spell = rand() % 11 + 20;
    int spell_per_room = total_spell / 6;

    for (int i = 0; i < 6; ++i) {
        int current_spell = 0;
        while(current_spell < spell_per_room){
            int x_random = room_positions[i][1] + rand() % (room_positions[i][3] - room_positions[i][1] - 1) + 1;
            int y_random = room_positions[i][0] + rand() % (room_positions[i][2] - room_positions[i][0] - 1) + 1;
            if(map[y_random][x_random] == '.'){
                int rand_spell = rand() % 4;
                if(rand_spell == 0){
                    map[y_random][x_random] = '!';
                }
                if(rand_spell == 1){
                    map[y_random][x_random] = '~';
                }
                if(rand_spell == 2){
                    map[y_random][x_random] = '%';
                }
                color_map[y_random][x_random] = 12;
                current_spell++;
            }
        }
    }
}

void create_enemy(){
    int total_enemy = 6;
    int enemy_per_room = 1;
    char enemies[5] = "DFGSU";
    for (int i = 0; i < 6; ++i) {
        while(1){
            int x_random = room_positions[i][1] + rand() % (room_positions[i][3] - room_positions[i][1] - 1) + 1;
            int y_random = room_positions[i][0] + rand() % (room_positions[i][2] - room_positions[i][0] - 1) + 1;
            if(map[y_random][x_random] == '.'){
                map[y_random][x_random] = enemies[i];
                color_map[y_random][x_random] = 12;
                break;
            }
        }
    }
}

int weapon_count[5] = {1, 0, 0, 0, 0};
int spell_count[3] = {0, 0, 0};

void create_password_doors() {
    int total_password_door = rand() % 5 + 1;
    int current_password_door = 0;

    while (current_password_door < total_password_door) {
        int x_random = rand() % COLS;
        int y_random = rand() % LINES;

        if (map[y_random][x_random] == '+') {
            map[y_random][x_random] = '@';
            color_map[y_random][x_random] = 15;

            for (int i = 0; i < 6; ++i) {
                if (y_random >= room_positions[i][0] && y_random <= room_positions[i][2] &&
                    x_random >= room_positions[i][1] && x_random <= room_positions[i][3]) {

                    map[room_positions[i][0] + 1][room_positions[i][1] + 1] = '&';
                    color_map[room_positions[i][0] + 1][room_positions[i][1] + 1] = 12;
                    break;
                }
            }

            current_password_door++;
        }
    }
}

int password;

int food_count[3] = {0, 0, 0};

void food_menu(){
    while (1) {
        clear();
        mvprintw(LINES / 2 - 3, COLS / 2 - 10, "1) Regular Food: %d", food_count[0]);
        mvprintw(LINES / 2 - 1, COLS / 2 - 10, "2) Supreme Food: %d", food_count[1]);
        mvprintw(LINES / 2 + 1, COLS / 2 - 10, "3) Magic Food: %d", food_count[2]);
        mvprintw(LINES / 2 + 3, COLS / 2 - 10, "Press Enter to Exit");
        refresh();
        int c = getch();
        if (c == '\n') {
            break;
        }
        if (c == '1') {
            if (food_count[0] == 0) {
                mvprintw(LINES / 2 - 5, COLS / 2 - 10, "You Don't Have Any Regular Food");
                getch();
            } else {
                hunger++;
                food_count[0]--;
            }
        }
        if (c == '2') {
            if (food_count[1] == 0) {
                mvprintw(LINES / 2 - 5, COLS / 2 - 10, "You Don't Have Any Supreme Food");
                getch();
            } else {
                hunger++;
                food_count[1]--;
            }
        }
        if (c == '3') {
            if (food_count[2] == 0) {
                mvprintw(LINES / 2 - 5, COLS / 2 - 10, "You Don't Have Any Magic Food");
                getch();
            } else {
                hunger++;
                food_count[2]--;
            }
        }
    }
    clear();
    refresh();
}


void spell_menu(){
    while (1) {
        clear();
        mvprintw(LINES / 2 - 3, COLS / 2 - 10, "1) Health Spell: %d", spell_count[0]);
        mvprintw(LINES / 2 - 1, COLS / 2 - 10, "2) Speed Spell: %d", spell_count[1]);
        mvprintw(LINES / 2 + 1, COLS / 2 - 10, "3) Damage Spell: %d", spell_count[2]);
        mvprintw(LINES / 2 + 3, COLS / 2 - 10, "Press Enter to Exit");
        refresh();
        int c = getch();
        if (c == '\n') {
            break;
        }
        if (c == '1') {
            if (spell_count[0] == 0) {
                mvprintw(LINES / 2 - 5, COLS / 2 - 10, "You Don't Have Any Health Spell");
                getch();
            } else {
                spell_count[0]--;
            }
        }
        if (c == '2') {
            if (spell_count[1] == 0) {
                mvprintw(LINES / 2 - 5, COLS / 2 - 10, "You Don't Have Any Speed Spell");
                getch();
            } else {
                spell_count[1]--;
            }
        }
        if (c == '3') {
            if (spell_count[2] == 0) {
                mvprintw(LINES / 2 - 5, COLS / 2 - 10, "You Don't Have Any Damage Spell");
                getch();
            } else {
                spell_count[2]--;
            }
        }
    }
    clear();
    refresh();
}

int weapon = 0;

char *weapon_names[6] = {"Mace", "Dagger", "Magic Wand", "Normal Arrow", "Sword", "None"};

void weapon_menu() {
    while (1) {
        clear();
        mvprintw(LINES / 2 - 3, COLS / 2 - 10, "1) Mace: %d", weapon_count[0]);
        mvprintw(LINES / 2 - 1, COLS / 2 - 10, "2) Dagger: %d", weapon_count[1]);
        mvprintw(LINES / 2 + 1, COLS / 2 - 10, "3) Magic Wand: %d", weapon_count[2]);
        mvprintw(LINES / 2 + 3, COLS / 2 - 10, "4) Normal Arrow: %d", weapon_count[3]);
        mvprintw(LINES / 2 + 5, COLS / 2 - 10, "5) Swrod: %d", weapon_count[4]);
        mvprintw(LINES / 2 + 7, COLS / 2 - 10, "Press Enter to Exit");
        refresh();
        int c = getch();
        if (c == '\n') {
            clear();
            break;
        }
        if (c == '1') {
            if (weapon != 5) {
                mvprintw(LINES / 2 - 5, COLS / 2 - 10, "Put Your Current Weapon Down.");
                getch();
            } else {
                weapon = 0;
                mvprintw(LINES / 2 - 5, COLS / 2 - 10, "You Picked %s.", weapon_names[0]);
                getch();
            }
        }
        if (c == '2') {
            if (weapon != 5) {
                mvprintw(LINES / 2 - 5, COLS / 2 - 10, "Put Your Current Weapon Down.");
                getch();
            } else {
                weapon = 1;
                mvprintw(LINES / 2 - 5, COLS / 2 - 10, "You Picked %s.", weapon_names[1]);
                getch();
            }
        }
        if (c == '3') {
            if (weapon != 5) {
                mvprintw(LINES / 2 - 5, COLS / 2 - 10, "Put Your Current Weapon Down.");
                getch();
            } else {
                weapon = 2;
                mvprintw(LINES / 2 - 5, COLS / 2 - 10, "You Picked %s.", weapon_names[0]);
                getch();
            }
        }
        if (c == '4') {
            if (weapon != 5) {
                mvprintw(LINES / 2 - 5, COLS / 2 - 10, "Put Your Current Weapon Down.");
                getch();
            } else {
                weapon = 3;
                mvprintw(LINES / 2 - 5, COLS / 2 - 10, "You Picked %s.", weapon_names[0]);
                getch();
            }
        }
        if (c == '5') {
            if (weapon != 5) {
                mvprintw(LINES / 2 - 5, COLS / 2 - 10, "Put Your Current Weapon Down.");
                getch();
            } else {
                weapon = 4;
                mvprintw(LINES / 2 - 5, COLS / 2 - 10, "You Picked %s.", weapon_names[0]);
                getch();
            }
        }
    }
    clear();
    refresh();
}

void move_character() {
    int ch;
    int x = 5, y = 5;
    clear();
    mvaddch(y, x, '@');
    show_room(y, x);
    refresh();
    create_gold();
    create_weapon();
    create_food();
    create_enemy();
    create_spell();
    create_password_doors();
    long int hunger_timer = time(NULL), password_time;
    int flag = 0;
    int password = 0; // Ensure password is properly declared
    keypad(stdscr, TRUE);

    while ((ch = getch()) != 'q') {
        if (difftime(time(NULL), hunger_timer) >= 5) {
            hunger--;
            hunger_timer = time(NULL);
        }
        int x_previous = x, y_previous = y;
        switch (ch) {
            case 'k':
            case KEY_DOWN:
                if (map[y + 1][x] == '#' || map[y + 1][x] == '.' || map[y + 1][x] == '*' || map[y + 1][x] == '$' || map[y + 1][x] == '+' || map[y + 1][x] == '&' || map[y + 1][x] == 'd' || map[y + 1][x] == 's' || map[y + 1][x] == 'm' || map[y + 1][x] == 'n' || map[y + 1][x] == '!' || map[y + 1][x] == '~' || map[y + 1][x] == '%' || ('1' <= map[y + 1][x] && map[y + 1][x] <= '3')) {
                    mvaddch(y, x, map[y][x]);
                    y++;
                }
                break;
            case 'j':
            case KEY_UP:
                if (map[y - 1][x] == '#' || map[y - 1][x] == '.' || map[y - 1][x] == '*' || map[y - 1][x] == '$' || map[y - 1][x] == '+' || map[y - 1][x] == '&' || map[y - 1][x] == 'd' || map[y - 1][x] == 's' || map[y - 1][x] == 'm' || map[y - 1][x] == 'n' || map[y - 1][x] == '!' || map[y - 1][x] == '~' || map[y - 1][x] == '%' || ('1' <= map[y - 1][x] && map[y - 1][x] <= '3')) {
                    mvaddch(y, x, map[y][x]);
                    y--;
                }
                break;
            case 'h':
            case KEY_LEFT:
                if (map[y][x - 1] == '#' || map[y][x - 1] == '.' || map[y][x - 1] == '*' || map[y][x - 1] == '$' || map[y][x - 1] == '+' || map[y][x - 1] == '&' || map[y][x - 1] == 'd' || map[y][x - 1] == 's' || map[y][x - 1] == 'm' || map[y][x - 1] == 'n' || map[y][x - 1] == '!' || map[y][x - 1] == '~' || map[y][x - 1] == '%' || ('1' <= map[y][x - 1] && map[y][x - 1] <= '3')) {
                    mvaddch(y, x, map[y][x]);
                    x--;
                }
                break;
            case 'l':
            case KEY_RIGHT:
                if (map[y][x + 1] == '#' || map[y][x + 1] == '.' || map[y][x + 1] == '*' || map[y][x + 1] == '$' || map[y][x + 1] == '+' || map[y][x + 1] == '&' || map[y][x + 1] == 'd' || map[y][x + 1] == 's' || map[y][x + 1] == 'm' || map[y][x + 1] == 'n' || map[y][x + 1] == '!' || map[y][x + 1] == '~' || map[y][x + 1] == '%' || ('1' <= map[y][x + 1] && map[y][x + 1] <= '3')) {
                    mvaddch(y, x, map[y][x]);
                    x++;
                }
                break;
            case 'b':
                if (map[y + 1][x - 1] == '#' || map[y + 1][x - 1] == '.' || map[y + 1][x - 1] == '*' || map[y + 1][x - 1] == '$' || map[y + 1][x - 1] == '+' || map[y + 1][x - 1] == '&' || map[y + 1][x - 1] == 'd' || map[y + 1][x - 1] == 's' || map[y + 1][x - 1] == 'm' || map[y + 1][x - 1] == 'n' || map[y + 1][x - 1] == '!' || map[y + 1][x - 1] == '~' || map[y + 1][x - 1] == '%' || ('1' <= map[y + 1][x - 1] && map[y + 1][x - 1] <= '3')) {
                    mvaddch(y, x, map[y][x]);
                    y++;
                    x--;
                }
                break;
            case 'n':
                if (map[y + 1][x + 1] == '#' || map[y + 1][x + 1] == '.' || map[y + 1][x + 1] == '*' || map[y + 1][x + 1] == '$' || map[y + 1][x + 1] == '+' || map[y + 1][x + 1] == '&' || map[y + 1][x + 1] == 'd' || map[y + 1][x + 1] == 's' || map[y + 1][x + 1] == 'm' || map[y + 1][x + 1] == 'n' || map[y + 1][x + 1] == '!' || map[y + 1][x + 1] == '~' || map[y + 1][x + 1] == '%' || ('1' <= map[y + 1][x + 1] && map[y + 1][x + 1] <= '3')) {
                    mvaddch(y, x, map[y][x]);
                    y++;
                    x++;
                }
                break;
            case 'u':
                if (map[y - 1][x + 1] == '#' || map[y - 1][x + 1] == '.' || map[y - 1][x + 1] == '*' || map[y - 1][x + 1] == '$' || map[y - 1][x + 1] == '+' || map[y - 1][x + 1] == '&' || map[y - 1][x + 1] == 'd' || map[y - 1][x + 1] == 's' || map[y - 1][x + 1] == 'm' || map[y - 1][x + 1] == 'n' || map[y - 1][x + 1] == '!' || map[y - 1][x + 1] == '~' || map[y - 1][x + 1] == '%' || ('1' <= map[y - 1][x + 1] && map[y - 1][x + 1] <= '3')) {
                    mvaddch(y, x, map[y][x]);
                    y--;
                    x++;
                }
                break;
            case 'e':
                food_menu();
                break;
            case 'i':
                weapon_menu();
                break;
            case 'w':
                if(weapon == 5){
                    mvprintw(0, 0, "You Already Have No Weapon.");
                    getch();
                    mvprintw(0, 0, "                           ");
                }
                else{
                    mvprintw(0, 0, "You Put %s Away.", weapon_names[weapon]);
                    weapon = 5;
                    getch();
                    mvprintw(0, 0, "                           ");
                }
            break;
            case 'z':
                spell_menu();
                break;
    default:
        break;
}

        if(map[y_previous][x_previous] == '*'){
            gold++;
            map[y_previous][x_previous] = '.';
            color_map[y_previous][x_previous] = 12;
            mvprintw(0, 0, "You Found 1 Gold!");
            getch();
            mvprintw(0, 0, "                 ");
        }
        if(map[y_previous][x_previous] == '$'){
            gold += 10;
            map[y_previous][x_previous] = '.';
            color_map[y_previous][x_previous] = 12;
            mvprintw(0, 0, "You Found 1 Black Gold (10 Gold)!");
            getch();
            mvprintw(0, 0, "                                 ");
        }
        if('1' <= map[y_previous][x_previous] && map[y_previous][x_previous] <= '3'){
            int index = map[y_previous][x_previous] - '0';
            food_count[index - 1]++;
            map[y_previous][x_previous] = '.';
            color_map[y_previous][x_previous] = 12;
            mvprintw(0, 0, "You Found 1 Food!");
            getch();
            mvprintw(0, 0, "                 ");
        }
        if(map[y_previous][x_previous] == '&'){
            password = rand() % 9000 + 1000;
            password_time = time(NULL);
            flag = 1;
            continue;
        }
        if(map[y_previous][x_previous] == 'd'){
            weapon_count[0]++;
            map[y_previous][x_previous] = '.';
            color_map[y_previous][x_previous] = 12;
            mvprintw(0, 0, "You Found 10 Daggers!");
            getch();
            mvprintw(0, 0, "                     ");
        }
        if(map[y_previous][x_previous] == 'm'){
            weapon_count[1]++;
            map[y_previous][x_previous] = '.';
            color_map[y_previous][x_previous] = 12;
            mvprintw(0, 0, "You Found 8 Magic Wands!");
            getch();
            mvprintw(0, 0, "                        ");
        }
        if(map[y_previous][x_previous] == 'n'){
            weapon_count[2]++;
            map[y_previous][x_previous] = '.';
            color_map[y_previous][x_previous] = 12;
            mvprintw(0, 0, "You Found 20 Normal Arrows!");
            getch();
            mvprintw(0, 0, "                           ");
        }
        if(map[y_previous][x_previous] == 's'){
            weapon_count[3]++;
            map[y_previous][x_previous] = '.';
            color_map[y_previous][x_previous] = 12;
            mvprintw(0, 0, "You Found 1 Sword!");
            getch();
            mvprintw(0, 0, "                  ");
        }
        if(map[y_previous][x_previous] == '!'){
            spell_count[0]++;
            map[y_previous][x_previous] = '.';
            color_map[y_previous][x_previous] = 12;
            mvprintw(0, 0, "You Found 1 Health Spell!");
            getch();
            mvprintw(0, 0, "                         ");
        }
        if(map[y_previous][x_previous] == '~'){
            spell_count[1]++;
            map[y_previous][x_previous] = '.';
            color_map[y_previous][x_previous] = 12;
            mvprintw(0, 0, "You Found 1 Speed Spell!");
            getch();
            mvprintw(0, 0, "                         ");
        }
        if(map[y_previous][x_previous] == '%'){
            spell_count[2]++;
            map[y_previous][x_previous] = '.';
            color_map[y_previous][x_previous] = 12;
            mvprintw(0, 0, "You Found 1 Damage Spell!");
            getch();
            mvprintw(0, 0, "                         ");
        }
    
        if(flag == 1 && difftime(time(NULL), password_time) <= 30){
            mvprintw(0, 0, "Password: %d", password);
        } else if (flag == 1 && difftime(time(NULL), password_time) > 30){
            flag = 0;
            mvprintw(0, 0, "                     ");
            for (int i = 0; i <= 1; i++) {
                for (int j = 0; j <= 40; j++) {
                    mvaddch(i, j, map[i][j]);
                }
            }
        }
        if(difftime(time(NULL), password_time) > 0 && difftime(time(NULL), password_time) <= 30){
            mvprintw(1, 0, "Time: %.0f seconds", difftime(time(NULL), password_time));
        }

        mvprintw(LINES - 1, 5, "Gold: %d", gold);

        mvprintw(LINES - 1, 20, "Hunger: ++++++++++");
        move(LINES - 1, 28);
        attron(COLOR_PAIR(14));
        for (int i = 0; i < hunger; i++) {
            addch('+');
        }
        attroff(COLOR_PAIR(14));

        show_room(y, x); 
        continue_corridor(y, x, 0);
        mvaddch(y, x, '@');
        refresh();
    }
}

#endif // GENERATE_MAP_H
