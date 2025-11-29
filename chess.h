#ifndef GAME_H
#define GAME_H

#include <vector>
#include "types.h"
#include "bitBoards.h"


void setUpBoard(vector<vector<int>>  &v, string fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR");
void resetCastlingRights(CastlingRights* rights, pair<pair<bool, bool>, pair<bool, bool>> options={{true, true},  {true, true}});
bool executeCurMove(vector<vector<int>> &board, Move move, bool isWhite, CastlingRights &castlingRights);
int checkIfMoveInVector(Move &move, vector<Move> &moves);
void getMove(string &strMove, vector<vector<int>> &board, vector<Move> &moves, int typeOfGame, bool isWhite, CastlingRights& castlingRights, int gamestate);
void generateMoves(vector<vector<int>> &board, bool isWhite, vector<Move> &moves, Move &lastMove, CastlingRights &castlingRights);
int getGameState(vector<vector<int>> &board, vector<Move> &moves, bool isWhite);
void forceMove(vector<vector<int>> &board, Move move, int oldPiece=0);

#endif