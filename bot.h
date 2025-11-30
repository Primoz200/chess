#include <vector>
#include <string>
#include "types.h"

#pragma once

using namespace std;
string getBotMove(vector<vector<int>> &board, vector<Move> &moves, bool isWhite, CastlingRights& castlingRights, int gamestate);
