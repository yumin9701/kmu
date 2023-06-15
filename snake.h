#include <iostream>
#include <vector>
#include <string.h>
#include <cstdlib>
#include "position.cpp"
using namespace std;


class Snake {
private:
    vector<pair<int, int>> snake_body;  // 뱀의 몸통 좌표를 저장하는 벡터
    pair<int, int> direction;  // 뱀의 이동 방향
    const int row, col;  // 게임 맵의 행과 열 크기
    vector<pair<int, int>> wall;  // 벽 좌표를 저장하는 벡터
    pair<int, int> gate[2];  // 게이트(문) 좌표를 저장하는 배열
    bool end;  // 게임 종료 여부
    int speed;  // 뱀의 이동 속도
    char* map_change;  // 게임 맵을 문자열로 변환하여 저장하는 배열
    int Gate;  // 게이트(문) 개수
    int level;  // 게임 레벨
    int snake_length;  // 현재 뱀의 길이

public:
    Snake(int r, int c);
    ~Snake();
    int growth_item = 0;  // 성장 아이템 획득 개수
    int poison_item = 0;  // 독 아이템 획득 개수
    void turn_head(int d);  // 뱀의 이동 방향 설정
    void move_head(int map[25][35]);  // 뱀의 머리를 이동시킴
    void move_body();  // 뱀의 몸통을 이동시킴
    char* map_changer(int map[25][35]);  // 맵을 리스트 형태로 변환하여 반환
    int gate_dir(pair<int, int> gate, int map[25][35]);  // 게이트의 방향을 반환
    char getDir();  // 뱀의 현재 이동 방향을 반환
    void setEnd(bool e);  // 게임 종료 여부를 설정
    bool getEnd();  // 게임 종료 여부를 반환
    int getSpeed();  // 게임의 속도를 반환
    int getRow();  // 게임 맵의 행 개수를 반환
    int getCol();  // 게임 맵의 열 개수를 반환
    void make_gate(int map[25][35]);  // 게이트 생성
    void del_gate(int map[25][35]);  // 게이트 제거
    void gate_count(int i);  // 게이트 카운트 설정
    int getGateCnt();  // 게이트 카운트를 반환
    int getLevel();  // 현재 레벨을 반환
    int getLen();  // 뱀의 길이를 반환
    int getSize();  // 뱀의 크기를 반환
    void reset_position(int r, int c) ;
    void set_level(int new_level);  // 레벨 설정
    void reduce_length(WINDOW* win1);  // 뱀의 길이를 줄임
    void increase_length(WINDOW* win1);  // 아이템을 먹어 뱀의 길이를 늘림
    pair<int, int> getHead();  // 뱀의 머리 위치를 반환
    void resize(int new_size);  // 뱀의 크기 조절
    void change_length();  // 뱀의 길이 변경
};
