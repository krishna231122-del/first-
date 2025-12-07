#include <iostream>
#include <vector>
#include <limits>

using namespace std;

class TicTacToe {
private:
    char board[3][3];

public:
    TicTacToe() {
        resetBoard();
    }

    void resetBoard() {
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                board[i][j] = ' ';
    }

    void displayBoard() {
        cout << "\n";
        for(int i = 0; i < 3; i++) {
            cout << " ";
            for(int j = 0; j < 3; j++) {
                cout << board[i][j];
                if(j < 2) cout << " | ";
            }
            cout << "\n";
            if(i < 2) cout << "---|---|---\n";
        }
        cout << "\n";
    }

    bool placeMark(int row, int col, char player) {
        if(row < 0 || row > 2 || col < 0 || col > 2 || board[row][col] != ' ')
            return false;
        board[row][col] = player;
        return true;
    }

    bool checkWin(char player) {
        // Rows and columns
        for(int i = 0; i < 3; i++)
            if((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
               (board[0][i] == player && board[1][i] == player && board[2][i] == player))
                return true;
        // Diagonals
        if((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
           (board[0][2] == player && board[1][1] == player && board[2][0] == player))
            return true;
        return false;
    }

    bool checkDraw() {
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                if(board[i][j] == ' ')
                    return false;
        return true;
    }

    vector<pair<int,int>> emptyCells() {
        vector<pair<int,int>> moves;
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 3; j++)
                if(board[i][j] == ' ')
                    moves.push_back({i,j});
        return moves;
    }

    int minimax(bool isMaximizing, char aiPlayer, char humanPlayer) {
        if(checkWin(aiPlayer)) return 10;
        if(checkWin(humanPlayer)) return -10;
        if(checkDraw()) return 0;

        if(isMaximizing) {
            int bestScore = numeric_limits<int>::min();
            for(auto cell : emptyCells()) {
                board[cell.first][cell.second] = aiPlayer;
                int score = minimax(false, aiPlayer, humanPlayer);
                board[cell.first][cell.second] = ' ';
                bestScore = max(score, bestScore);
            }
            return bestScore;
        } else {
            int bestScore = numeric_limits<int>::max();
            for(auto cell : emptyCells()) {
                board[cell.first][cell.second] = humanPlayer;
                int score = minimax(true, aiPlayer, humanPlayer);
                board[cell.first][cell.second] = ' ';
                bestScore = min(score, bestScore);
            }
            return bestScore;
        }
    }

    pair<int,int> bestMove(char aiPlayer, char humanPlayer) {
        int bestScore = numeric_limits<int>::min();
        pair<int,int> move = {-1,-1};
        for(auto cell : emptyCells()) {
            board[cell.first][cell.second] = aiPlayer;
            int score = minimax(false, aiPlayer, humanPlayer);
            board[cell.first][cell.second] = ' ';
            if(score > bestScore) {
                bestScore = score;
                move = cell;
            }
        }
        return move;
    }
};

int main() {
    TicTacToe game;
    int mode;
    cout << "Tic-Tac-Toe Game\n1. 2 Player\n2. Play vs AI\nChoose mode: ";
    cin >> mode;

    char player = 'X';
    char aiPlayer = 'O';
    char humanPlayer = 'X';

    game.displayBoard();

    while(true) {
        int row, col;
        if(mode == 1 || (mode == 2 && player == humanPlayer)) {
            cout << "Player " << player << " enter row and column (0-2): ";
            cin >> row >> col;
        }

        bool validMove = false;

        if(mode == 1 || (mode == 2 && player == humanPlayer)) {
            validMove = game.placeMark(row, col, player);
            if(!validMove) {
                cout << "Invalid move! Try again.\n";
                continue;
            }
        } else if(mode == 2 && player == aiPlayer) {
            auto move = game.bestMove(aiPlayer, humanPlayer);
            game.placeMark(move.first, move.second, aiPlayer);
            cout << "AI places at: " << move.first << " " << move.second << "\n";
        }

        game.displayBoard();

        if(game.checkWin(player)) {
            if(mode == 2 && player == aiPlayer) cout << "AI wins!\n";
            else cout << "Player " << player << " wins!\n";
            break;
        }

        if(game.checkDraw()) {
            cout << "It's a draw!\n";
            break;
        }

        // Switch player
        player = (player == 'X') ? 'O' : 'X';
    }

    return 0;
}
