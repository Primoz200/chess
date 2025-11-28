#include <vector>
#include "types.h"
#include <random>
#include <algorithm>
#include <string>
#include <map>

#define IN_GAME 1
#define WHITE_WIN 2
#define BLACK_WIN 3
#define STALEMATE 4
#define DRAW 5

using namespace std;

map<int, int> pieceWorth = {
    {1, 1},
    {2, 5},
    {3, 3},
    {4, 3},
    {5, 9},
    {6, 5}
};

void shuffleVector(vector<Move>& v){
    std::random_device rd;      // non-deterministic seed
    std::mt19937 g(rd());       // Mersenne Twister engine

    std::shuffle(v.begin(), v.end(), g);
}

int colorMultiplier(int a){
    if(a >= 7) return -1;
    if(a > 0 && a <=6) return 1;

    return 0;
}

int boolColorMultiplier(bool a){
    return a ? 1 : -1;
}

void forceMoveH(vector<vector<int>> &board, Move move, int oldPiece=0){                 //ONLY use on tempboards
    board[move.toY][move.toX] = board[move.fromY][move.fromX];
    board[move.fromY][move.fromX] = oldPiece;
}

int evaluate(vector<vector<int>> &board, bool isWhite, int gamestate){
    int eval = 0;

    if(gamestate == DRAW ||gamestate == STALEMATE) return 0;
    if(gamestate == WHITE_WIN) return 1000;
    if(gamestate == BLACK_WIN) return -1000;

    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            int piece = board[y][x];
            if(piece == 0) continue;
            eval += colorMultiplier(piece) * pieceWorth[piece % 6];
        }
    }

    return eval;
}

string getBotMove(vector<vector<int>> &board, vector<Move> &moves, bool isWhite, int gamestate){
    shuffleVector(moves);
    vector<vector<int>> tempBoard(8, vector<int>(8, 0));
    tempBoard = board;
    string s = "";

    int maxEval = isWhite ? -1001 : 1001;
    int indMove = -1;
    for(int i = 0; i < moves.size(); i++){
        if(moves[i].isNull()) continue;

        int oldPiece =  tempBoard[moves[i].toY][moves[i].toX];
        forceMoveH(tempBoard, moves[i]);      //since moves in vector moves are all legal

        int newEval = evaluate(tempBoard, isWhite, gamestate);
        if(newEval*boolColorMultiplier(isWhite) > maxEval*boolColorMultiplier(isWhite)){
            maxEval = newEval;
            indMove = i;
        }

        forceMoveH(tempBoard, moves[i].reverseMove(), oldPiece);
    }

    if(indMove != -1) return moves[indMove].move2String();
    else {
        for(auto m : moves){
            if(!m.isNull()) return m.move2String();
        }
    }
    return s;
}