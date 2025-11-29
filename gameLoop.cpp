#include <iostream>
#include <vector>
#include <map>
#include "chess.h"
#include "types.h"
#include "bitBoards.h"

using namespace std;

#define USER_V_USER 1
#define USER_V_BOT 2
#define BOT_V_BOT 3
#define IN_GAME 1
#define WHITE_WIN 2
#define BLACK_WIN 3
#define STALEMATE 4
#define DRAW 5

map<int, string> figure = {
    {0, " "},
    {1, "♟"},
    {2, "♜"},
    {3, "♞"},
    {4, "♝"},
    {5, "♛"},
    {6, "♚"},
    {7, "♙"},
    {8, "♖"},
    {9, "♘"},
    {10, "♗"},
    {11, "♕"},
    {12, "♔"}
};

void printChars(char c, int n){
    cout << "--";
    for(int i = 0; i < n; i++){
        if(i % 4 == 0) cout << "|";
        else cout << "-";
    }
}

void printBoard(vector<vector<int>> &v) {
    cout << "  ";
    for (int k = 0; k < 8; k++) {
        cout << "| " << string(1, 'A' + k) << " ";
    }
    cout << "|\n";
    printChars('-', 33);
    cout << "\n";
    for (int i = 0; i < v.size(); i++) {
        cout << (8-i) << " | ";
        for (int j = 0; j < v[i].size(); j++) {
            cout << figure[v[i][j]] << " | ";
            // if (v[i][j] < 10) { cout << " "; }
        }
        cout << "\n";
        printChars('-', 33);
        cout << "\n";
    }
    cout << "  ";
    for (int k = 0; k < 8; k++) {
        cout << "| " << string(1, 'A' + k) << " ";
    }
    cout << "|\n";
}

void printBitboard(uint64_t a) {
    for(int i = 0; i < 64; i++){
        
        if( i % 8 ==  0) cout << "\n";
        cout << ((a>>i) & 1L) << " ";
    }
}

int gameLoop(vector<vector<int>> &board, bool isWhite, CastlingRights &castlingRights) {
    int gameState = IN_GAME;
    string moveString;
    vector<Move> moves;
    Move nullMove = {0, 0, 0, 0, NULL};
    Move curMove = {0, 0, 0, 0, &nullMove};
    int moveNr = 1;

    int typeOfGame;
    cout << "Your desired type of game. 1-user_v_user 2-user_v_bot 3-bot_v_bott: ";
    cin >> typeOfGame; 

    while(gameState == IN_GAME){
        cout << "\033[2J\033[1;1H";
        printBoard(board);
        //cout << (moveNr/2+1) << "  ";
        generateMoves(board, isWhite, moves, curMove, castlingRights);

        gameState = getGameState(board, moves, isWhite);
        if(gameState != IN_GAME) return gameState;
        
        
        if(isWhite){ cout << "White to move:\n";}
        else { cout << "Black to move:\n";}

        curMove.updatePrevMove();
        getMove(moveString, board, moves, typeOfGame, isWhite, castlingRights, gameState);
        curMove.string2move(moveString);
        while(!checkIfMoveInVector(curMove, moves)){
            if(moveString == "q") {gameState = STALEMATE; break;}

            cout << "invalid Move\n";
            if(isWhite){ cout << "White to move:\n";}
            else { cout << "Black to move:\n";}

            getMove(moveString, board, moves, typeOfGame, isWhite, castlingRights, gameState);
            curMove.string2move(moveString);
        }

        executeCurMove(board, curMove, isWhite, castlingRights);
        isWhite = !isWhite;
        moveNr++;
    }

    return gameState;
}

void postGameOutput(int endState){
    if(endState == STALEMATE){
        cout << "STALEMATE\n";
    }else if(endState == WHITE_WIN){
        cout << "WHITE WINS\n";
    }else if(endState == BLACK_WIN){
        cout << "BLACK WINS\n";
    }else if(endState == DRAW){
        cout << "DRAW BY OTHER CAUSES\n";
    }
}

int main() {
    vector<vector<int>> board(8, vector<int>(8, 0));
    setUpBoard(board);

    CastlingRights castlingRights;
    resetCastlingRights(&castlingRights);

    postGameOutput(gameLoop(board, true, castlingRights));
}

