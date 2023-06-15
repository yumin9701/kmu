#include "snake.h"
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <ncurses.h>
#include <unistd.h>

using namespace std;

// 외부 변수와 함수 선언
extern int map[4][25][35];
extern void appear_poison(int stage_num, WINDOW *win1);
extern void appear_growth(int stage_num, WINDOW *win1);

extern void disappear_poison(int stage_num, WINDOW *win1);
extern void disappear_growth(int stage_num, WINDOW *win1);

extern void increase_length(WINDOW *win1);
extern vector<pair<int, int>> reduce_length(WINDOW *win1);
extern pair<int, int> getHead();
extern vector<pair<int, int>> vector_p;
extern vector<pair<int, int>> vector_g;

// 미션 상태를 나타내는 변수들
char mission_size = 'X';
char mission_gr = 'X';
char mission_p = 'X';
char mission_gt = 'X';

// 미션 목표 수
int num_mission_size = 4;
int num_mission_gr = 1;
int num_mission_p = 0;
int num_mission_gt = 0;

// 새 창 생성 함수
void newWindow(float y, float x) {
    clear();
    initscr();
    noecho();
    getmaxyx(stdscr, y, x);
}

// 게임 시작 창 출력 함수
void start_window(float y, float x) {
    clear();
    initscr();
    noecho();
    getmaxyx(stdscr, y, x);

    mvprintw(y / 2, (x - 16) / 2, "Snake Game");
    mvprintw((y / 2) + 3, (x - 18) / 2, "Press the key");

    refresh();
}

// 게임 종료 창 출력 함수
void gameOver(float y, float x) {
    clear();
    initscr();
    noecho();
    getmaxyx(stdscr, y, x);

    mvprintw(y / 2, (x - 16) / 2, "Game Over");

    refresh();
}

// 사용자 입력 받는 함수
int UserInput() {
    int UserInput = getch();
    refresh();
    endwin();
    clear();
    return UserInput;
}

// 게임 시작 화면 출력 및 입력 처리 함수
int startGame(float y, float x) {
    start_window(y, x);
    return UserInput();
}

// 게임 종료 화면 출력 및 입력 처리 함수
int finishWindow(float y, float x) {
    gameOver(y, x);
    return UserInput();
}

// 스테이지 클리어 화면 출력 함수
void stage_clear(float y, float x, int level) {
    clear();
    initscr();
    noecho();

    getmaxyx(stdscr, y, x);
    WINDOW *win = newwin(8, 30, (y - 5) / 2, (x - 30) / 2);
    box(win, 0, 0);
    refresh();

    if (level <= 3) {
        mvwprintw(win, 2, 8, "Let's start Game!");
        mvwprintw(win, 4, 10, "Level %d", level + 1);
        mvwprintw(win, 6, 4, "Press Enter to continue.");
    } else if (level == 4) {
        mvwprintw(win, 2, 8, "Congratulations!");
        mvwprintw(win, 4, 6, "You passed the game!");
        mvwprintw(win, 6, 4, "Press Enter to exit.");
    }

    wrefresh(win);

    int input = 0;
    while (input != '\n') {
        input = getch();
    }

    delwin(win);
    clear();
    refresh();
}

// 뱀 위치 초기화 함수
void Snake::reset_position(int r, int c) {
    snake_body.clear();
    for (int i = 0; i < 3; i++) {
        snake_body.push_back(make_pair(c / 2, r / 2));
    }
    direction = make_pair(0, 1);
}

// 게임 맵 그리기 함수
void drawGameMap(WINDOW *win, Snake &snake, char *table, int row, int col) {
    werase(win);

    for (int i = 0; i < (row * col); i++) {
        if (table[i] != ' ') {
            int y = i / col;
            int x = i - (y * col);
            int ch;
            switch (table[i]) {
            case '0': ch = ' '; break;
            case '1': ch = '#'; break;
            case '2': ch = '@'; break;
            case '3': ch = 'H'; break;
            case '4': ch = 'B'; break;
            case '5': ch = '+'; break;
            case '6': ch = '-'; break;
            case '8': ch = 'X'; break;
            case '9': ch = 'G'; break;
            }
            mvwaddch(win, y, x, ch);
        }
    }
    wrefresh(win);
}

// 맵 업데이트 함수
void updateMap(Snake &snake, int map[25][35]) {
    snake.make_gate(map);
}

// 점수 출력 함수
void print_score(WINDOW *w, int snake_length, int level, int growth_item, int poison_item, int Gate, int num_mission_size) {
    werase(w);
    wborder(w, '|', '|', '-', '-', '*', '*', '*', '*');
    mvwprintw(w, 1, 15, "Score Board");
    mvwprintw(w, 3, 10, "B: %d / %d ", snake_length, num_mission_size);
    mvwprintw(w, 5, 10, "+: %d ", growth_item);
    mvwprintw(w, 7, 10, "-: %d ", poison_item);
    mvwprintw(w, 9, 10, "G : %d ", Gate);
    wrefresh(w);
}

// 미션 출력 함수
void print_mission(WINDOW *w, int level) {
    werase(w);
    wborder(w, '|', '|', '-', '-', '*', '*', '*', '*');
    mvwprintw(w, 1, 17, "Mission");
    mvwprintw(w, 3, 7, "B:   %d ( %c )", num_mission_size, mission_size);
    mvwprintw(w, 5, 7, "+:   %d  ( %c )", num_mission_gr, mission_gr);
    mvwprintw(w, 7, 7, "-:   %d  ( %c )", num_mission_p, mission_p);
    mvwprintw(w, 9, 7, "G:   %d  ( %c )", num_mission_gt, mission_gt);
    wrefresh(w);
}

// 미션 설정 함수
void setMission(Snake &snake, WINDOW *win1) {
    if (!vector_g.empty()) {
        pair<int, int> head = snake.getHead();
        if (head == vector_g.back()) {
            snake.increase_length(win1);
            snake.change_length();
            snake.growth_item++;
        }
    }

    if (!vector_p.empty()) {
        pair<int, int> head = snake.getHead();
        if (head == vector_p.back()) {
            snake.reduce_length(win1);
            snake.change_length();
            snake.poison_item++;
        }
    }

    if (snake.getSize() == num_mission_size) {
        mission_size = 'V';
    }

    if (snake.growth_item == num_mission_gr) {
        mission_gr = 'V';
    }

    if (snake.poison_item == num_mission_p) {
        mission_p = 'V';
    }

    if (snake.getGateCnt() == num_mission_gt) {
        mission_gt = 'V';
    }
}

// 다음 레벨로 진행하는 함수
void nextLevel(Snake &snake, WINDOW *win1) {
    if ((mission_size == 'V') && (mission_gt == 'V') && (mission_gr == 'V') && (mission_p == 'V')) {
        stage_clear(0, 0, snake.getLevel());
        snake.resize(3);
        snake.growth_item = 0;
        snake.poison_item = 0;
        snake.gate_count(0);
        snake.reset_position(snake.getRow(), snake.getCol());
        mission_size = 'X';
        mission_gr = 'X';
        mission_p = 'X';
        mission_gt = 'X';
        disappear_growth(snake.getLevel() - 1, win1);
        disappear_poison(snake.getLevel() - 1, win1);
        snake.set_level(snake.getLevel() + 1);

        if (snake.getLevel() == 2) {
            num_mission_size = 4;
            num_mission_gr = 1;
            num_mission_p = 1;
            num_mission_gt = 1;
        } else if (snake.getLevel() == 3) {
            num_mission_size = 5;
            num_mission_gr = 1;
            num_mission_p = 1;
            num_mission_gt = 1;
        } else if (snake.getLevel() == 4) {
            num_mission_size = 5;
            num_mission_gr = 1;
            num_mission_p = 1;
            num_mission_gt = 2;
        }

        if (snake.getLevel() == 5) {
            snake.setEnd(true);
        }
    }
}

// 게임 실행 함수
void game() {
    float x, y;
    initscr();
    noecho();
    cbreak();

    getmaxyx(stdscr, y, x);
    WINDOW *win1 = newwin(25, 35, 0, 0);
    WINDOW *scoreBoard = newwin(11, 40, 0, 45);
    wrefresh(scoreBoard);
    WINDOW *mission = newwin(11, 40, 13, 45);
    wrefresh(mission);

    Snake snake(25, 35);

    int grow_count = 0;
    int poison_count = 0;
    int map_count = 0;

    while (!snake.getEnd()) {
        WINDOW *win1 = newwin(25, 35, 0, 0);  // 게임용 새 창 생성
        print_score(scoreBoard, snake.getLen(), snake.getLevel(), snake.growth_item, snake.poison_item, snake.getGateCnt(), num_mission_size);  // 점수를 점수판 창에 출력
        print_mission(mission, snake.getLevel());  // 미션을 미션 창에 출력

        srand(time(NULL));
        char *map_table = snake.map_changer(map[snake.getLevel() - 1]);  // 현재 레벨의 맵 테이블 가져오기
        nodelay(win1, TRUE);
        keypad(win1, TRUE);
        refresh();

        wrefresh(win1);
        drawGameMap(win1, snake, map_table, snake.getRow(), snake.getCol());
        setMission(snake, win1);
        if (map_count == 0) {
            updateMap(snake, map[snake.getLevel() - 1]);
        }
        map_count += 1;
        if (map_count == 80) {
            snake.del_gate(map[snake.getLevel() - 1]);
            updateMap(snake, map[snake.getLevel() - 1]);
            map_count = 1;
        }
        if (grow_count == 0) {
            appear_growth(snake.getLevel() - 1, win1);
        }
        grow_count += 1;
        if (grow_count == 75) {
            disappear_growth(snake.getLevel() - 1, win1);
            appear_growth(snake.getLevel() - 1, win1);
            grow_count = 1;
        }
        if (poison_count == 0) {
            appear_poison(snake.getLevel() - 1, win1);
        }
        poison_count += 1;
        if (poison_count == 75) {
            disappear_poison(snake.getLevel() - 1, win1);
            appear_poison(snake.getLevel() - 1, win1);
            poison_count = 1;
        }

        int input = wgetch(win1);
        char d = snake.getDir();
        switch (input) {
        case KEY_UP:
            if (d != 'u' && d != 'd')
                snake.turn_head(0);
            else if (d == 'd') {
                snake.setEnd(true);
            }
            break;
        case KEY_DOWN:
            if (d != 'd' && d != 'u')
                snake.turn_head(2);
            else if (d == 'u') {
                snake.setEnd(true);
            }
            break;
        case KEY_LEFT:
            if (d != 'l' && d != 'r')
                snake.turn_head(3);
            else if (d == 'r') {
                snake.setEnd(true);
            }
            break;
        case KEY_RIGHT:
            if (d != 'r' && d != 'l')
                snake.turn_head(1);
            else if (d == 'l') {
                snake.setEnd(true);
            }
            break;
        }
        if (snake.getSize() < 3) {
            snake.setEnd(TRUE);
        }

        snake.move_body();
        snake.move_head(map[snake.getLevel() - 1]);
        nextLevel(snake, win1);
        if (snake.getEnd()) {
            break;
        }
        usleep(snake.getSpeed());
    }
}

int main() {
    if (startGame(0, 0) != ' ') {
        game();
    }

    finishWindow(0, 0);
    endwin();
    return 0;
}

