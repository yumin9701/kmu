#include "snake.h"
using namespace std;

extern int map[4][25][35];
extern void appear_poison(int stage_num, WINDOW* win1);
extern void appear_growth(int stage_num, WINDOW* win1);
extern void disappear_poison(int stage_num, WINDOW* win1);
extern void disappear_growth(int stage_num, WINDOW* win1);
extern WINDOW* win1;

Snake::Snake(int r, int c) : row(r), col(c) {
    for (int i = 0; i < 3; i++)
        snake_body.emplace_back(col / 2, row / 2 + i);
    turn_head(0);
    end = false;
    speed = 200000;
    map_change = new char[row * col];
    level = 1;
    snake_length = 3;
    gate_count(0);
}

Snake::~Snake() {
    delete[] map_change;
}

void Snake::turn_head(int d) {
    switch (d) {
        case 0: direction = make_pair(0, -1); break;  // 위로 이동
        case 1: direction = make_pair(1, 0); break;   // 오른쪽으로 이동
        case 2: direction = make_pair(0, 1); break;   // 아래로 이동
        case 3: direction = make_pair(-1, 0); break;  // 왼쪽으로 이동
    }
}

int Snake::gate_dir(pair<int, int> gate, int map[25][35]) {
    pair<int, int> A(gate.first, gate.second - 1);  // 게이트 좌측 좌표
    if (map[A.second][A.first] == 0) return 0;
    pair<int, int> B(gate.first + 1, gate.second);  // 게이트 상단 좌표
    if (map[B.second][B.first] == 0) return 1;
    pair<int, int> C(gate.first, gate.second + 1);  // 게이트 우측 좌표
    if (map[C.second][C.first] == 0) return 2;
    pair<int, int> D(gate.first - 1, gate.second);  // 게이트 하단 좌표
    if (map[D.second][D.first] == 0) return 3;
    return -1;
}

void Snake::move_head(int map[25][35]) {
    snake_body[0].first += direction.first;   // 머리의 x 좌표 업데이트
    snake_body[0].second += direction.second; // 머리의 y 좌표 업데이트
    for (int i = 0; i < wall.size(); i++) {
        if (snake_body[0] == wall[i]) {
            if (snake_body[0] == gate[1]) {  // 게이트[1]에 도착한 경우
                snake_body[0].first = gate[0].first;  // 머리를 게이트[0]의 위치로 이동
                snake_body[0].second = gate[0].second;
                turn_head(gate_dir(gate[0], map));  // 머리의 이동 방향 업데이트
                gate_count(1);
                break;
            } else if (snake_body[0] == gate[0]) {  // 게이트[0]에 도착한 경우
                snake_body[0].first = gate[1].first;  // 머리를 게이트[1]의 위치로 이동
                snake_body[0].second = gate[1].second;
                turn_head(gate_dir(gate[1], map));  // 머리의 이동 방향 업데이트
                gate_count(1);
                break;
            } else {
                setEnd(true);  // 게임 종료
                del_gate(map);  // 게이트 제거
            }
        }
    }
}

void Snake::move_body() {
    for (unsigned int i = snake_body.size() - 1; i > 0; --i)
        snake_body[i] = snake_body[i - 1];  // 몸통을 한 칸씩 이동
}

char* Snake::map_changer(int map[25][35]) {
    memset(map_change, ' ', row * col);  // 맵 리스트 초기화
    for (unsigned int i = 0; i < 25; i++) {
        for (int j = 0; j < 35; j++) {
            switch (map[i][j]) {
                case 0: map_change[i * col + j] = '0'; break;  // 빈 공간
                case 1: map_change[i * col + j] = '1'; wall.emplace_back(j, i); break;  // 벽
                case 2: map_change[i * col + j] = '2'; break;  // 성장 아이템
                case 4: map_change[i * col + j] = '4'; break;  // 뱀의 몸통
                case 5: map_change[i * col + j] = '5'; break;
                case 6: map_change[i * col + j] = '6'; break;
                case 8: map_change[i * col + j] = '8'; break;
                case 9: map_change[i * col + j] = '9'; break;
            }
        }
    }
    map_change[snake_body[0].second * col + snake_body[0].first] = '3';  // 머리 좌표
    for (unsigned int i = 1; i < snake_body.size(); ++i)
        map_change[snake_body[i].second * col + snake_body[i].first] = '4';  // 몸통 좌표
    return map_change;
}

char Snake::getDir() {
    if (direction.first == 1) return 'r';  // 오른쪽으로 이동 중
    else if (direction.first == -1) return 'l';  // 왼쪽으로 이동 중
    else if (direction.second == -1) return 'u';  // 위로 이동 중
    else return 'd';  // 아래로 이동 중
}

void Snake::setEnd(bool e) {
    end = e;
}

bool Snake::getEnd() {
    return end;
}

int Snake::getSpeed() {
    return speed;
}

int Snake::getRow() {
    return row;
}

int Snake::getCol() {
    return col;
}

int Snake::getLevel() {
    return level;
}

int Snake::getLen() {
    return snake_length;
}

void Snake::make_gate(int map[25][35]) {
    int randWall = rand() % wall.size();  // 랜덤한 벽 좌표 선택
    int randWall2 = rand() % wall.size();  // 랜덤한 벽 좌표 선택
    if (randWall == randWall2) make_gate(map);  // 두 개의 벽 좌표가 동일하면 다시 선택
    gate[0] = wall[randWall];  // 게이트[0]에 벽 좌표 설정
    gate[1] = wall[randWall2];  // 게이트[1]에 벽 좌표 설정
    map[gate[0].second][gate[0].first] = 8;  // 게이트[0] 좌표에 게이트 번호 설정
    map[gate[1].second][gate[1].first] = 9;  // 게이트[1] 좌표에 게이트 번호 설정
}

void Snake::del_gate(int map[25][35]) {
    map[gate[0].second][gate[0].first] = 1;  // 게이트[0] 좌표를 벽으로 변경
    map[gate[1].second][gate[1].first] = 1;  // 게이트[1] 좌표를 벽으로 변경
    gate[0] = make_pair(0, 0);  // 게이트[0] 좌표 초기화
    gate[1] = make_pair(0, 0);  // 게이트[1] 좌표 초기화
}

void Snake::gate_count(int i) {
    if (i == 0) Gate = 0;
    else Gate += 1;
}

int Snake::getGateCnt() {
    return Gate;
}

int Snake::getSize() {
    return snake_body.size();
}

void Snake::set_level(int new_level) {
    level = new_level;
}

void Snake::reduce_length(WINDOW* win1) {
    disappear_poison(level - 1, win1);
    snake_body.pop_back();  // 뱀의 꼬리를 제거하여 길이 감소
    appear_poison(level - 1, win1);
}

void Snake::increase_length(WINDOW* win1) {
    disappear_growth(level - 1, win1);
    snake_body.push_back(snake_body.back());  // 뱀의 꼬리에 새로운 몸통 추가하여 길이 증가
    appear_growth(level - 1, win1);
}

pair<int, int> Snake::getHead() {
    return snake_body[0];  // 머리 좌표 반환
}

void Snake::resize(int new_size) {
    snake_body.resize(new_size);  // 뱀의 크기 조정
}

void Snake::change_length() {
    snake_length = snake_body.size();
}

