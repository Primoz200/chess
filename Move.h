#include <string>

using namespace std;
#ifndef MOVE_H
#define MOVE_H

class Move {
    public:
        int fromX;
        int fromY;
        int toX;
        int toY;
        Move* prev;

        Move(int x1, int y1, int x2, int y2, Move* pr) {
            fromX = x1;
            fromY = y1;
            toX = x2;
            toY = y2;
            prev = pr;
        }

        // string toString() {        //for debugging
        //     string result;

        //     result += "--" + to_string(fromX) + " " +
        //             to_string(fromY) + " " +
        //             to_string(toX)   + " " +
        //             to_string(toY)   + "\n";

        //     if (prev != nullptr) {
        //         result += to_string(prev->fromX) + " " +
        //                 to_string(prev->fromY) + " " +
        //                 to_string(prev->toX)   + " " +
        //                 to_string(prev->toY)   + "\n";
        //     }

        //     return result;
        // }

        void updatePrevBoard(){
            prev->prev = NULL;
            prev->fromX = fromX;
            prev->fromY = fromY;
            prev->toX = toX;
            prev->toY = toY;
        }

        void string2move(string moveS){
            if(moveS.length() != 4) {
                fromX = 0;
                fromY = 0;
                toX = 0;
                toY = 0;
            }
            fromX = moveS[0] - 'a';
            fromY = 7 - (moveS[1] - '1');
            toX = moveS[2] - 'a';
            toY = 7 - (moveS[3] - '1');
        }

        string move2String(){
            string res; 

            res += 
                to_string('a' + fromX) +=
                to_string('7' - fromY) +=
                to_string('a' + toX) +=
                to_string('7' - toY);

            return res;
        }

        bool equals(Move &move){
            return fromX == move.fromX && fromY == move.fromY &&
                   toX == move.toX && toY == move.toY;
        }

        Move reverseMove(){
            return {toX, toY, fromX, fromY, NULL};
        }

};

#endif