#include "MazeSolver.h"
#include <string>
#include <iostream>
#include <queue>
#include <functional>


using namespace std;

int rowAdj[] = {-1, 0, 0, 1};
int colAdj[] = {0, -1, 1, 0};

void MazeSolver::load(std::ifstream &inputFile) {
    vector<string> textMaze;
    string line;
    while (!inputFile.eof()) {
        getline(inputFile, line);
        if (line[0] == 's') {
            sscanf(line.data(), "%*s %d %*c %d", &m_Start.x, &m_Start.y);
            continue;
        }
        if (line[0] == 'e') {
            sscanf(line.data(), "%*s %d %*c %d", &m_End.x, &m_End.y);
            continue;
        } else {
            textMaze.push_back(line);
        }
    }
    m_Maze.toTiles(textMaze, m_Renderer.getWindowWidth(), m_Renderer.getWindowHeight(), m_Start, m_End);
}

void MazeSolver::init() {
    try {
        m_Renderer.init();
    }
    catch (const runtime_error &ex) {
        cout << ex.what() << endl;
        m_Renderer.quit();
    }
}

double MazeSolver::tileDist(Tile &t1, Tile &t2) {
    return sqrt(pow((t1.m_i - t2.m_i), 2) + pow((t1.m_j - t2.m_j), 2));
}

void MazeSolver::solve(char algo) {
    switch (algo) {
        case '1':
            randomSearch();
            break;
        case '2':
            DFS();
            break;
        case '3':
            BFS();
            break;
        case '4':
            greedySearch();
            break;
        case '5':
            AStar();
            break;
    }

    cout << "Path not found" << endl;
    loop();
}

int MazeSolver::update() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            m_Renderer.quit();
            return 0;
        }
    }
    m_Maze.draw(m_Renderer);
    m_Renderer.present();
    m_Renderer.redrawBackground();
    SDL_Delay(5);
    return 1;
}

void MazeSolver::backtrace(Tile *tile) {
    while (tile->m_Pred) {
        if (!update())
            exit(1);
        SDL_Delay(20);
        tile->m_State = State::PATH;
        tile = tile->m_Pred;
    }
    loop();
}

void MazeSolver::BFS() {

    if (m_Start.x == m_End.x && m_Start.y == m_End.y) {
        cout << "Path length: " << 0 << endl;
        cout << "Nodes expanded: " << 0 << endl;
        backtrace(&m_Maze.m_Tiles[m_Start.x][m_Start.y]);
    }

    queue<Tile> q;
    q.push(m_Maze.m_Tiles[m_Start.y][m_Start.x]);
    m_Maze.m_Tiles[m_Start.y][m_Start.x].m_State = State::OPENED;
    m_Maze.m_Tiles[m_Start.y][m_Start.x].m_Pred = nullptr;

    while (!q.empty()) {

        if (!update())
            exit(0);

        Tile tile = q.front();
        q.pop();
        m_ExpandedNodes++;
        for (int side = 0; side < 4; side++) {
            int i = tile.m_i + rowAdj[side];
            int j = tile.m_j + colAdj[side];

            if (!m_Maze.m_Tiles[i][j].m_Wall && m_Maze.m_Tiles[i][j].m_State == State::FRESH) {

                m_Maze.m_Tiles[i][j].m_State = State::OPENED;
                m_Maze.m_Tiles[i][j].m_Distance = tile.m_Distance + 1;
                m_Maze.m_Tiles[i][j].m_Pred = &m_Maze.m_Tiles[tile.m_i][tile.m_j];
                if (i == m_End.y && j == m_End.x) {
                    cout << "Path length: " << tile.m_Distance + 1 << endl;
                    cout << "Nodes expanded: " << m_ExpandedNodes << endl;
                    backtrace(&m_Maze.m_Tiles[i][j]);
                }
                q.push(m_Maze.m_Tiles[i][j]);
            }
            m_Maze.m_Tiles[tile.m_i][tile.m_j].m_State = State::CLOSED;
        }
    }
}

void MazeSolver::DFS() {
    m_Maze.m_Tiles[m_Start.y][m_Start.x].m_Pred = nullptr;
    m_Maze.m_Tiles[m_Start.y][m_Start.x].m_Distance = 0;
    DFSRec(m_Maze.m_Tiles[m_Start.y][m_Start.x]);
}

void MazeSolver::DFSRec(Tile &tile) {

    if (m_Start.x == m_End.x && m_Start.y == m_End.y) {
        cout << "Path length: " << 0 << endl;
        cout << "Nodes expanded: " << 0 << endl;
        backtrace(&m_Maze.m_Tiles[m_Start.x][m_Start.y]);
    }

    tile.m_State = State::OPENED;
    if (!update())
        exit(0);
    m_ExpandedNodes++;
    for (int side = 0; side < 4; side++) {
        int i = tile.m_i + rowAdj[side];
        int j = tile.m_j + colAdj[side];
        if (!m_Maze.m_Tiles[i][j].m_Wall && m_Maze.m_Tiles[i][j].m_State == State::FRESH) {
            m_Maze.m_Tiles[i][j].m_Pred = &m_Maze.m_Tiles[tile.m_i][tile.m_j];
            m_Maze.m_Tiles[i][j].m_Distance = tile.m_Distance + 1;
            if (i == m_End.y && j == m_End.x) {
                cout << "Path length: " << tile.m_Distance + 1 << endl;
                cout << "Nodes expanded: " << m_ExpandedNodes << endl;
                backtrace(&m_Maze.m_Tiles[i][j]);
            }
            DFSRec(m_Maze.m_Tiles[i][j]);
        }
    }
    tile.m_State = State::CLOSED;
}

void MazeSolver::randomSearch() {

    if (m_Start.x == m_End.x && m_Start.y == m_End.y) {
        cout << "Path length: " << 0 << endl;
        cout << "Nodes expanded: " << 0 << endl;
        backtrace(&m_Maze.m_Tiles[m_Start.x][m_Start.y]);
    }

    vector<Tile> vec;
    vec.push_back(m_Maze.m_Tiles[m_Start.y][m_Start.x]);
    m_Maze.m_Tiles[m_Start.y][m_Start.x].m_State = State::OPENED;
    m_Maze.m_Tiles[m_Start.y][m_Start.x].m_Pred = nullptr;

    while (!vec.empty()) {
        if (!update())
            exit(0);
        size_t pos = std::rand() % vec.size();
        Tile tile = vec[pos];
        vec.erase(vec.begin() + pos);
        m_ExpandedNodes++;
        for (int side = 0; side < 4; side++) {
            int i = tile.m_i + rowAdj[side];
            int j = tile.m_j + colAdj[side];

            if (!m_Maze.m_Tiles[i][j].m_Wall && m_Maze.m_Tiles[i][j].m_State == State::FRESH) {

                m_Maze.m_Tiles[i][j].m_State = State::OPENED;
                m_Maze.m_Tiles[i][j].m_Distance = tile.m_Distance + 1;
                m_Maze.m_Tiles[i][j].m_Pred = &m_Maze.m_Tiles[tile.m_i][tile.m_j];
                if (i == m_End.y && j == m_End.x) {
                    cout << "Path length: " << tile.m_Distance + 1 << endl;
                    cout << "Nodes expanded: " << m_ExpandedNodes << endl;
                    backtrace(&m_Maze.m_Tiles[i][j]);
                }
                vec.push_back(m_Maze.m_Tiles[i][j]);
            }
            m_Maze.m_Tiles[tile.m_i][tile.m_j].m_State = State::CLOSED;
        }
    }
}

void MazeSolver::greedySearch() {

    if (m_Start.x == m_End.x && m_Start.y == m_End.y) {
        cout << "Path length: " << 0 << endl;
        cout << "Nodes expanded: " << 0 << endl;
        backtrace(&m_Maze.m_Tiles[m_Start.x][m_Start.y]);
    }

    priority_queue<Tile, vector<Tile>, less<>> q;
    q.push(m_Maze.m_Tiles[m_Start.y][m_Start.x]);
    m_Maze.m_Tiles[m_Start.y][m_Start.x].m_State = State::OPENED;
    m_Maze.m_Tiles[m_Start.y][m_Start.x].m_Pred = nullptr;
    m_Maze.m_Tiles[m_Start.y][m_Start.x].m_EuclideanDist = tileDist(m_Maze.m_Tiles[m_Start.y][m_Start.x],
                                                                    m_Maze.m_Tiles[m_End.y][m_End.x]);
    while (!q.empty()) {

        if (!update())
            exit(0);

        Tile tile = q.top();
        q.pop();
        m_ExpandedNodes++;
        for (int side = 0; side < 4; side++) {
            int i = tile.m_i + rowAdj[side];
            int j = tile.m_j + colAdj[side];

            if (!m_Maze.m_Tiles[i][j].m_Wall && m_Maze.m_Tiles[i][j].m_State == State::FRESH) {

                m_Maze.m_Tiles[i][j].m_State = State::OPENED;
                m_Maze.m_Tiles[i][j].m_Distance = tile.m_Distance + 1;
                m_Maze.m_Tiles[i][j].m_Pred = &m_Maze.m_Tiles[tile.m_i][tile.m_j];
                m_Maze.m_Tiles[i][j].m_EuclideanDist = tileDist(m_Maze.m_Tiles[i][j], m_Maze.m_Tiles[m_End.y][m_End.x]);
                if (i == m_End.y && j == m_End.x) {
                    cout << "Path length: " << tile.m_Distance + 1 << endl;
                    cout << "Nodes expanded: " << m_ExpandedNodes << endl;
                    backtrace(&m_Maze.m_Tiles[i][j]);
                }
                q.push(m_Maze.m_Tiles[i][j]);
            }
            m_Maze.m_Tiles[tile.m_i][tile.m_j].m_State = State::CLOSED;
        }
    }
}


void MazeSolver::AStar() {

    if (m_Start.x == m_End.x && m_Start.y == m_End.y) {
        cout << "Path length: " << 0 << endl;
        cout << "Nodes expanded: " << 0 << endl;
        backtrace(&m_Maze.m_Tiles[m_Start.x][m_Start.y]);
    }


    class AStarCompare {
    public:
        bool operator()(Tile &lhs, Tile &rhs) {
            return lhs.m_EuclideanDist + lhs.m_Distance > rhs.m_Distance + rhs.m_EuclideanDist;
        }
    };

    priority_queue<Tile, std::vector<Tile>, AStarCompare> q;
    q.push(m_Maze.m_Tiles[m_Start.y][m_Start.x]);
    m_Maze.m_Tiles[m_Start.y][m_Start.x].m_State = State::OPENED;
    m_Maze.m_Tiles[m_Start.y][m_Start.x].m_Pred = nullptr;
    m_Maze.m_Tiles[m_Start.y][m_Start.x].m_EuclideanDist = tileDist(m_Maze.m_Tiles[m_Start.y][m_Start.x],
                                                                    m_Maze.m_Tiles[m_End.y][m_End.x]);
    while (!q.empty()) {

        if (!update())
            exit(0);

        Tile tile = q.top();
        q.pop();
        m_ExpandedNodes++;
        for (int side = 0; side < 4; side++) {
            int i = tile.m_i + rowAdj[side];
            int j = tile.m_j + colAdj[side];

            if (!m_Maze.m_Tiles[i][j].m_Wall && m_Maze.m_Tiles[i][j].m_State == State::FRESH) {

                m_Maze.m_Tiles[i][j].m_State = State::OPENED;
                m_Maze.m_Tiles[i][j].m_Distance = tile.m_Distance + 1;
                m_Maze.m_Tiles[i][j].m_Pred = &m_Maze.m_Tiles[tile.m_i][tile.m_j];
                m_Maze.m_Tiles[i][j].m_EuclideanDist = tileDist(m_Maze.m_Tiles[i][j], m_Maze.m_Tiles[m_End.y][m_End.x]);
                if (i == m_End.y && j == m_End.x) {
                    cout << "Path length: " << tile.m_Distance + 1 << endl;
                    cout << "Nodes expanded: " << m_ExpandedNodes << endl;
                    backtrace(&m_Maze.m_Tiles[i][j]);
                }
                q.push(m_Maze.m_Tiles[i][j]);
            }
            m_Maze.m_Tiles[tile.m_i][tile.m_j].m_State = State::CLOSED;
        }
    }
}

void MazeSolver::loop() {
    while (update()) {}
    exit(1);
}
