#include <iostream>
#include <random>

using namespace std;

static char emptyChar = '.';
static char fullChar = '#';

class Grid {
public:
    bool* grid;
    int w;
    int h;
    int cellCount;

    void initGrid(int cols, int rows){
        w = cols;
        h = rows;
        cellCount = w * h;
        //create 2d array in heap;
        grid = new bool[cellCount];
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                grid[i*w+j] = false;
            }

        }
    }

    void deleteGrid();

    void printGrid() {
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                grid[i*w + j] ? cout << fullChar : cout << emptyChar;
                cout << " ";
            }
            cout << endl;
        }
    }

    void randomiseGrid(int amt) {
        for (int i = 0; i < 25; ++i) {
            while (true) {
                int randX = rand() % w;
                int randY = rand() % h;

                if (!grid[randY*w + randX]) {
                    grid[randY*w + randX] = true;
                    break;
                }
            }


        }
    }
private:

};



int main() {
    srand(1);
    Grid grid = Grid();
    grid.initGrid(10,10);
    grid.randomiseGrid(25);
    grid.printGrid();
    return 0;
}
