#include <iostream>
#include <fstream>
#include <random>

using namespace std;

static char emptyChar = ' ';
static char fullChar = '#';

static int threadCount = 12;

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
        cout << endl;
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



int main() {
    //setup
    srand(2);
    Grid grid = Grid();

    // if a grid is loaded;
    bool simulating = false;


    while (true) {
        string input;
        if (simulating) {
            cout << "cmds: step, save, exit: ";
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

                grid.stepSimulation();
                grid.printGrid();
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
