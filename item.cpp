#include <ncurses.h>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
#include <string>
#include <vector>
#include "position.cpp"

using namespace std;

extern int map[4][25][35]; // 4개의 스테이지를 가진 25x35 크기의 맵 배열을 외부에서 참조
extern WINDOW* win1; // 외부에서 참조하는 윈도우 포인터 변수

vector<position> vector_g; // growth 아이템의 위치를 저장하는 벡터
vector<position> vector_p; // poison 아이템의 위치를 저장하는 벡터

position growth_position; // growth 아이템의 현재 위치
position poison_position; // poison 아이템의 현재 위치


void make_item(int stage_num, vector<position>& item_vector, position& item_position, int item_value) {
    item_position = item_vector.back(); // 아이템의 위치를 벡터의 가장 뒤의 요소로 설정
    map[stage_num][item_position.y][item_position.x] = item_value; // 맵 배열에 아이템의 값을 저장
}

void appear_item(int stage_num, vector<position>& item_vector, position& item_position, int item_value, WINDOW* win1) {
    nodelay(win1, true); // 입력 대기 시간을 설정하여 비차단 모드로 설정
    while (1) {
        if (map[stage_num][item_position.random().y][item_position.random().x] != 0) {
            item_position.random(); // 아이템의 위치를 랜덤으로 설정
        } else {
            break;
        }
    }
    item_vector.push_back(item_position); // 아이템의 위치를 벡터에 추가
    make_item(stage_num, item_vector, item_position, item_value); // 아이템을 생성하여 맵에 추가
}

void disappear_item(int stage_num, vector<position>& item_vector, position& item_position, int item_value, WINDOW* win1) {
    nodelay(win1, true); // 입력 대기 시간을 설정하여 비차단 모드로 설정
    if (!item_vector.empty()) {
        item_position = item_vector.back(); // 아이템의 위치를 벡터의 가장 뒤의 요소로 설정
        map[stage_num][item_position.y][item_position.x] = 0; // 맵 배열에서 아이템의 값을 초기화
        item_vector.pop_back(); // 벡터에서 아이템의 위치 제거
    }
}

void make_grow(int stage_num) {
    make_item(stage_num, vector_g, growth_position, 5); // growth 아이템 생성
}

void make_poison(int stage_num) {
    make_item(stage_num, vector_p, poison_position, 6); // poison 아이템 생성
}

void appear_growth(int stage_num, WINDOW* win1) {
    appear_item(stage_num, vector_g, growth_position, 5, win1); // growth 아이템 출현
}

void appear_poison(int stage_num, WINDOW* win1) {
    appear_item(stage_num, vector_p, poison_position, 6, win1); // poison 아이템 출현
}

void disappear_growth(int stage_num, WINDOW* win1) {
    disappear_item(stage_num, vector_g, growth_position, 5, win1); // growth 아이템 사라짐
}

void disappear_poison(int stage_num, WINDOW* win1) {
    disappear_item(stage_num, vector_p, poison_position, 6, win1); // poison 아이템 사라짐
}
