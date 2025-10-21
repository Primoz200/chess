#include <array>
#include <vector>
#include <stdint.h>


std::array<uint64_t, 13> pieces = {};

inline int getBitNr(int x, int y){
    return y*8+x;
}

void setBit(uint64_t &piece, int bit){
    piece |= (1L << bit);
}

void resetBit(uint64_t &piece, int bit){
    piece & ~(1L << bit);
}

std::array<uint64_t, 13>& attackBitBoards(std::vector<std::vector<int>> &board){        //a8=0bit h1=63bit
    for(int8_t i = 0; i<8; i++){
        for(int8_t j = 0; j<8; j++){
            int figura = board[i][j];
            if(figura == 0) continue;

            switch (figura % 6) {
                case 0:
                    pieces[figura] = 0;
                    for(int8_t k1 = -1; k1 < 2; k1++){
                        for(int8_t k2 = -1; k2 < 2; k2++){
                            if(k2 == 0 && k1 == 0) continue;
                            int bit = getBitNr(j+k1, i+k2);
                            if(bit >= 0 && bit <=63) {
                                setBit(pieces[figura], bit);
                            }
                        }
                    }
                    break;
                case 1:
                    break;
                case 2:
                    break;
                case 3:
                    break;
                case 4:
                    break;
                case 5:
                    break;
            }

        }
    }

    return pieces;
}

