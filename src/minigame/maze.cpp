#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

const int MIN_SIZE = 5;
const int MAX_SIZE = 30;

// Cell structure for maze generation
struct Cell {
    int x, y;
    bool visited = false;
    bool walls[4] = {true, true, true, true}; // Top, Right, Bottom, Left walls
};

// Directions for maze walls
enum Direction { TOP = 0, RIGHT, BOTTOM, LEFT };

// Maze grid
std::vector<std::vector<Cell>> maze;
int mazeSize;
Cell* playerPosition;
Cell* endPosition;

// Initialize maze grid with cells
void initializeMaze() {
    maze.clear();
    maze.resize(mazeSize, std::vector<Cell>(mazeSize));
    for (int i = 0; i < mazeSize; ++i) {
        for (int j = 0; j < mazeSize; ++j) {
            maze[i][j].x = j;
            maze[i][j].y = i;
        }
    }
}

// Utility function to remove walls between two cells
void removeWalls(Cell* current, Cell* next) {
    int dx = current->x - next->x;
    int dy = current->y - next->y;
    if (dx == 1) {
        current->walls[LEFT] = false;
        next->walls[RIGHT] = false;
    } else if (dx == -1) {
        current->walls[RIGHT] = false;
        next->walls[LEFT] = false;
    }
    if (dy == 1) {
        current->walls[TOP] = false;
        next->walls[BOTTOM] = false;
    } else if (dy == -1) {
        current->walls[BOTTOM] = false;
        next->walls[TOP] = false;
    }
}

// Generate maze using recursive backtracking (DFS)
void generateMaze(Cell* current) {
    current->visited = true;
    std::vector<Cell*> neighbors;

    if (current->y > 0 && !maze[current->y - 1][current->x].visited)
        neighbors.push_back(&maze[current->y - 1][current->x]);
    if (current->y < mazeSize - 1 && !maze[current->y + 1][current->x].visited)
        neighbors.push_back(&maze[current->y + 1][current->x]);
    if (current->x > 0 && !maze[current->y][current->x - 1].visited)
        neighbors.push_back(&maze[current->y][current->x - 1]);
    if (current->x < mazeSize - 1 && !maze[current->y][current->x + 1].visited)
        neighbors.push_back(&maze[current->y][current->x + 1]);

    while (!neighbors.empty()) {
        int index = std::rand() % neighbors.size();
        Cell* next = neighbors[index];
        if (!next->visited) {
            removeWalls(current, next);
            generateMaze(next);
        }
        neighbors.erase(neighbors.begin() + index);
    }
}

// Reset player position to start
void resetGame() {
    playerPosition = &maze[0][0];
    endPosition = &maze[mazeSize - 1][mazeSize - 1];
}

// Render function to draw maze and player
void render(sf::RenderWindow &window) {
    window.clear(sf::Color::White);

    float cellSize = window.getSize().x / mazeSize;

    // Draw maze walls
    sf::RectangleShape wall(sf::Vector2f(cellSize, 2)); // Horizontal wall thickness

    for (int i = 0; i < mazeSize; ++i) {
        for (int j = 0; j < mazeSize; ++j) {
            Cell& cell = maze[i][j];
            float x = j * cellSize;
            float y = i * cellSize;

            // Draw each wall
            if (cell.walls[TOP]) { // Top wall
                wall.setSize(sf::Vector2f(cellSize, 2));
                wall.setPosition(x, y);
                wall.setFillColor(sf::Color::Black);
                window.draw(wall);
            }
            if (cell.walls[RIGHT]) { // Right wall
                wall.setSize(sf::Vector2f(2, cellSize));
                wall.setPosition(x + cellSize, y);
                window.draw(wall);
            }
            if (cell.walls[BOTTOM]) { // Bottom wall
                wall.setSize(sf::Vector2f(cellSize, 2));
                wall.setPosition(x, y + cellSize);
                window.draw(wall);
            }
            if (cell.walls[LEFT]) { // Left wall
                wall.setSize(sf::Vector2f(2, cellSize));
                wall.setPosition(x, y);
                window.draw(wall);
            }
        }
    }

    // Draw player and end point
    sf::CircleShape player(cellSize / 4);
    player.setFillColor(sf::Color::Green);
    player.setPosition(playerPosition->x * cellSize + cellSize / 4, playerPosition->y * cellSize + cellSize / 4);
    window.draw(player);

    sf::CircleShape end(cellSize / 4);
    end.setFillColor(sf::Color::Red);
    end.setPosition(endPosition->x * cellSize + cellSize / 4, endPosition->y * cellSize + cellSize / 4);
    window.draw(end);

    window.display();
}

int main(int argc, char* argv[]) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    // Check if difficulty level argument is provided
    int difficultyLevel = 50; // Default to medium difficulty
    if (argc > 1) {
        difficultyLevel = std::atoi(argv[1]);
    }
    if (difficultyLevel < 0) difficultyLevel = 0;
    if (difficultyLevel > 100) difficultyLevel = 100;

    // Scale maze size based on difficulty level
    mazeSize = MIN_SIZE + (MAX_SIZE - MIN_SIZE) * difficultyLevel / 100;

    // Initialize and generate maze
    initializeMaze();
    generateMaze(&maze[0][0]);
    resetGame();

    // SFML window setup
    sf::RenderWindow window(sf::VideoMode(600, 600), "Maze Game");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            // Player movement (WASD keys)
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::W && !playerPosition->walls[TOP])
                    playerPosition = &maze[playerPosition->y - 1][playerPosition->x];
                else if (event.key.code == sf::Keyboard::S && !playerPosition->walls[BOTTOM])
                    playerPosition = &maze[playerPosition->y + 1][playerPosition->x];
                else if (event.key.code == sf::Keyboard::A && !playerPosition->walls[LEFT])
                    playerPosition = &maze[playerPosition->y][playerPosition->x - 1];
                else if (event.key.code == sf::Keyboard::D && !playerPosition->walls[RIGHT])
                    playerPosition = &maze[playerPosition->y][playerPosition->x + 1];

                // Check if player reached end position
                if (playerPosition == endPosition) {
                    std::cout << "You've reached the end!" << std::endl;
                    return -123;
                }
            }
        }
        render(window);
    }
    return 0;
}
