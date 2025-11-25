#include <vector>
#include "types.h"
#include <random>
#include <string>

using namespace std;

int random_int(int low, int high) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(low, high);
    return dist(gen);
}

string getBotMove(vector<vector<int>> &board, vector<Move> &moves){
    string s = moves[random_int(0, moves.size()-1)].move2String();
    return s;
}