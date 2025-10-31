#include <array>
#include <vector>
#include <stdint.h>
#include <utility>
using namespace std;

pair<uint64_t, uint64_t>& attackBitBoards(vector<vector<int>> &board);
int getBitNr(int x, int y);
void setBit(uint64_t &piece, int bit);