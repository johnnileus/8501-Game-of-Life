#include <iostream>
#include <fstream>
#include <random>

using namespace std;

static char emptyChar = ' ';
static char fullChar = '#';

pair<int, int> directions[8] = {
        { -1, -1 }, { 0, -1 }, { 1, -1 },
        { -1,  0 },                { 1,  0 },
        { -1,  1 }, { 0,  1 }, { 1,  1 }};
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
        for (int i = 0; i < amt; ++i) {
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

    bool calculateCell(int x, int y) {
        int cellNumber = y*w + x;
        bool currentVal = grid[cellNumber];

        int neighbours = 0;
        for (int i = 0; i < 8; ++i) {
            int newX = x + directions[i].first;
            int newY = y + directions[i].second;
            //if cell is in grid and is full
            if (newX >= 0 && newX < w && newY >= 0 and newY < h && grid[newY*w + newX]) {
                neighbours++;
            }
        }
        if (currentVal) {
            if (neighbours < 2 || neighbours > 3) {
                return false;
            }
            return true;
        } else {
            if (neighbours == 3) {
                return true;
            }
            return false;
        }


    }

    void stepSimulation() {
        bool tempGrid[cellCount];

        for (int i = 0; i < cellCount; ++i) {
            tempGrid[i] = false;
        }


        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                int cellNumber = y*w + x;
                tempGrid[cellNumber] = calculateCell(x, y);
            }
        }
        for (int i = 0; i < cellCount; ++i) {
            grid[i] = tempGrid[i];
        }

    }

private:

};



int main() {
    //setup
    srand(1);
    Grid grid = Grid();
    grid.initGrid(30,30);
    grid.randomiseGrid(150);



    grid.printGrid();
    grid.stepSimulation();
    cout << endl;
    grid.printGrid();
    string b;



    return 0;
}
