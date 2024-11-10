#include <SFML/Graphics.hpp>
#include <vector>
#include <limits>
#include <iostream>

const int ROWS = 6;
const int COLS = 7;
const int WIN_CONDITION = 4;

enum class Player { NONE, HUMAN, AI };

std::vector<std::vector<Player>> board(ROWS, std::vector<Player>(COLS, Player::NONE));

// Function to check if a move is valid
bool isValidMove(int col) {
    return board[0][col] == Player::NONE;
}

// Drop a piece in the selected column
void dropPiece(int col, Player player) {
    for (int row = ROWS - 1; row >= 0; --row) {
        if (board[row][col] == Player::NONE) {
            board[row][col] = player;
            break;
        }
    }
}

// Undo the last piece
void undoPiece(int col) {
    for (int row = 0; row < ROWS; ++row) {
        if (board[row][col] != Player::NONE) {
            board[row][col] = Player::NONE;
            break;
        }
    }
}

// Check for a win in any direction
bool checkWin(Player player) {
    // Check rows
    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col <= COLS - WIN_CONDITION; ++col) {
            bool win = true;
            for (int k = 0; k < WIN_CONDITION; ++k) {
                if (board[row][col + k] != player) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }
    }

    // Check columns
    for (int col = 0; col < COLS; ++col) {
        for (int row = 0; row <= ROWS - WIN_CONDITION; ++row) {
            bool win = true;
            for (int k = 0; k < WIN_CONDITION; ++k) {
                if (board[row + k][col] != player) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }
    }

    // Check diagonals
    for (int row = 0; row <= ROWS - WIN_CONDITION; ++row) {
        for (int col = 0; col <= COLS - WIN_CONDITION; ++col) {
            bool win = true;
            for (int k = 0; k < WIN_CONDITION; ++k) {
                if (board[row + k][col + k] != player) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }
    }

    for (int row = 0; row <= ROWS - WIN_CONDITION; ++row) {
        for (int col = WIN_CONDITION - 1; col < COLS; ++col) {
            bool win = true;
            for (int k = 0; k < WIN_CONDITION; ++k) {
                if (board[row + k][col - k] != player) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }
    }

    return false;
}

// Evaluate board score
int evaluate(Player player) {
    if (checkWin(Player::AI)) return 1000;
    if (checkWin(Player::HUMAN)) return -1000;
    return 0;
}

// Minimax with Alpha-Beta Pruning
int minimax(int depth, int alpha, int beta, bool maximizingPlayer) {
    int score = evaluate(maximizingPlayer ? Player::AI : Player::HUMAN);
    if (depth == 0 || score == 1000 || score == -1000) return score;

    if (maximizingPlayer) {
        int maxEval = std::numeric_limits<int>::min();
        for (int col = 0; col < COLS; ++col) {
            if (isValidMove(col)) {
                dropPiece(col, Player::AI);
                int eval = minimax(depth - 1, alpha, beta, false);
                undoPiece(col);
                maxEval = std::max(maxEval, eval);
                alpha = std::max(alpha, eval);
                if (beta <= alpha) break;
            }
        }
        return maxEval;
    } else {
        int minEval = std::numeric_limits<int>::max();
        for (int col = 0; col < COLS; ++col) {
            if (isValidMove(col)) {
                dropPiece(col, Player::HUMAN);
                int eval = minimax(depth - 1, alpha, beta, true);
                undoPiece(col);
                minEval = std::min(minEval, eval);
                beta = std::min(beta, eval);
                if (beta <= alpha) break;
            }
        }
        return minEval;
    }
}

// Set the AI search depth based on difficulty input (0-100)
int getDepthFromDifficulty(int difficultyInput) {
    if (difficultyInput <= 33) return 2;
    else if (difficultyInput <= 66) return 4;
    else return 6;
}

// Find the best move for AI based on difficulty level
int findBestMove(int difficultyInput) {
    int bestMove = -1;
    int bestScore = std::numeric_limits<int>::min();
    
    // Set depth based on difficulty
    int depth = getDepthFromDifficulty(difficultyInput);

    for (int col = 0; col < COLS; ++col) {
        if (isValidMove(col)) {
            dropPiece(col, Player::AI);
            int moveScore = minimax(depth, std::numeric_limits<int>::min(), std::numeric_limits<int>::max(), false);
            undoPiece(col);

            if (moveScore > bestScore) {
                bestScore = moveScore;
                bestMove = col;
            }
        }
    }

    return bestMove;
}

// Render board using SFML
void render(sf::RenderWindow &window) {
    window.clear(sf::Color::Blue);
    sf::CircleShape piece(46.f);
    piece.setOrigin(50.f, 50.f);

    for (int row = 0; row < ROWS; ++row) {
        for (int col = 0; col < COLS; ++col) {
            piece.setPosition(col * 100.f + 50, row * 100.f + 50);
            if (board[row][col] == Player::HUMAN) piece.setFillColor(sf::Color::Red);
            else if (board[row][col] == Player::AI) piece.setFillColor(sf::Color::Yellow);
            else piece.setFillColor(sf::Color::White);
            window.draw(piece);
        }
    }
    window.display();
}

int main(int argc, char* argv[]) {
    sf::RenderWindow window(sf::VideoMode(COLS * 100, ROWS * 100), "Connect 4");

    int difficultyInput = 50; // Default to medium difficulty
    if (argc > 1) {
        difficultyInput = std::atoi(argv[1]);
    }
    if (difficultyInput < 0) difficultyInput = 0;
    if (difficultyInput > 100) difficultyInput = 100;

    Player currentPlayer = Player::HUMAN;
    bool gameOver = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (!gameOver && currentPlayer == Player::HUMAN && event.type == sf::Event::MouseButtonPressed) {
                int col = event.mouseButton.x / 100;
                if (isValidMove(col)) {
                    dropPiece(col, Player::HUMAN);
                    if (checkWin(Player::HUMAN)) {
                        return -123;
                        render(window);
                    }
                    currentPlayer = Player::AI;
                }
            }
        }

        if (!gameOver && currentPlayer == Player::AI) {
            int bestMove = findBestMove(difficultyInput);
            dropPiece(bestMove, Player::AI);
            if (checkWin(Player::AI)) {
                gameOver = true;
                return 0;
            }
            currentPlayer = Player::HUMAN;
        }

        render(window);
    }

    return 0;
}
