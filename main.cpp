#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <math.h>
#include <chrono>

using namespace std;

static char emptyChar = '.';
static char fullChar = '#';

static int threadCount = 12;

pair<int, int> directions[8] = {
        { -1, -1 }, { 0, -1 }, { 1, -1 },
        { -1,  0 },                { 1,  0 },
        { -1,  1 }, { 0,  1 }, { 1,  1 }};

void printA(){
    cout << "A";
}



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
        cout << endl;
        for (int i = 0; i < h; ++i) {
            for (int j = 0; j < w; ++j) {
                grid[i*w + j] ? cout << fullChar : cout << emptyChar;
                cout << " ";
            }
            cout << "|" <<endl;
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
        bool* tempGrid = new bool[cellCount];

        for (int i = 0; i < cellCount; ++i) {
            tempGrid[i] = false;
        }


//        float cellsPerThread = (float) cellCount/threadCount;
//
//        vector<thread> threads;
//
//        for (int i = 0; i < threadCount; ++i) {
//            int end = round((i+1) * cellsPerThread);
//            int start = round(i * cellsPerThread);
//            int assignedCells = end - start;
//            cout << assignedCells << "A";
//            threads.push_back(thread(&Grid::stepThread, this, tempGrid, start, end));
//        }
//
//        for (thread& t: threads) {
//            t.join();
//        }

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

    void loadFromFile(string fileName) {
        delete grid;


        ifstream file(fileName+".txt");
        if (!file.is_open()) {
            cout << "file failed to open";
            return;
        }

        string line;

        getline(file, line);
        w = stoi(line);
        getline(file, line);
        h = stoi(line);

        cellCount = w * h;
        cout << w << "x" << h << endl;
        grid = new bool[cellCount];
        for (int i = 0; i < cellCount; ++i) {
            grid[i] = false;
        }
        char inp;
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                file.get(inp);
                bool val = false;
                if (inp == '1') {
                    val = true;
                }
                grid[y*w + x] = val;
            }
            file.get(inp); //discard \n
        }



    }

    void saveToFile(string fileName) {
        ofstream file(fileName+".txt");
        file << w << endl << h << endl;
        for (int y = 0; y < h; ++y) {
            for (int x = 0; x < w; ++x) {
                file << grid[y*w + x];
            }
            file << endl;
        }
    }
private:

};

class StaticPattern {
public:
    int w;
    int h;
    int cellCount;
    vector<bool> pattern;

    StaticPattern(vector<bool> newPat, int width, int height) {
        pattern = newPat;
        w = width;
        h = height;
        cellCount = w * h;
    }



private:
};


void findInGrid(Grid grid, StaticPattern pat) {
    for (int y = 0; y < grid.h; ++y) {
        for (int x = 0; x < grid.w; ++x) {

            int totalMatches = 0;
            for (int i = 0; i < pat.cellCount; ++i) {

                int patX = i % pat.w;
                int patY = (i - patX) / pat.w;

                int newX = patX + x;
                int newY = patY + y;

                if (newX < grid.w && newY < grid.h) {
                    if (grid.grid[newY * grid.w + newX] == pat.pattern[i]) {
                        totalMatches++;
                    } else { break; }
                }

            }
            if (totalMatches == pat.cellCount) {
                totalMatches = 0;
                cout << "matching" << x << " " << y << " ";
                for (int a = -1; a < pat.w + 1; ++a) {

                    //top row calculations
                    bool topRow;
                    if (y - 1 < 0) {
                        topRow = 0;
                    } else {
                        topRow = grid.grid[(y - 1) * grid.w + x + a];
                    }

                    //bottom row calculations
                    bool bottomRow;
                    if (y + pat.h >= grid.h) {
                        bottomRow = 0;
                    } else {
                        bottomRow = (grid.grid[(y + pat.h) * grid.w + x + a]);
                    }

                    if (!topRow && !bottomRow) {
                        totalMatches++;
                    } else {
                        break;
                    }
                }

                for (int b = 0; b < pat.h; ++b) {
                    bool leftCol;
                    if (x - 1 < 0) {
                        leftCol = 0;
                    } else {
                        leftCol = grid.grid[(y + b) * grid.w + x - 1];
                    }
                    bool rightCol;
                    if (x - 1 < 0) {
                        rightCol = 0;
                    } else {
                        rightCol = grid.grid[(y + b) * grid.w + x + pat.w];
                    }
                    cout << "| x:" << x << " y:" << y << " b:" << b << " " << leftCol << " " << rightCol << "|";
                    if (!leftCol && !rightCol) {
                        totalMatches++;
                    } else {
                        break;
                    }
                }

                cout << "." << totalMatches << " " << (pat.w + 2) * 2 << endl;
                if (totalMatches == pat.w + 2 + pat.h) {
                    cout << "MATCH: " << x << " " << y;
                }
                cout << endl;
            }
        }
    }

}

int main() {
    //setup
    srand(2);
    Grid grid = Grid();


    // if a grid is loaded;
    bool simulating = false;


    vector<bool> arr = {1,1,1,1};
    StaticPattern block = StaticPattern(arr, 2, 2);

    arr = {0,1,0,
           1,0,1,
           1,0,1,
           0,1,0};
    StaticPattern beehive = StaticPattern(arr, 3, 4);


    while (true) {
        string input;
        if (simulating) {
            cout << "cmds: step, save, match, exit: ";
            cin >> input;
            if (input.substr(0,4) == "exit") {
                simulating = false;
            }
            else if (input.substr(0,4) == "save") {
                cout << "name of file:";
                string name;
                cin >> name;


                grid.saveToFile(name);
            }
            else if (input.substr(0,4) == "step") {


                auto t1 = chrono::high_resolution_clock::now();
                grid.stepSimulation();
                auto t2 = chrono::high_resolution_clock::now();
                auto ms_int = chrono::duration_cast<chrono::milliseconds>(t2-t1);
                cout << ms_int.count() << "ms."<< endl;

                grid.printGrid();
            }
            else if (input.substr(0,5) == "match") {
                findInGrid(grid, block);
                findInGrid(grid, beehive);
            }

        } else {
            cout << "cmds: new, load: ";
            cin >> input;
            if (input.substr(0,3) == "new") {
                int x, y, cellAmt;
                cout << "x: ";

                cin >> x;
                cout << "y: ";
                cin >> y;
                cout << "cell#: ";
                cin >> cellAmt;


                grid.initGrid(x, y);
                grid.randomiseGrid(cellAmt);

                grid.printGrid();

                simulating = true;
            }
            else if (input.substr(0,4) == "load") {
                cout << "name of file to load:";
                string name;
                cin >> name;

                grid.loadFromFile(name);
                grid.printGrid();
                simulating = true;
            }
        }


    }


    return 0;
}
