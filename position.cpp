#include <ncurses.h>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
#include <string>
#include <vector>


using namespace std;


class position {
public:
    int x;
    int y;
    position& random() {
        srand(time(NULL));
        x = rand() % 35;
        y = rand() % 25;
        return *this;
    }
};
