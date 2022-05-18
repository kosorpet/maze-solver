#include <iostream>
#include "MazeSolver.h"

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        cout << "Usage: ./SOLVER [1-5] <maze.txt>\n"
                "1 - Random Search\n"
                "2 - DFS\n"
                "3 - BFS\n"
                "4 - Greedy Search\n"
                "5 - A*\n"
                "<maze.txt>      textfile representing the maze." << endl;
        return 1;
    }

    const char *fileName = argv[2];
    char algo = *argv[1];

    if (argc != 3 || !(algo == '1' || algo == '2' || algo == '3' || algo == '4' || algo == '5')) {
        cout << "Wrong parameters. Algorithms:\n "
                "1 - Random Search\n"
                "2 - DFS\n"
                "3 - BFS\n"
                "4 - Greedy Search\n"
                "5 - A*" << endl;
        return 1;
    }

    MazeSolver solver;
    ifstream mazeFile(fileName);
    if (mazeFile.is_open()) {
        solver.init();
        solver.load(mazeFile);
        solver.solve(algo);
    } else {
        cout << "Unable to open file" << endl;
        return 1;
    }
    return 0;
}