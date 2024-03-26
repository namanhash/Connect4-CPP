#include <iostream>
#include <vector>
#include <limits>
#include <cstdlib> 
#include <ctime>   

const int EASY_DEPTH = 3;
const int MEDIUM_DEPTH = 5;
const int HARD_DEPTH = 7;


const int ROWS = 6;
const int COLS = 7;
const int INF = std::numeric_limits<int>::max();
const char AI_PIECE = 'X';
const char PLAYER_PIECE = 'O';
const char EMPTY_PIECE = '.';

std::vector<std::vector<char> > board(ROWS, std::vector<char>(COLS, EMPTY_PIECE));


void printBoard() {
    for (int r = 0; r < ROWS; ++r) {
        for (int c = 0; c < COLS; ++c) {
            std::cout << board[r][c] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "---------------------" << std::endl;
}


bool isValidLocation(int col) {
    return board[0][col] == EMPTY_PIECE;
}


int getNextOpenRow(int col) {
    for (int r = ROWS - 1; r >= 0; r--) {
        if (board[r][col] == EMPTY_PIECE) {
            return r;
        }
    }
    return -1;
}


void placePiece(int col, char piece) {
    int row = getNextOpenRow(col);
    if(row != -1) {
        board[row][col] = piece;
    }
}


bool checkWin(char piece) {
    // Horizontal 
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            if (board[r][c] == piece && board[r][c + 1] == piece && board[r][c + 2] == piece && board[r][c + 3] == piece) {
                return true;
            }
        }
    }
    // Vertical 
    for (int c = 0; c < COLS; c++) {
        for (int r = 0; r < ROWS - 3; r++) {
            if (board[r][c] == piece && board[r + 1][c] == piece && board[r + 2][c] == piece && board[r + 3][c] == piece) {
                return true;
            }
        }
    }
    // Positive diagonal 
    for (int r = 0; r < ROWS - 3; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            if (board[r][c] == piece && board[r + 1][c + 1] == piece && board[r + 2][c + 2] == piece && board[r + 3][c + 3] == piece) {
                return true;
            }
        }
    }
    // Negative diagonal 
    for (int r = 3; r < ROWS; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            if (board[r][c] == piece && board[r - 1][c + 1] == piece && board[r - 2][c + 2] == piece && board[r - 3][c + 3] == piece) {
                return true;
            }
        }
    }
    return false;
}

// evaluation of board state
int evaluateWindow(std::vector<char> window, char aiPiece, char playerPiece) {
    int score = 0;
    int aiCount = std::count(window.begin(), window.end(), aiPiece);
    int playerCount = std::count(window.begin(), window.end(), playerPiece);

    if(aiCount == 4) {
        score += 100;
    } else if(aiCount == 3 && playerCount == 0) {
        score += 5;
    } else if(aiCount == 2 && playerCount == 0) {
        score += 2;
    }

    if(playerCount == 3 && aiCount == 0) {
        score -= 4;
    }

    return score;
}


int scorePosition(char aiPiece, char playerPiece) {
    int score = 0;

    // Score center column
    std::vector<char> centerColumn;
    for(int r = 0; r < ROWS; ++r) centerColumn.push_back(board[r][COLS / 2]);
    score += evaluateWindow(centerColumn, aiPiece, playerPiece) * 3;

    // Score Horizontal
    for (int r = 0; r < ROWS; r++) {
        std::vector<char> rowArray = board[r];
        for (int c = 0; c < COLS - 3; c++) {
            std::vector<char> window(rowArray.begin() + c, rowArray.begin() + c + 4);
            score += evaluateWindow(window, aiPiece, playerPiece);
        }
    }

    // Score Vertical
    for (int c = 0; c < COLS; c++) {
        std::vector<char> colArray;
        for (int r = 0; r < ROWS; r++) colArray.push_back(board[r][c]);
        for (int r = 0; r < ROWS - 3; r++) {
            std::vector<char> window(colArray.begin() + r, colArray.begin() + r + 4);
            score += evaluateWindow(window, aiPiece, playerPiece);
        }
    }

    // Score Positive Diagonal
    for (int r = 0; r < ROWS - 3; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            std::vector<char> window = {board[r][c], board[r + 1][c + 1], board[r + 2][c + 2], board[r + 3][c + 3]};
            score += evaluateWindow(window, aiPiece, playerPiece);
        }
    }

    // Score Negative Diagonal
    for (int r = 0; r < ROWS - 3; r++) {
        for (int c = 0; c < COLS - 3; c++) {
            std::vector<char> window = {board[r + 3][c], board[r + 2][c + 1], board[r + 1][c + 2], board[r][c + 3]};
            score += evaluateWindow(window, aiPiece, playerPiece);
        }
    }

    return score;
}

int minimax(int depth, bool maximizingPlayer, char aiPiece, char playerPiece) {
    if (depth == 0 || checkWin(aiPiece) || checkWin(playerPiece)) {
        if (checkWin(aiPiece)) {
            return INF;
        } else if (checkWin(playerPiece)) {
            return -INF;
        } else {
            return scorePosition(aiPiece, playerPiece);
        }
    }

    if (maximizingPlayer) {
        int value = -INF;
        for (int col = 0; col < COLS; col++) {
            if (isValidLocation(col)) {
                int row = getNextOpenRow(col);
                board[row][col] = aiPiece;
                value = std::max(value, minimax(depth - 1, false, aiPiece, playerPiece));
                board[row][col] = EMPTY_PIECE;
            }
        }
        return value;
    } else {
        int value = INF;
        for (int col = 0; col < COLS; col++) {
            if (isValidLocation(col)) {
                int row = getNextOpenRow(col);
                board[row][col] = playerPiece;
                value = std::min(value, minimax(depth - 1, true, aiPiece, playerPiece));
                board[row][col] = EMPTY_PIECE;
            }
        }
        return value;
    }
}

// Find the best move
int pickBestMove(char aiPiece, char playerPiece, int depth) {
    int bestScore = -INF;
    int bestCol = rand() % COLS;
    for (int col = 0; col < COLS; col++) {
        if (isValidLocation(col)) {
            int row = getNextOpenRow(col);
            board[row][col] = aiPiece;
            int score = minimax(depth, false, aiPiece, playerPiece); 
            board[row][col] = EMPTY_PIECE;
            if (score > bestScore) {
                bestScore = score;
                bestCol = col;
            }
        }
    }
    return bestCol;
}

int main() {
    srand(static_cast<unsigned>(time(0))); 
    int difficultyDepth = MEDIUM_DEPTH; 
    std::cout << "Select difficulty level:\n";
    std::cout << "1. Easy\n";
    std::cout << "2. Medium\n";
    std::cout << "3. Hard\n";
    std::cout << "Enter choice (1-3): ";
    int choice;
    std::cin >> choice;

    switch (choice) {
        case 1:
            difficultyDepth = EASY_DEPTH;
            break;
        case 2:
            difficultyDepth = MEDIUM_DEPTH;
            break;
        case 3:
            difficultyDepth = HARD_DEPTH;
            break;
        default:
            std::cout << "Invalid choice, defaulting to Medium difficulty.\n";
    }

    char currentPlayer = PLAYER_PIECE; // Start with player
    bool gameOver = false;

    while (!gameOver) {
        printBoard();
        if (currentPlayer == AI_PIECE) {
            int col = pickBestMove(AI_PIECE, PLAYER_PIECE, difficultyDepth);
            if (isValidLocation(col)) {
                placePiece(col, AI_PIECE);
                std::cout << "AI placed on column " << col + 1 << std::endl;
                if (checkWin(AI_PIECE)) {
                    std::cout << "AI wins!" << std::endl;
                    gameOver = true;
                }
            }
            currentPlayer = PLAYER_PIECE;
        } else {
            std::cout << "Player's turn (Enter a column number 1-7): ";
            int playerCol;
            std::cin >> playerCol;
            playerCol--; 
            if (isValidLocation(playerCol)) {
                placePiece(playerCol, PLAYER_PIECE);
                if (checkWin(PLAYER_PIECE)) {
                    std::cout << "Player wins!" << std::endl;
                    gameOver = true;
                }
            } else {
                std::cout << "Column is full or invalid, try again." << std::endl;
                continue; 
            }
            currentPlayer = AI_PIECE;
        }

        
    }

    printBoard(); 

    return 0;
}