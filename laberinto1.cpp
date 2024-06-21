#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <chrono>

using namespace std;

const int N = 8;
const char WALL = '#';
const char PATH = '0';
const char VISITED = '*';
const char START = 'S';
const char END = 'E';

struct Point {
    int x, y;
    Point(int _x, int _y) : x(_x), y(_y) {}
};

void generateMaze(vector<vector<char>>& maze);
bool solveMazeBacktracking(vector<vector<char>>& maze, int x, int y);
bool solveMazeBFSWithPath(vector<vector<char>>& maze, vector<Point>& path);
void printMaze(const vector<vector<char>>& maze);
void printMazeWithPause(const vector<vector<char>>& maze);

int main() {
    srand(time(0));
    string command;

    while (true) {
        cout << "Escribe 'iniciar' para generar un nuevo laberinto o 'salir' para terminar: ";
        cin >> command;

        if (command == "salir") {
            break;
        } else if (command != "iniciar") {
            cout << "Comando no reconocido. Intenta de nuevo.\n";
            continue;
        }

        vector<vector<char>> maze(N, vector<char>(N, WALL));

        // Generar el laberinto
        generateMaze(maze);

        // Imprimir el laberinto generado
        cout << "Laberinto generado:\n";
        printMaze(maze);

        // Resolver el laberinto usando Backtracking
        vector<vector<char>> mazeCopy = maze;
        if (solveMazeBacktracking(mazeCopy, N-1, 0)) {
            cout << "Solucion con Backtracking:\n";
            printMazeWithPause(mazeCopy);
            cout << "¡Felicidades! Haz resuelto el laberinto.\n";
        } else {
            cout << "No se pudo encontrar una solucion con Backtracking.\n";
        }
    }

    return 0;
}

void generateMaze(vector<vector<char>>& maze) {
    stack<Point> s;
    s.push(Point(N-1, 0));
    maze[N-1][0] = PATH;

    vector<Point> directions = { Point(1, 0), Point(-1, 0), Point(0, 1), Point(0, -1) };

    while (!s.empty()) {
        Point p = s.top();
        vector<Point> neighbors;

        for (auto d : directions) {
            int nx = p.x + d.x * 2, ny = p.y + d.y * 2;
            if (nx >= 0 && ny >= 0 && nx < N && ny < N && maze[nx][ny] == WALL) {
                neighbors.push_back(d);
            }
        }

        if (!neighbors.empty()) {
            Point dir = neighbors[rand() % neighbors.size()];
            int nx = p.x + dir.x, ny = p.y + dir.y;
            maze[nx][ny] = PATH;
            nx += dir.x;
            ny += dir.y;
            maze[nx][ny] = PATH;
            s.push(Point(nx, ny));
        } else {
            s.pop();
        }
    }

    maze[N-1][0] = START; // Start
    maze[0][N-1] = END;   // End

    // Asegurar que las celdas adyacentes a la salida sean caminos
    vector<Point> endNeighbors = { Point(0, N-2), Point(1, N-1), Point(1, N-2) };
    for (auto d : endNeighbors) {
        if (d.x >= 0 && d.y >= 0 && d.x < N && d.y < N) {
            maze[d.x][d.y] = PATH;
        }
    }
}

bool solveMazeBacktracking(vector<vector<char>>& maze, int x, int y) {
    if (x == 0 && y == N-1) {
        maze[x][y] = VISITED;
        return true;
    }

    vector<Point> directions = { Point(1, 0), Point(-1, 0), Point(0, 1), Point(0, -1) };

    for (auto d : directions) {
        int nx = x + d.x, ny = y + d.y;
        if (nx >= 0 && ny >= 0 && nx < N && ny < N && (maze[nx][ny] == PATH || maze[nx][ny] == END)) {
            if (maze[nx][ny] != END) {
                maze[nx][ny] = VISITED;
                printMazeWithPause(maze); // Imprimir cada paso del recorrido
            }
            if (solveMazeBacktracking(maze, nx, ny)) {
                return true;
            }
            if (maze[nx][ny] != END) {
                maze[nx][ny] = PATH; // Backtrack
                printMazeWithPause(maze); // Imprimir cada paso del backtrack
            }
        }
    }

    return false;
}

void printMaze(const vector<vector<char>>& maze) {
    for (const auto& row : maze) {
        for (const auto& cell : row) {
            cout << cell << ' ';
        }
        cout << '\n';
    }
}

void printMazeWithPause(const vector<vector<char>>& maze) {
    printMaze(maze);
    this_thread::sleep_for(chrono::seconds(1)); // Pausa de 1 segundo para ver el recorrido
    cout << "\n";
}
