#include <array>
#include <vector>
#include <utility>
#include <stdint.h>

using namespace std;

array<uint64_t, 13> pieces = {};
pair<uint64_t, uint64_t> allAttacks = {0, 0};

inline int getBitNr(int x, int y){
    if(y > 7 || y < 0 || x < 0 || x > 7 ) return 64;
    else return y*8+x;
}

void setBit(uint64_t &piece, int bit){
    piece |= (1ULL << bit);
}

void resetBit(uint64_t &piece, int bit){
    piece &= ~(1ULL << bit);
}

void slide(vector<vector<int>> &board, int i, int j, int dirx, int diry){    
    int figura = board[i][j];
    int x = dirx, y = diry;

    auto &attacks = (figura <= 6) ? allAttacks.first : allAttacks.second;

    while (i + y >= 0 && i + y <= 7 && j + x >= 0 && j + x <= 7) {

        int bit = getBitNr(j+x, i+y);
        if (board[i+y][j+x] == 0) {
            if(bit >= 0 && bit <= 63){
                setBit(pieces[figura], bit);
                setBit(attacks, bit);
            }
        } else {
            if(bit >= 0 && bit <= 63){
                setBit(pieces[figura], bit);
                setBit(attacks, bit);
            }
            break;
        }
        x += dirx;
        y += diry;
    }
}

pair<uint64_t, uint64_t>& attackBitBoards(vector<vector<int>> &board){        //a8=0bit h1=63bit
    allAttacks = {0, 0};
    for(int i = 0; i < pieces.size(); i++){
        pieces[i] = 0;
    }
    for(int8_t i = 0; i<8; i++){                                //checks for inside the board are performed by the getBitNr function
        for(int8_t j = 0; j<8; j++){
            int figura = board[i][j];
            if(figura == 0) continue;
            uint64_t &attacks = figura <= 6 ? allAttacks.first : allAttacks.second;

            switch (figura % 6) {
                case 0:{
                    for(int8_t k1 = -1; k1 < 2; k1++){
                        for(int8_t k2 = -1; k2 < 2; k2++){
                            if(k2 == 0 && k1 == 0) continue;
                            int bit = getBitNr(j+k1, i+k2);
                            if(bit >= 0 && bit <=63) {
                                setBit(pieces[figura], bit);
                                if(figura <= 6) setBit(attacks, bit);
                                else setBit(allAttacks.second, bit);
                            }
                        }
                    }
                    break;
                }
                case 1:{       
                    int dir = figura == 1 ? -1 : 1;
                    int bit = 0;

                    for(int dx : {-1, 1}){
                        bit = getBitNr(j+dx, i+dir);

                        if(bit >= 0 && bit <= 63){
                            setBit(pieces[figura], bit);
                            setBit(attacks, bit);
                        }
                    }
                    break;
                }
                case 2:{
                    slide(board, i, j, 1, 0);       // right
                    slide(board, i, j, -1, 0);      // left
                    slide(board, i, j, 0, 1);       // down
                    slide(board, i, j, 0, -1);      // up
                    break;
                }
                case 3:{
                    int possible[8][2] = {
                        {2, 1}, {2, -1}, {-2, 1}, {-2, -1},
                        {1, 2}, {1, -2}, {-1, 2}, {-1, -2}
                    };
                    for(auto m : possible){
                        int bit = getBitNr(j+m[0], i+m[1]);
                        if(bit >= 0 && bit <= 63){
                            setBit(pieces[figura], bit);
                            setBit(attacks, bit);
                        }
                    }
                    break;
                }
                case 4:{
                    slide(board, i, j, 1, 1);           //down right
                    slide(board, i, j, -1, -1);         //up left
                    slide(board, i, j, 1, -1);          //up right
                    slide(board, i, j, -1, 1);          //down left
                    break;
                }
                case 5:{
                    slide(board, i, j, 1, 0);       // right
                    slide(board, i, j, -1, 0);      // left
                    slide(board, i, j, 0, 1);       // down
                    slide(board, i, j, 0, -1);      // up
                    slide(board, i, j, 1, 1);           //down right
                    slide(board, i, j, -1, -1);         //up left
                    slide(board, i, j, 1, -1);          //up right
                    slide(board, i, j, -1, 1);          //down left
                    break;
                }
            }

        }
    }

    return allAttacks;
}

