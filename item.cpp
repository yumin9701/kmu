#include <ncurses.h>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
#include <string>
#include <vector>
#include "position.cpp"

using namespace std;

extern int map[4][25][35];
extern WINDOW* win1;

vector<position> vector_g;
vector<position> vector_p;

position growth_position;
position poison_position;


void make_item(int stage_num, vector<position>& item_vector, position& item_position, int item_value) {
    item_position = item_vector.back();
    map[stage_num][item_position.y][item_position.x] = item_value;
}

void appear_item(int stage_num, vector<position>& item_vector, position& item_position, int item_value, WINDOW* win1) {
    nodelay(win1, true);
    while (1) {
        if (map[stage_num][item_position.random().y][item_position.random().x] != 0) {
            item_position.random();
        } else {
            break;
        }
    }
    item_vector.push_back(item_position);
    make_item(stage_num, item_vector, item_position, item_value);
}

void disappear_item(int stage_num, vector<position>& item_vector, position& item_position, int item_value, WINDOW* win1) {
    nodelay(win1, true);
    if (!item_vector.empty()) {
        item_position = item_vector.back();
        map[stage_num][item_position.y][item_position.x] = 0;
        item_vector.pop_back();
    }
}

void make_grow(int stage_num) {
    make_item(stage_num, vector_g, growth_position, 5);
}

void make_poison(int stage_num) {
    make_item(stage_num, vector_p, poison_position, 6);
}

void appear_growth(int stage_num, WINDOW* win1) {
    appear_item(stage_num, vector_g, growth_position, 5, win1);
}

void appear_poison(int stage_num, WINDOW* win1) {
    appear_item(stage_num, vector_p, poison_position, 6, win1);
}

void disappear_growth(int stage_num, WINDOW* win1) {
    disappear_item(stage_num, vector_g, growth_position, 5, win1);
}

void disappear_poison(int stage_num, WINDOW* win1) {
    disappear_item(stage_num, vector_p, poison_position, 6, win1);
}

