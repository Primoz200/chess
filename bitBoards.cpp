#include <array>
#include <vector>
#include <utility>
#include <stdint.h>

using namespace std;

array<uint64_t, 13> pieces = {};
pair<uint64_t, uint64_t> allAttacks = {0, 0};

inline int getBitNr(int x, int y){
    return y*8+x;
}

void setBit(uint64_t &piece, int bit){
    piece |= (1L << bit);
}

void resetBit(uint64_t &piece, int bit){
    piece & ~(1L << bit);
}

pair<uint64_t, uint64_t>& attackBitBoards(vector<vector<int>> &board){        //a8=0bit h1=63bit
    allAttacks = {0, 0};
    for(int i = 0; i < 14; i++){
        pieces[i] = 0;
    }
    for(int8_t i = 0; i<8; i++){
        for(int8_t j = 0; j<8; j++){
            int figura = board[i][j];
            if(figura == 0) continue;

            switch (figura % 6) {
                case 0:{
                    for(int8_t k1 = -1; k1 < 2; k1++){
                        for(int8_t k2 = -1; k2 < 2; k2++){
                            if(k2 == 0 && k1 == 0) continue;
                            int bit = getBitNr(j+k1, i+k2);
                            if(bit >= 0 && bit <=63) {
                                setBit(pieces[figura], bit);
                                if(figura <= 6) setBit(allAttacks.first, bit);
                                else setBit(allAttacks.second, bit);
                            }
                        }
                    }
                    break;
                }
                case 1:{
                    int bit1, bit2;
                    if(figura <= 6) {
                        bit1 = getBitNr(j+1, i-1);
                        bit2 = getBitNr(j-1, i-1);
                    }else {
                        bit1 = getBitNr(j+1, i+1);
                        bit2 = getBitNr(j-1, i+1);
                    }
                    
                    if(bit1 >= 0 && bit1 <= 63) {
                        setBit(pieces[figura], bit1);
                        if(figura <= 6) setBit(allAttacks.first, bit1);
                        else setBit(allAttacks.second, bit1);
                    }
                    if(bit2 >= 0 && bit2 <= 63) {
                        setBit(pieces[figura], bit2);
                        if(figura <= 6) setBit(allAttacks.first, bit2);
                        else setBit(allAttacks.second, bit2);
                    }
                    break;
                }
                case 2:{
                    break;
                }
                case 3:{
                    break;
                }
                case 4:{
                    break;
                }
                case 5:{
                    break;
                }
            }

        }
    }

    return allAttacks;
}

