#include <iostream>
#include <vector>
#include <string>
#include <map> 
#include <array>
#include <stdint.h>
#include "bitBoards.h"

#define USER_V_USER 1;
#define USER_V_BOT 2;
#define BOT_V_BOT 3;

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

void string2Move(string moveS, Move* move) { //TODO FIX
    move->fromX = moveS[0] - 'a';
    move->fromY = 7 - (moveS[1] - '1');
    move->toX = moveS[2] - 'a';
    move->toY = 7 - (moveS[3] - '1');
}

void updatePrevBoard(Move* move){
    move->prev->prev = NULL;
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

void forceMove(vector<vector<int>> &board, Move move){                 //ONLY use on tempboards
    board[move.toY][move.toX] = board[move.fromY][move.fromX];
    board[move.fromY][move.fromX] = 0;
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

void generateKingMoves(vector<vector<int>> &board, vector<Move>& moves, Move &move, pair<bool, bool> &castlingRights){
    bool color = board[move.fromY][move.fromX] >= 7 ? 0 : 1;

    vector<vector<int>> tempBoard = board;
    for(int x = -1; x <= 1; x++){
        for(int y = -1; y <= 1; y++){
            if(getBitNr(move.fromX+x, move.fromY+y) >= 64 || getBitNr(move.fromX+x, move.fromY+y) < 0) continue;    //if clause is true the square is outside of the board
            if(x == 0 && y == 0) continue;
            if(tempBoard[move.fromY+y][move.fromX+x] == 0){
                tempBoard[move.fromY+y][move.fromX+x] = board[move.fromY][move.fromX];
                tempBoard[move.fromY][move.fromX] = 0;
                if(!kingInCheck(tempBoard, move, color)){
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
            if(!kingInCheck(tempBoard, move, color)){
                moves.push_back({move.fromX, move.fromY, move.fromX+x, move.fromY+y, NULL});
            }
            tempBoard[move.fromY+y][move.fromX+x] = board[move.fromY+y][move.fromX+x];
            tempBoard[move.fromY][move.fromX] = board[move.fromY][move.fromX];    
        }
    }

    int rookNr = color ? 2 : 8;
    int kingNr = color ? 6 : 12;
    int rank = color ? 7 : 0;
    if(((color && castlingRights.first) || (!color && castlingRights.second)) && !kingInCheck(board, move, color)){
        if(move.fromX == 4 && move.fromY == rank){
            if(tempBoard[move.fromY][move.fromX+1] == 0 && tempBoard[move.fromY][move.fromX+2] == 0 && tempBoard[move.fromY][move.fromX+3] == rookNr){
                tempBoard[move.fromY][move.fromX+1] = kingNr;
                tempBoard[move.fromY][move.fromX] = 0;
                if(!kingInCheck(tempBoard, move, color)){
                    moves.push_back({move.fromX, move.fromY, move.fromX+2, move.fromY, NULL});
                }
                tempBoard[move.fromY][move.fromX+1] = 0;
                tempBoard[move.fromY][move.fromX] = kingNr;
            }
            
            if(tempBoard[move.fromY][move.fromX-1] == 0 && tempBoard[move.fromY][move.fromX-2] == 0 && tempBoard[move.fromY][move.fromX-3] == 0 && tempBoard[move.fromY][move.fromX-4] == rookNr){
                tempBoard[move.fromY][move.fromX-1] = kingNr;
                tempBoard[move.fromY][move.fromX] = 0;
                if(!kingInCheck(tempBoard, move, color)){
                    moves.push_back({move.fromX, move.fromY, move.fromX-2, move.fromY, NULL});
                }
                tempBoard[move.fromY][move.fromX-1] = 0;
                tempBoard[move.fromY][move.fromX] = kingNr;
            }
        }
    }
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
        if(move.prev->fromY == 1 && move.prev->toY == 3 && board[move.prev->toY][move.prev->toY] == 7){
            if(move.fromY == 3 && abs(move.fromX-move.prev->toX)==1){
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
        if(move.prev->fromY == 6 && move.prev->toY == 4 && board[move.prev->toY][move.prev->toY] == 1){
            if(move.fromY == 4  && abs(move.fromX-move.prev->toX)==1){
                moves.push_back({move.fromX, move.fromY, move.prev->fromX, move.fromY+1, NULL});
            }
        }
    }
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

void generateMoves(vector<vector<int>> &board, bool color, vector<Move> &moves, Move lastMove, pair<bool, bool> &castlingRights){
    vector<vector<int>> tempBoard = board;
    Move refMove = {0, 0, 0, 0, &lastMove};      //this move is used only for its fromX and fromY values, its not supposed to be played. Pointer must be right in case of enpassant blocks
    moves.clear();

    int nrGeneratedMoves = 0;
    for(int i = 0; i <= 7; i++){       
        for(int j = 0; j <= 7; j++){
            if(board[i][j] == 0) continue;
            if((color && board[i][j] <= 6) || (!color && board[i][j] >= 7)){
                refMove.fromX = j;
                refMove.fromY = i;
                switch (board[i][j] % 6) {
                    case 0:
                        generateKingMoves(board, moves, refMove, castlingRights);
                        break;
                    case 1:
                        generatePawnMoves(board, moves, refMove, color);
                        break;
                    case 2:
                        generateSlidingMoves(board, moves, j, i, 0, 1);
                        generateSlidingMoves(board, moves, j, i, 0, -1);
                        generateSlidingMoves(board, moves, j, i, 1, 0);
                        generateSlidingMoves(board, moves, j, i, -1, 0);
                        break;
                    case 3:
                        generateKnightMoves(board, moves, refMove);
                        break;
                    case 4:
                        generateSlidingMoves(board, moves, j, i, 1, 1);
                        generateSlidingMoves(board, moves, j, i, 1, -1);
                        generateSlidingMoves(board, moves, j, i, -1, 1);
                        generateSlidingMoves(board, moves, j, i, -1, -1);
                        break;
                    case 5:
                        for(int k1 = -1; k1 <= 1; k1++){
                            for(int k2 = -1; k2 <= 1; k2++){
                                if(k1==0 && k2==0) continue;
                                generateSlidingMoves(board, moves, j, i, k1, k2);
                            }
                        }
                        break;
                }

                for(int a = nrGeneratedMoves; a < moves.size(); a++){
                    if(!makeMove(tempBoard, moves[a], color)){
                        moves[a].fromX = 0;
                        moves[a].fromY = 0;
                        moves[a].toX = 0;
                        moves[a].toY = 0;
                    }else{
                        forceMove(tempBoard, moves[a].reverseMove());
                    }
                }
                nrGeneratedMoves = moves.size();
            }
        }
    }
}

void kings(vector<vector<int>> &board, Move &move, pair<bool, bool> &castlingRights){
    bool color = board[move.fromY][move.fromX]==6 ? true : false;

    if(move.fromX-move.toX == 2) {
        board[move.fromY][move.fromX-1] = board[move.fromY][move.fromX-4];
        board[move.fromY][move.fromX-4] = 0;
    }
    if(move.toX-move.fromX == 2) {
        board[move.fromY][move.fromX+1] = board[move.fromY][move.fromX+3];
        board[move.fromY][move.fromX+3] = 0;
    }        
    color ? castlingRights.first = false : castlingRights.second = false;
    makeMove(board, move, color);
}

void pawns(vector<vector<int>> &board, Move move, bool color){
    if(color && move.prev->fromY==1 && move.prev->toY==3){
        if(move.toX == move.prev->toX && move.toY == 2 ){
            board[move.toY+1][move.toX] = 0;
        }
    }
    if(!color && move.prev->fromY==6 && move.prev->toY==4){
        if(move.toX == move.prev->toX && move.toY == 5){
            board[move.toY-1][move.toX] = 0;
        }
    }
    if(move.toY == 0 || move.toY == 7){
        board[move.toY][move.toX] = color ? 5 : 11;
    }
    makeMove(board, move, color);
}

bool evalCurMove(vector<vector<int>> &board, Move move, bool color, pair<bool, bool> &castlingRights) { // returns true if move was made
    int figura = board[move.fromY][move.fromX];
    if(figura == 0) return false;
    bool valid = false;
    switch (figura % 6) {
        case 0:
            kings(board, move, castlingRights);
            break;
        case 1:
            pawns(board, move, color);
            break;
        case 2:                         //knights bishops rooks and queens dont have special moves that have to be handled seperatly
        case 3:
        case 4:
        case 5:
            makeMove(board, move, color);
            break;
    }

    return valid;
}


//white: pawns:1 rooks:2 knight:3 bishop:4 queen:5 king:6
//black: pawns:7 rooks:8 knight:9 bishop:10 queen:11 king:12

int main() {
    vector<vector<int>> board(8, vector<int>(8, 0));
    setUpBoard(board);

    vector<Move> moves;
    bool endOfGame = false;
    int nOfMoves = 1;
    string strMove = "";
    Move prMove(0, 0, 0, 0, NULL);
    Move move(0, 0, 0, 0, &prMove);  
    pair<bool, bool> castlingRights = {true, true};
    int typeOfGame = 1;

    while(!endOfGame) {
        bool color = nOfMoves % 2 == 1? true : false;

        move.toString();
        printBoard(board);
        generateMoves(board, color, moves, move, castlingRights);

        if(moves.empty()){
            if(kingInCheck(board, move, color)){
                cout << "CHECKMATE\n";
                break;
            }
            else{
                cout << "STALEMATE";
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
        if(strMove == "q") break;
        updatePrevBoard(&move);
        string2Move(strMove, &move);
        while(!insideBoard(move) || !checkIfMoveInVector(move, moves)){

            cout << "invalid move. "; 
            if(nOfMoves % 2 == 1) {
                cout << "White to move: \n";
            }
            else{
                cout << "Black to move: \n";
            }
            cin >> strMove;
        if(strMove == "q") break;

            string2Move(strMove, &move);
        }
        
        evalCurMove(board, move, color, castlingRights);
        nOfMoves++;
    }
}