#include <vector>
#include "types.h"
#include <random>
#include <algorithm>
#include <string>
#include <map>
#include "chess.h"
#include <iostream>

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

const vector<vector<int>> pawnValue = {
    {15, 15, 15, 15, 15, 15, 15, 15},
    {14, 14, 15, 15, 15, 15, 14, 14},
    {12, 11, 11, 12, 12, 11, 11, 12},
    {10, 10, 11, 13, 13, 11, 10, 10},
    {10, 10, 11, 13, 13, 11, 10, 10},
    {9, 10, 10, 12, 12, 10, 10, 9},
    {9, 10, 10, 10, 10, 10, 10, 9},
    {10, 10, 10, 10, 10, 10, 10, 10}
};

const vector<vector<int>> knightValue = {
    {24, 26, 28, 28, 28, 28, 26, 24},
    {26, 28, 30, 30, 30, 30, 28, 26},
    {26, 30, 32, 32, 32, 32, 30, 26},
    {28, 30, 32, 33, 33, 32, 30, 28},
    {28, 30, 32, 33, 33, 32, 30, 28},
    {26, 30, 32, 32, 32, 32, 30, 26},
    {26, 28, 30, 30, 30, 30, 28, 26},
    {24, 26, 28, 28, 28, 28, 26, 24}
};

const vector<vector<int>> kingValueMiddleGame = {
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

int evaluate(vector<vector<int>> &board, int gamestate){
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
                int pawnV;
                if(piece == 1) pawnV = pawnValue[y][x];
                else pawnV = pawnValue[7-y][x];
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


int minMax(vector<vector<int>> &board, vector<Move> &moves, bool isWhite, CastlingRights& castlingRights, int gamestate, int depth){
    if(depth >=10 || gamestate != IN_GAME){
        return evaluate(board, gamestate);
    }

    if(isWhite){
        int bestEval = -1;
    }
    for(int i = 0; i < moves.size(); i++){
        if(moves[i].isNull()) continue;
    }
}

string getBotMove(vector<vector<int>> &board, bool isWhite, CastlingRights& castlingRights, int gamestate){
    vector<vector<int>> tempBoard(8, vector<int>(8, 0));
    tempBoard = board;
    string s = "";



    // for(int i = 0; i < moves.size(); i++){
    //     if(moves[i].isNull()) continue;

    //     int oldPiece =  tempBoard[moves[i].toY][moves[i].toX];

    //     executeCurMove(tempBoard, moves[i], isWhite, castlingRights);

    //     int newEval = evaluate(tempBoard, gamestate);
    //     if(newEval*boolColorMultiplier(isWhite) > maxEval*boolColorMultiplier(isWhite)){
    //         maxEval = newEval;
    //         indMove = i;
    //     }

    //     forceMove(tempBoard, moves[i].reverseMove(), oldPiece);
    // // }

    // if(indMove != -1) return moves[indMove].move2String();
    // else {
    //     for(auto m : moves){
    //         if(!m.isNull()) return m.move2String();
    //     }
    // }
    return s;
}