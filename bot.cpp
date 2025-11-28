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
    {1, 10},
    {2, 50},
    {3, 30},
    {4, 30},
    {5, 90},
    {6, 40}
};

vector<vector<int>> pawnValue = {
    {15, 15, 15, 15, 15, 15, 15, 15},
    {14, 14, 15, 15, 15, 15, 14, 14},
    {12, 11, 11, 12, 12, 11, 11, 12},
    {10, 10, 11, 13, 13, 11, 10, 10},
    {10, 10, 11, 13, 13, 11, 10, 10},
    {9, 10, 10, 12, 12, 10, 10, 9},
    {9, 10, 10, 10, 10, 10, 10, 9},
    {10, 10, 10, 10, 10, 10, 10, 10}
};

vector<vector<int>> knightValue = {
    {24, 26, 28, 28, 28, 28, 26, 24},
    {26, 28, 30, 30, 30, 30, 28, 26},
    {26, 30, 32, 32, 32, 32, 30, 26},
    {28, 30, 32, 33, 33, 32, 30, 28},
    {28, 30, 32, 33, 33, 32, 30, 28},
    {26, 30, 32, 32, 32, 32, 30, 26},
    {26, 28, 30, 30, 30, 30, 28, 26},
    {24, 26, 28, 28, 28, 28, 26, 24}
};

vector<vector<int>> kingValueMiddleGame = {
    {30, 30, 30, 30, 30, 30, 30, 30},
    {30, 30, 30, 30, 30, 30, 30, 30},
    {30, 30, 30, 30, 30, 30, 30, 30},
    {30, 30, 30, 30, 30, 30, 30, 30},
    {35, 35, 35, 35, 35, 35, 35, 35},
    {38, 38, 38, 38, 38, 38, 38, 38},
    {43, 43, 41, 41, 41, 41, 43, 43},
    {48, 48, 48, 44, 44, 45, 48, 48}
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
    int pieceCount=0;
    pair<int,int> whiteKingPosition = {-1, -1};
    pair<int,int> blackKingPosition = {-1, -1};

    if(gamestate == DRAW || gamestate == STALEMATE) return 0;
    if(gamestate == WHITE_WIN) return 10000;
    if(gamestate == BLACK_WIN) return -10000;

    for(int x = 0; x < 8; x++){
        for(int y = 0; y < 8; y++){
            int piece = board[y][x];
            if(piece == 0) continue;
            pieceCount++;
            if(piece % 6 == 0) {
                if(piece == 6) {
                    whiteKingPosition.first = x;
                    whiteKingPosition.second = y;
                }else{
                    blackKingPosition.first = x;
                    blackKingPosition.second = y;
                }
            }
            else if(piece%6 == 1){
                int pawnV = isWhite ? pawnValue[y][x] : pawnValue[7-y][x];
                eval += colorMultiplier(piece) * pawnV;
            }
            else if(piece % 6 == 3){
                eval += colorMultiplier(piece) * knightValue[x][y];
            }
            else{ eval += colorMultiplier(piece) * pieceWorth[piece % 6];}
    
        }
    }

    if(pieceCount > 10){        //arbitratry selected piece number for when it transitions into an endgame
        eval+=kingValueMiddleGame[whiteKingPosition.second][whiteKingPosition.first];
        eval+= -kingValueMiddleGame[7 - blackKingPosition.second][blackKingPosition.first];
    }//else endgame table

    return eval;
}

string getBotMove(vector<vector<int>> &board, vector<Move> &moves, bool isWhite, int gamestate){
    shuffleVector(moves);
    vector<vector<int>> tempBoard(8, vector<int>(8, 0));
    tempBoard = board;
    string s = "";

    int maxEval = isWhite ? -10001 : 10001;
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