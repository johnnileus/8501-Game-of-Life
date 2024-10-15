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
        delete tempGrid;

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

class BasePattern {
public:
    int w;
    int h;
    int cellCount;
    vector<bool> pattern;

    BasePattern(vector<bool> newPat, int width, int height) {
        pattern = newPat;
        w = width;
        h = height;
        cellCount = w * h;
    }
};

bool matchCoordinate(Grid grid, BasePattern pat, int x, int y) {
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
            if (!leftCol && !rightCol) {
                totalMatches++;
            } else {
                break;
            }
        }

        if (totalMatches == pat.w + 2 + pat.h) {
            cout << "MATCH: " << x << " " << y << endl;
            return true;
        }
    }
    return false;
}

vector<pair<int,int>>* findInGrid(Grid grid, BasePattern pat) {
    vector<pair<int, int>>* matches = new vector<pair<int,int>>();

    for (int y = 0; y < grid.h -pat.h + 1; ++y) {
        for (int x = 0; x < grid.w - pat.w + 1; ++x) {

            if (matchCoordinate(grid, pat, x, y)) {
                matches->push_back({x, y});
            }

        }
    }
    for(auto& match: *matches) {
        cout << ":" << match.first << " "<<match.second << endl;
    }
    return matches;
}

class Pattern;

class MatchedPattern {
public:
    int x;
    int y;
    int stage;
    int streak;
    Pattern* pattern;

    MatchedPattern(int newx, int newy, int newstage, Pattern* ptrn) {
        x = newx;
        y = newy;
        stage = newstage;
        streak = 1;
        pattern = ptrn;
    }

private:
};

class Pattern {
public:
    int period;
    vector<pair<int, int>> offsets;
    vector<BasePattern> patterns;

    Pattern(vector<BasePattern> pats, vector<pair<int,int>> offs) {
        period = pats.size();
        patterns = pats;
        offsets = offs;
    }

    vector<MatchedPattern>* search(Grid grid) {
        vector<MatchedPattern>* matchedPatterns = new vector<MatchedPattern>;
        for (int i = 0; i < period; ++i) {
            vector<pair<int, int>>* matches = findInGrid(grid, patterns[i]);
            for (auto match : *matches) {
                matchedPatterns->push_back(MatchedPattern(match.first, match.second, i, this));
            }
            delete matches;
        }
        return matchedPatterns;
    }
};



int main() {
    //setup
    srand(2);
    Grid grid = Grid();


    // if a grid is loaded;
    bool simulating = false;


    vector<bool> arr = {1,1,1,1};
    BasePattern block = BasePattern(arr, 2, 2);

    arr = {0,1,0,
           1,0,1,
           1,0,1,
           0,1,0};
    BasePattern beehive = BasePattern(arr, 3, 4);

    arr = {1,1,1};
    vector<pair<int,int>> offsets = {{1,-1}, {-1, 1}};
    Pattern blinker = Pattern({BasePattern(arr, 1, 3), BasePattern(arr, 3, 1)}, offsets);


    vector<MatchedPattern> matchedPatterns;
    while (true) {
        string input;

        if (simulating) {
            cout << "cmds: step, save, match, exit: ";
            cin >> input;
            if (input.substr(0,4) == "exit") {
                simulating = false;
                matchedPatterns.clear();
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

                for (auto& pat: matchedPatterns) {
                    pat.stage = (pat.stage+1) % pat.pattern->period;
                }


                grid.printGrid();
            }
            else if (input.substr(0,5) == "match") {

                for (int i = 0; i < matchedPatterns.size(); ++i) {
                    MatchedPattern *pat = &matchedPatterns[i];

                    cout << "Pattern: " << pat->x << pat->y << ", streak:" << pat->streak << endl;
                    pair<int,int> offset = pat->pattern->offsets[pat->stage];

//                    cout << "bwuh: " << pat.stage << " " << pat.x + offset.first << " " << pat.y + offset.second;
//                    cout << endl;
                    if (matchCoordinate(grid, pat->pattern->patterns[pat->stage],
                                      pat->x + offset.first, pat->y + offset.second)) {
                        cout << "AAAAAA";
                        pat->streak++;
                        pat->x += offset.first;
                        pat->y += offset.second;
                    } else {
                        //remove pattern from vector
                    }
                }



                vector<MatchedPattern>* newPats = blinker.search(grid);
                matchedPatterns.insert(matchedPatterns.end(), newPats->begin(), newPats->end());
                delete newPats;
                cout << "A" << matchedPatterns.size();



//                findInGrid(grid, block);
//                findInGrid(grid, beehive);
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
