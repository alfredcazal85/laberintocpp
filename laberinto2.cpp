// Agregue, que busque la salida con mayor impetu
#include <iostream> //permite mostrar mensajes en la pantalla
#include <vector> //nos da lista de cosas
#include <stack> //permite apilar cosas
#include <queue>
#include <ctime>
#include <cstdlib>
#include <thread>
#include <chrono>

using namespace std; //es un atajo para usar cosas estandar de c++

const int N = 10; // tamaño del tablero
const char WALL = '#'; // como se vera mi muralla
const char PATH = '0'; // como se vera el camino
const char VISITED = '*'; // como se vera la celda visitada
const char START = 'S'; // inicio
const char END = 'E'; // salida

struct Point { // el puntero que en el tablero tiene que tener una posicion x e y para ir recorriendolo
    int x, y;
    Point(int _x, int _y) : x(_x), y(_y) {}
};

void generateMaze(vector<vector<char>>& maze); // hace el laberinto
bool solveMazeBacktracking(vector<vector<char>>& maze, int x, int y); // trata de resolverlo
bool solveMazeBFSWithPath(vector<vector<char>>& maze, vector<Point>& path); 
void printMaze(const vector<vector<char>>& maze); //muestra el laberinto en pantalla
void printMazeWithPause(const vector<vector<char>>& maze); 

int main() { //comienza el programa 
    srand(time(0)); // hace que el laberinto se configure de manera diferente cada vez que damos iniciar
    string command; // guarda los comandos del usuario

    while (true) { //bucle que sigue repitiendose hasta que digamos parar
        cout << "Escribe 'iniciar' para generar un nuevo laberinto o 'salir' para terminar: ";
        cin >> command;

        if (command == "salir") { //si coloca el programa para
            break;
        } else if (command != "iniciar") { //inicia el juego hasta que encuentra la salida
            cout << "Comando no reconocido. Intenta de nuevo.\n";
            continue;
        }

        vector<vector<char>> maze(N, vector<char>(N, WALL)); //crea el laberinto

        // hace el laberinto con murallas y caminos
        generateMaze(maze);

        // Imprimir el laberinto generado y mostrarlo en pantalla
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

    return 0; // termina el programa
}

void generateMaze(vector<vector<char>>& maze) { //funcion que crea el laberinto, y comienza de la celda inferior izquierda
    stack<Point> s;
    s.push(Point(N-1, 0));
    maze[N-1][0] = PATH;

    vector<Point> directions = { Point(1, 0), Point(-1, 0), Point(0, 1), Point(0, -1) }; //establezco los movimientos posibles

    while (!s.empty()) { //mientras la pila no este vacia seguimos
        Point p = s.top(); //tomamos punto de arriba de la pila
        vector<Point> neighbors; //guarda los vecinos del punto actual

        for (auto d : directions) { //revisa todas las direcciones 
            int nx = p.x + d.x * 2, ny = p.y + d.y * 2; //calculamos posicion del vecino
            if (nx >= 0 && ny >= 0 && nx < N && ny < N && maze[nx][ny] == WALL) { //si vecino esta dentro del laberinto y es una pared añadimos a vecinos
                neighbors.push_back(d);
            }
        }

        if (!neighbors.empty()) { //si hay vecinos elegimos uno al azar
            Point dir = neighbors[rand() % neighbors.size()]; 
            int nx = p.x + dir.x, ny = p.y + dir.y;
            maze[nx][ny] = PATH; //hacemos camino en la direccion elegida
            nx += dir.x;
            ny += dir.y;
            maze[nx][ny] = PATH;
            s.push(Point(nx, ny)); //añadimos el nuevo punto a la pila, tipo el camino recorrido
        } else { //si no hay vecinos quitamos el punto de la pila
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

bool solveMazeBacktracking(vector<vector<char>>& maze, int x, int y) { //funcion para encontrar la salida del laberinto
    if (x == 0 && y == N-1) { //si llegamos al final se termina
        maze[x][y] = VISITED;
        return true;
    }

    vector<Point> directions = { Point(1, 0), Point(-1, 0), Point(0, 1), Point(0, -1) }; //direcciones en las que podemos movernos

    for (auto d : directions) { //revisa direcciones 
        int nx = x + d.x, ny = y + d.y; //calcula posicion vecino
        if (nx >= 0 && ny >= 0 && nx < N && ny < N && (maze[nx][ny] == PATH || maze[nx][ny] == END)) { //si vecino es camino o el fin seguimos
            if (maze[nx][ny] == END) {//si llegamos al fin marcamos punto y terminamos
                maze[nx][ny] = VISITED; //marcamos el punto como visitado 
                printMazeWithPause(maze); 
                return true;
            }
            maze[nx][ny] = VISITED;
            printMazeWithPause(maze); // Imprimir cada paso del recorrido
            if (solveMazeBacktracking(maze, nx, ny)) { //llamamos a la funcion para seguir buscando la salida
                return true;
            }
            maze[nx][ny] = PATH; // Si no encontramos salida, volvemos atras y desmarcamos el punto
            printMazeWithPause(maze); // Imprimir cada paso del backtrack
        }
    }

    return false;
}

void printMaze(const vector<vector<char>>& maze) { //muestra el laberinto en pantalla 
    for (const auto& row : maze) {
        for (const auto& cell : row) {
            cout << cell << ' ';
        }
        cout << '\n';
    }
}

void printMazeWithPause(const vector<vector<char>>& maze) { //muestra el laberinto y la pausa
    printMaze(maze);
    this_thread::sleep_for(chrono::seconds(1)); // Pausa de 1 segundo entre turno y turno
    cout << "\n";
}
