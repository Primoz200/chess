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
        int flag;
        int capture;

        Move(int x1, int y1, int x2, int y2, Move* pr, int flags = 0, int captures = 0) {
            fromX = x1;
            fromY = y1;
            toX = x2;
            toY = y2;
            prev = pr;
            flag = flags;
            capture = captures;
        }

        string toString() {        //for debugging
            string result;

            result += "--" + to_string(fromX) + " " +
                    to_string(fromY) + " " +
                    to_string(toX)   + " " +
                    to_string(toY)   + " " + "flag:" +
                    to_string(flag)  + "\n";

            if (prev != nullptr) {
                result += to_string(prev->fromX) + " " +
                        to_string(prev->fromY) + " " +
                        to_string(prev->toX)   + " " +
                        to_string(prev->toY)   + "\n";
            }

            return result;
        }

        void updatePrevMove(){
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
                return;
            }
            fromX = moveS[0] - 'a';
            fromY = 7 - (moveS[1] - '1');
            toX = moveS[2] - 'a';
            toY = 7 - (moveS[3] - '1');
        }

        string move2String(){
            string res; 

            res.push_back('a' + fromX);
            res.push_back('8' - fromY);
            res.push_back('a' + toX);
            res.push_back('8' - toY);

            return res;
        }
        
        bool equals(Move &move){
            return fromX == move.fromX && fromY == move.fromY &&
                   toX == move.toX && toY == move.toY;
        }

        bool isNull(){
            return fromX == 0 && fromY == 0 && toX == 0 && toY == 0;
        }

        Move reverseMove(){
            return {toX, toY, fromX, fromY, NULL, flag};
        }

        int score(){
            if(this->isNull()) return 0;
            else return 1;
        }
};

#endif
#ifndef CastlingRights_H
#define CastlingRights_H

struct CastlingRights{
    bool whiteRight;
    bool whiteLeft;
    bool blackRight;
    bool blackLeft;
};

#endif