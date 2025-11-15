#include <iostream>
#include <vector>
#include <string>
#include <map> 
#include <array>
#include <stdint.h>
#include "bitBoards.h"


using namespace std;

map<int, string> figure = {
    {0, "-"},
    {1, "p"},
    {2, "r"},
    {3, "n"},
    {4, "b"},
    {5, "q"},
    {6, "k"},
    {7, "P"},
    {8, "R"},
    {9, "N"},
    {10, "B"},
    {11, "Q"},
    {12, "K"}};

void setUpBoard(vector<vector<int>>  &v) {
    for(int i = 0; i < 3; i++){
        v[0][i] = 8+i;
        v[0][7-i] = 8+i;

        v[7][i] = 2+i;
        v[7][7-i] = 2+i;
    }
    v[0][3] = 11;
    v[0][4] = 12;
    v[7][3] = 5;
    v[7][4] = 6;

    for(int i = 0; i < 8; i++){
        v[1][i] = 7;
        v[6][i] = 1;
    }


}

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

        void toString(){
            cout << "--" << fromX << " " << fromY  << " " << toX << " " << toY << endl;
            if(prev != NULL){
                cout << prev->fromX << " " << prev->fromY << " " << prev->toX << " " << prev->toY << endl;
            }
        }

        bool equals(Move &move){
            return fromX == move.fromX && fromY == move.fromY &&
                   toX == move.toX && toY == move.toY;
        }

        Move reverseMove(){
            return {toX, toY, fromX, fromY, NULL};
        }

};

void printBoard(vector<vector<int>> &v) {
    cout << "   ";
    for (int k = 0; k < 8; k++) {
        cout << string(1, 'A' + k);
        cout << " "; 
    }
    cout << endl;
    for (int i = 0; i < v.size(); i++) {
        cout << (8-i) << ": ";
        for (int j = 0; j < v[i].size(); j++) {
            cout << figure[v[i][j]] << " ";
            // if (v[i][j] < 10) { cout << " "; }
        }
    cout << endl;
    }
}

void printBitboard(uint64_t a) {
    for(int i = 0; i < 64; i++){
        
        if( i % 8 ==  0) cout << "\n";
        cout << ((a>>i) & 1L) << " ";
    }
}

void string2Move(string moveS, Move* move) {
    move->fromX = moveS[0] - 'a';
    move->fromY = 7 - (moveS[1] - '1');
    move->toX = moveS[2] - 'a';
    move->toY = 7 - (moveS[3] - '1');
}

void updatePrevBoard(Move* move){
    move->prev->prev == NULL;
    move->prev->fromX = move->fromX;
    move->prev->fromY = move->fromY;
    move->prev->toX = move->toX;
    move->prev->toY = move->toY;
}

bool kingInCheck(vector<vector<int>> &board, Move &move, bool color){ //retruns true if king of color is in check
    int targetNr = color ? 6 : 12;
    uint64_t kingPos = 0;

    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++){ 
            if(board[i][j] == targetNr) {setBit(kingPos, getBitNr(j, i)); break;}
        }
    }
    uint64_t attackBoard = color ? attackBitBoards(board).second : attackBitBoards(board).first;

    if(attackBoard & kingPos) return true;

    return false;
}

bool makeMove(vector<vector<int>> &board, Move &move, bool color) {
    board[move.toY][move.toX] = board[move.fromY][move.fromX];
    board[move.fromY][move.fromX] = 0;

   if(kingInCheck(board, move, color)) {
        board[move.fromY][move.fromX] = board[move.toY][move.toX];
        board[move.toY][move.toX] = 0;
        return false;
    }
    return true;
}

int checkIfMoveInVector(Move &move, vector<Move> &moves){       //return value is the index of the move + 1 to make enPassant checks easier while also being valid if index = 0
    for(int i = 0; i < moves.size(); i++){
        if(move.equals(moves[i])) return i+1;
    }
    return 0;
}

bool insideBoard(Move &move) {                              //checks that its inside the board and that it does move
    if(move.fromX < 0 || move.fromX > 7) return false;
    if(move.fromY < 0 || move.fromY > 7) return false;
    if(move.toX < 0 || move.toX > 7) return false;
    if(move.toY < 0 || move.toY > 7) return false;

    if(move.fromX == move.toX && move.fromY == move.toY) return false;
    return true;
}

void generateSlidingMoves(vector<vector<int>> &board, vector<Move> &initial, int j, int i, int dirx, int diry){    //j and i suplement move.from X and move.fromY for less needed info
    int x = dirx, y = diry;
    
    bool colorOfAttackingPiece = board[i][j] < 7 ? true : false; 
    while(i + y >= 0 && i + y <= 7 && j + x >= 0 && j + x <= 7){
        if(board[i+y][j+x] == 0){
            initial.push_back({j, i, j+x, i+y, NULL});
        }else if(colorOfAttackingPiece){
            if(board[i+y][j+x] >= 7){
                initial.push_back({j, i, j+x, i+y, NULL});
            }
            break;
        }else{
            if(board[i+y][j+x] <= 6){
                initial.push_back({j, i, j+x, i+y, NULL});
            }
            break;
        }
        x+=dirx;
        y+=diry;
    }

}

void generateKingMoves(vector<vector<int>> &board, vector<Move>& moves, Move &move){  //dont look, ugly code
    bool color = board[move.fromY][move.fromX] >= 7 ? 0 : 1;

    vector<vector<int>> tempBoard = board;
    for(int x = -1; x <= 1; x++){
        for(int y = -1; y <= 1; y++){
            if(getBitNr(move.fromX+x, move.fromY+y) >= 64 || getBitNr(move.fromX+x, move.fromY+y) < 0) continue;    //if clause is true the square is outside of the board
            if(x == 0 && y == 0) continue;
            if(tempBoard[move.fromY+y][move.fromX+x] == 0){
                tempBoard[move.fromY+y][move.fromX+x] = board[move.fromY][move.fromX];
                tempBoard[move.fromY][move.fromX] = 0;
                if(!kingInCheck){
                    moves.push_back({move.fromX, move.fromY, move.fromX+x, move.fromY+y, NULL});
                }
                tempBoard[move.fromY+y][move.fromX+x] = board[move.fromY+y][move.fromX+x];
                tempBoard[move.fromY][move.fromX] = board[move.fromY][move.fromX];
            }else if(tempBoard[move.fromY+y][move.fromX+x] <= 6){
                if(color) continue;
            }else if(tempBoard[move.fromY+y][move.fromX+x] >= 7){
                if(!color) continue;   
        }
            tempBoard[move.fromY+y][move.fromX+x] = board[move.fromY][move.fromX];
            tempBoard[move.fromY][move.fromX] = 0;
            if(!kingInCheck(board, move, color)){
                moves.push_back({move.fromX, move.fromY, move.fromX+x, move.fromY+y, NULL});
            }
            tempBoard[move.fromY+y][move.fromX+x] = board[move.fromY+y][move.fromX+x];
            tempBoard[move.fromY][move.fromX] = board[move.fromY][move.fromX];    
        }
    }
}

bool kings(vector<vector<int>> &board, Move &move, bool color, pair<bool, bool> &castlingRights){
    if(!insideBoard(move)) return false;
    vector<Move> moves;

    generateKingMoves(board, moves, move);
    if(checkIfMoveInVector(move, moves)){
        return makeMove(board, move, color);
    }
    
    // if(move.fromY == move.toY){
    //     if(move.toX == 6 && board[move.toY][move.toX] == 0 && board[move.toY][move.toX-1] == 0){
    //         if(color && castlingRights.first){
    //             if(board[move.toY][move.toX+1] == 2){       //rook
    //                 if(makeMove(board, move, color)){                  //moves king
    //                 board[move.toY][move.toX-1] = 2;      //moves rook
    //                 board[move.toY][move.toX+1] = 0;
    //                 return true;
    //                 }
    //             }
    //         }else if(!color && castlingRights.second){
    //             if(board[move.toY][move.toX+1] == 8){       //rook
    //                 if(makeMove(board, move, color)){                  //moves king
    //                 board[move.toY][move.toX-1] = 8;      //moves rook
    //                 board[move.toY][move.toX+1] = 0;
    //                 return true;
    //                 }
    //             }
    //         }
    //     }
    //     if(move.toX == 2 && board[move.toY][move.toX] == 0 && board[move.toY][move.toX-1] == 0 && board[move.toY][move.toX+1] == 0){
    //         if(color && castlingRights.first){
    //             if(board[move.toY][move.toX-2] == 2){       //rook
    //                 if(makeMove(board, move, color)){                  //moves king
    //                     board[move.toY][move.toX+1] = 2;      //moves rook
    //                     board[move.toY][move.toX-2] = 0;
    //                     return true;
    //                 }
    //             }
    //         }else if(!color && castlingRights.second){
    //             if(board[move.toY][move.toX-2] == 8){       //rook
    //                 if(makeMove(board, move, color)){                  //moves king
    //                     board[move.toY][move.toX+1] = 8;      //moves rook
    //                     board[move.toY][move.toX-2] = 0;
    //                     return true;
    //                 }
    //             }
    //         }
    //     }
    // }

    return false;
}

void generatePawnMoves(vector<vector<int>> &board, vector<Move>& moves, Move &move, bool color){
    if(color){
        if(board[move.fromY-1][move.fromX] == 0){
            
            moves.push_back({move.fromX, move.fromY, move.fromX, move.fromY-1, NULL});
            if(board[move.fromY-2][move.fromX] == 0 && move.fromY == 6){
                moves.push_back({move.fromX, move.fromY, move.fromX, move.fromY-2, NULL});
            }
        }
        if(board[move.fromY-1][move.fromX+1] >= 7){
            moves.push_back({move.fromX, move.fromY, move.fromX+1, move.fromY-1, NULL});
        }
        if(board[move.fromY-1][move.fromX-1] >= 7){
            moves.push_back({move.fromX, move.fromY, move.fromX-1, move.fromY-1, NULL});
        }
        if(move.prev->fromY == 1 && move.prev->toY == 3){
            if(move.fromY == 3){
                moves.push_back({move.fromX, move.fromY, move.prev->fromX, move.fromY-1, NULL});
            }
        }
    }else {
        if(board[move.fromY+1][move.fromX] == 0){
            moves.push_back({move.fromX, move.fromY, move.fromX, move.fromY+1, NULL});
            if(board[move.fromY+2][move.fromX] == 0 && move.fromY == 1){
                moves.push_back({move.fromX, move.fromY, move.fromX, move.fromY+2, NULL});
            }
        }
        if(board[move.fromY+1][move.fromX+1] <= 6 && board[move.fromY+1][move.fromX+1] != 0){
            moves.push_back({move.fromX, move.fromY, move.fromX+1, move.fromY+1, NULL});
        }
        if(board[move.fromY+1][move.fromX-1] <= 6 && board[move.fromY+1][move.fromX-1] != 0){
            moves.push_back({move.fromX, move.fromY, move.fromX-1, move.fromY+1, NULL});
        }
        if(move.prev->fromY == 6 && move.prev->toY == 4){
            if(move.fromY == 4){
                moves.push_back({move.fromX, move.fromY, move.prev->fromX, move.fromY+1, NULL});
            }
        }
    }
}

bool pawns(vector<vector<int>> &board, Move &move, bool color){      //returns true if move is valid and moves piece if it is
    if(!insideBoard(move)) return false;
    vector<Move> moves;

    generatePawnMoves(board, moves, move, color);
    int logicalValue = checkIfMoveInVector(move, moves);
    if(logicalValue){
        if(makeMove(board, move, color)){
            if(move.toY == 7 || move.toY == 0){
                board[move.toY][move.toX] = color ? 5 : 11;
            }
            if(logicalValue == moves.size()){
                if(color) board[move.toY+1][move.toX] = 0;
                else board[move.toY-1][move.toX] = 0;
            }
            return true;            
        }
    }
    return false;
}

bool rooks(vector<vector<int>> &board, Move &move, bool color){
    if(!insideBoard(move)) return false;
    vector<Move> moves;
    generateSlidingMoves(board, moves, move.fromX, move.fromY, 1, 0);
    generateSlidingMoves(board, moves, move.fromX, move.fromY, -1, 0);
    generateSlidingMoves(board, moves, move.fromX, move.fromY, 0, 1);
    generateSlidingMoves(board, moves, move.fromX, move.fromY, 0, -1);

    if(checkIfMoveInVector(move, moves)){
        return makeMove(board, move, color);
    }

    return false;
}

void generateKnightMoves(vector<vector<int>> &board, vector<Move> &initial, Move &move){
    bool color = board[move.fromY][move.fromX] >= 7 ? 0 : 1;

    for(int i = -2; i <= 2; i++){
        for(int j = -2; j <= 2; j++){
            if(abs(i*j) != 2) continue;
            
            if(move.fromX+j < 0 || move.fromX+j > 7 || move.fromY+i < 0 || move.fromY+i > 7) continue;

            if(color){
                if(board[move.fromY+i][move.fromX+j] >= 7 || board[move.fromY+i][move.fromX+j] == 0){
                    initial.push_back({move.fromX, move.fromY, move.fromX+j, move.fromY+i, NULL});
                }
            }else{
                if(board[move.fromY+i][move.fromX+j] <= 6){
                    initial.push_back({move.fromX, move.fromY, move.fromX+j, move.fromY+i, NULL});
                }
            }
        }
    }
    
}

bool knights(vector<vector<int>> &board, Move &move, bool color){
    if(!insideBoard(move)) return false;

    vector<Move> moves;
    generateKnightMoves(board, moves, move);

    if(checkIfMoveInVector(move, moves)){
        return makeMove(board, move, color);
    }

    return false;
}

bool bishops(vector<vector<int>> &board, Move &move, bool color) {
    if(!insideBoard(move)) return false;
    vector<Move> moves;
    generateSlidingMoves(board, moves, move.fromX, move.fromY, 1, 1);
    generateSlidingMoves(board, moves, move.fromX, move.fromY, -1, -1);
    generateSlidingMoves(board, moves, move.fromX, move.fromY, -1, 1);
    generateSlidingMoves(board, moves, move.fromX, move.fromY, 1, -1);

    if(checkIfMoveInVector(move, moves)){
        return makeMove(board, move, color);
    }

    return false;

}
bool queens(vector<vector<int>> &board, Move &move, bool color) {
    if(!insideBoard(move)) return false;
    vector<Move> moves;

    for(int i = -1; i <= 1; i++){
        for(int j = -1; j <= 1; j++){
            if(i==0 && j==0) continue;
            generateSlidingMoves(board, moves, move.fromX, move.fromY, i, j);
        }
    }

    if(checkIfMoveInVector(move, moves)){
        return makeMove(board, move, color);
    }

    return false;
}

bool checkForMate(vector<vector<int>> &board, bool colorUnderThreat){
    vector<Move> moves;
    return false;
}

bool evalCurMove(vector<vector<int>> &board, Move move, bool color, pair<bool, bool> &castlingRights) { // returns true if move was made
    int figura = board[move.fromY][move.fromX];
    if(figura == 0) return false;
    bool valid = false;
    switch (figura % 6) {
        case 0:
            valid = kings(board, move, color, castlingRights);
            break;
        case 1:
            valid = pawns(board, move, color);
            break;
        case 2:
            valid = rooks(board, move, color);
            break;
        case 3:
            valid = knights(board, move, color);
            break;
        case 4:
            valid = bishops(board, move, color);
            break;
        case 5:
            valid = queens(board, move, color);
    }

    return valid;
}

//white: pawns:1 rooks:2 knight:3 bishop:4 queen:5 king:6
//black: pawns:7 rooks:8 knight:9 bishop:10 queen:11 king:12

int main() {
    vector<vector<int>> board(8, vector<int>(8, 0));
    setUpBoard(board);

    bool endOfGame = false;
    int nOfMoves = 1;
    string strMove = "";
    Move prMove(0, 0, 0, 0, NULL);
    Move move(0, 0, 0, 0, &prMove);  
    pair<bool, bool> castlingRights = {true, true};

    while(!endOfGame) {
        printBoard(board);
        if(kingInCheck(board, move, nOfMoves % 2 == 1? true : false)){
            if(endOfGame = checkForMate(board, nOfMoves % 2 == 1? true : false)){       //SETS end of game
                cout << "CHECKMATE";
                break;
            }
        }
        if(nOfMoves % 2 == 1) {
            cout << "White to move: \n";
        }
        else{
            cout << "Black to move: \n";
        }


        cin >> strMove;
        updatePrevBoard(&move);
        string2Move(strMove, &move);
        while(!insideBoard(move) || !evalCurMove(board, move, nOfMoves % 2 == 1? true : false, castlingRights)){

            cout << "invalid move. "; 
            if(nOfMoves % 2 == 1) {
                cout << "White to move: \n";
            }
            else{
                cout << "Black to move: \n";
            }
            cin >> strMove;
            string2Move(strMove, &move);
        }

        nOfMoves++;
    }
}