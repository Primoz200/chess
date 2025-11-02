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

bool kingInCheck(vector<vector<int>> &board, Move &move, bool color){
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
    if(kingInCheck(board, move, !color)){
        cout << "Check \n";
    }

    return true;
}

bool checkIfMoveInVector(Move &move, vector<Move> &moves){
    for(int i = 0; i < moves.size(); i++){
        if(move.equals(moves[i])) return true;
    }
    return false;
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

bool kings(vector<vector<int>> &board, Move &move, bool color, pair<bool, bool> &castlingRights){
    if(!insideBoard(move)) return false;

    if(abs(move.fromX - move.toX) <= 1 && abs(move.fromY - move.toY) <= 1){
        if(makeMove(board, move, color)){
            if(color){castlingRights.first = false;}
            else{ castlingRights.second = false;}
            return true;
        }
    }



    if(move.fromY == move.toY){
        if(move.toX == 6 && board[move.toY][move.toX] == 0 && board[move.toY][move.toX-1] == 0){
            if(color && castlingRights.first){
                if(board[move.toY][move.toX+1] == 2){       //rook
                    if(makeMove(board, move, color)){                  //moves king
                    board[move.toY][move.toX-1] = 2;      //moves rook
                    board[move.toY][move.toX+1] = 0;
                    return true;
                    }
                }
            }else if(!color && castlingRights.second){
                if(board[move.toY][move.toX+1] == 8){       //rook
                    if(makeMove(board, move, color)){                  //moves king
                    board[move.toY][move.toX-1] = 8;      //moves rook
                    board[move.toY][move.toX+1] = 0;
                    return true;
                    }
                }
            }
        }
        if(move.toX == 2 && board[move.toY][move.toX] == 0 && board[move.toY][move.toX-1] == 0 && board[move.toY][move.toX+1] == 0){
            if(color && castlingRights.first){
                if(board[move.toY][move.toX-2] == 2){       //rook
                    if(makeMove(board, move, color)){                  //moves king
                        board[move.toY][move.toX+1] = 2;      //moves rook
                        board[move.toY][move.toX-2] = 0;
                        return true;
                    }
                }
            }else if(!color && castlingRights.second){
                if(board[move.toY][move.toX-2] == 8){       //rook
                    if(makeMove(board, move, color)){                  //moves king
                        board[move.toY][move.toX+1] = 8;      //moves rook
                        board[move.toY][move.toX-2] = 0;
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool pawns(vector<vector<int>> &board, Move &move, bool color){      //returns true if move is valid and moves piece if it is
    if(!insideBoard(move)) return false;
    
    if(color) {     //white pawns
        if(move.fromY - move.toY > 2) {
            return false;
        }else if(move.fromY - move.toY == 2){       //move by 2
            if(move.fromY != 6) return false;   //not seventh rank
            else if(move.fromX != move.toX) return false;   //not same column   
            else if(board[move.toY][move.toX] != 0 || board[move.toY+1][move.toX] != 0) return false; //check if space is empty
            else {
                return makeMove(board, move, color);
            }
        }else if(move.fromY - move.toY == 1){           //move by 1
            if(move.fromX == move.toX){                 //same file
                if(board[move.toY][move.toX] != 0) return false;
                else {
                    if(makeMove(board, move, color)){
                        if(move.toY == 0){
                            board[move.toY][move.toX] = 5;  //queening
                        } 
                        return true;
                    }
                }
            }else if(abs(move.fromX - move.toX) != 1) return false;
            else if(board[move.toY][move.toX] >= 7) {     
                if(makeMove(board, move, color)){
                    if(move.toY == 0){
                        board[move.toY][move.toX] = 5;  //queening
                    }
                    return true;
                }

            }
            else if(board[move.toY][move.toX] == 0){                        //en passant
                if((move.prev)->fromY == 1 && (move.prev)->toY == 3){
                    if((move.prev)->fromX == move.toX){
                        if(makeMove(board, move, color)){
                            board[move.toY+1][move.toX]=0;
                            return true;
                        }
                    }
                }
            }
        } 

    }
    else {          //black pawns
        if(move.toY - move.fromY > 2) {
            return false;
        }else if(move.toY - move.fromY == 2){       //move by 2
            if(move.fromY != 1) return false;   //not seventh rank
            else if(move.fromX != move.toX) return false;   //not same column   
            else if(board[move.toY][move.toX] != 0 || board[move.toY-1][move.toX] != 0) {return false; } //check if space is empty
            else {
                return makeMove(board, move, color);
            }
        }else if(move.toY - move.fromY == 1){           //move by 1
            if(move.fromX == move.toX){                 //same file
                if(board[move.toY][move.toX] != 0) return false;
                else {
                    if(makeMove(board, move, color)){
                        if(move.toY == 7){
                            board[move.toY][move.toX] = 11;  //queening
                        }         
                        return true;
                    }
                }
            }else if(abs(move.fromX - move.toX) != 1) return false;
            else if(board[move.toY][move.toX] <= 6 &&board[move.toY][move.toX] != 0) {     //if smaller than 6 => is a white piece
                if(makeMove(board, move, color)){
                    if(move.toY == 7){
                        board[move.toY][move.toX] = 11;  //queening
                    }         
                    return true;
                }
            }
            else if(board[move.toY][move.toX] == 0){                                //en passant
                if((move.prev)->fromY == 6 && (move.prev)->toY == 4){
                    if((move.prev)->fromX == move.toX){
                        if(makeMove(board, move, color)){
                            board[move.toY-1][move.toX]=0;
                            return true;
                        }
                    }
                }
            }
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

bool knights(vector<vector<int>> &board, Move &move, bool color){
    if(!insideBoard(move)) return false;

    vector<Move> moves;
    for(int i = -2; i <= 2; i++){
        for(int j = -2; j <= 2; j++){
            if(abs(i*j) != 2) continue;
            
            if(move.toX+j < 0 || move.toX+j > 7 || move.toY+i < 0 || move.toY+i > 7) continue;

            if(color){
                if(board[move.toY+i][move.toX+j] >= 7 || board[move.toY+i][move.toX+j] == 0){
                    moves.push_back({move.fromX, move.fromY, move.fromX+j, move.fromY+i, NULL});
                }
            }else{
                if(board[move.toY+i][move.toX+j] <= 6){
                    moves.push_back({move.fromX, move.fromY, move.fromX+j, move.fromY+i, NULL});
                }
            }
        }
    }
    
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
        if(nOfMoves % 2 == 1) {
            cout << "White to move: \n";
        }
        else{
            cout << "Black to move: \n";
        }

        cin >> strMove;
        updatePrevBoard(&move);
        string2Move(strMove, &move);
        while(!evalCurMove(board, move, nOfMoves % 2 == 1? true : false, castlingRights)){

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