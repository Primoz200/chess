#include <iostream>
#include <vector>
#include <string>
#include <map> 
#include <array>
#include <stdint.h>
#include "bitBoards.h"
#include "types.h"
#include "bot.h"

using namespace std;

#define USER_V_USER 1
#define USER_V_BOT 2
#define BOT_V_BOT 3
#define IN_GAME 1
#define WHITE_WIN 2
#define BLACK_WIN 3
#define STALEMATE 4

//map<int, string> figure = {{0, "-"},{1, "P"},{2, "R"},{3, "N"},{4, "B"},{5, "Q"},{6, "K"},{7, "p"},{8, "r"},{9, "n"},{10, "b"},{11, "q"},{12, "k"}};

map<char, int> figureStr2number = {{' ', 0},{'P', 1},{'R', 2},{'N', 3},{'B', 4},{'Q', 5},{'K', 6},{'p', 7},{'r', 8},{'n', 9},{'b', 10},{'q', 11},{'k', 12}};

map<int, string> figure = {
    {0, " "},
    {1, "♟"},
    {2, "♜"},
    {3, "♞"},
    {4, "♝"},
    {5, "♛"},
    {6, "♚"},
    {7, "♙"},
    {8, "♖"},
    {9, "♘"},
    {10, "♗"},
    {11, "♕"},
    {12, "♔"}
};

void resetCastlingRights(CastlingRights* rights, pair<pair<bool, bool>, pair<bool, bool>> options={{true, true},  {true, true}}){
    rights->whiteRight=options.first.first;
    rights->whiteLeft=options.first.second;
    rights->blackRight=options.second.first;
    rights->blackLeft=options.second.second;
}


void setUpBoard(vector<vector<int>>  &v, string fen="rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR") {
    int nextSquare = 0;
    for(char a : fen){          //2r4k/2p1PQpp/q7/4p2/2n2P2/2P3P1/P6P/5K2
        if(a == '/') continue;  
        if(isdigit(a)){
            for(int i = 0; i < a - '0'; i++){
                v[nextSquare/8][nextSquare%8] = 0;
                nextSquare++;
            }
        }else{
            v[nextSquare/8][nextSquare%8] = figureStr2number[a];
            nextSquare++;
        }
    }
}

void printChars(char c, int n){
    cout << "--";
    for(int i = 0; i < n; i++){
        if(i % 4 == 0) cout << "|";
        else cout << "-";
    }
}


void printBoard(vector<vector<int>> &v) {
    cout << "  ";
    for (int k = 0; k < 8; k++) {
        cout << "| " << string(1, 'A' + k) << " ";
    }
    cout << "|\n";
    printChars('-', 33);
    cout << "\n";
    for (int i = 0; i < v.size(); i++) {
        cout << (8-i) << " | ";
        for (int j = 0; j < v[i].size(); j++) {
            cout << figure[v[i][j]] << " | ";
            // if (v[i][j] < 10) { cout << " "; }
        }
        cout << "\n";
        printChars('-', 33);
        cout << "\n";
    }
    cout << "  ";
    for (int k = 0; k < 8; k++) {
        cout << "| " << string(1, 'A' + k) << " ";
    }
    cout << "|\n";
}

void printBitboard(uint64_t a) {
    for(int i = 0; i < 64; i++){
        
        if( i % 8 ==  0) cout << "\n";
        cout << ((a>>i) & 1L) << " ";
    }
}

bool kingInCheck(vector<vector<int>> &board, bool color){ //retruns true if king of color is in check
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

   if(kingInCheck(board, color)) {
        board[move.fromY][move.fromX] = board[move.toY][move.toX];
        board[move.toY][move.toX] = 0;
        return false;
    }
    return true;
}

void forceMove(vector<vector<int>> &board, Move move, int oldPiece=0){                 //ONLY use on tempboards
    board[move.toY][move.toX] = board[move.fromY][move.fromX];
    board[move.fromY][move.fromX] = oldPiece;
}

int checkIfMoveInVector(Move &move, vector<Move> &moves){       //return value is the index of the move + 1 to make enPassant checks easier while also being valid if index = 0
    for(int i = 0; i < moves.size(); i++){
        if(move.equals(moves[i])) return i+1;
    }
    return 0;
}

bool allMovesNull(vector<Move> &moves){
    for(Move move : moves){
        if(!move.isNull()) return false;
    }

    return true;
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

void generateKingMoves(vector<vector<int>> &board, vector<Move>& moves, Move &move, CastlingRights &castlingRights){
    bool color = board[move.fromY][move.fromX] >= 7 ? 0 : 1;

    vector<vector<int>> tempBoard = board;
    for(int x = -1; x <= 1; x++){
        for(int y = -1; y <= 1; y++){
            if(getBitNr(move.fromX+x, move.fromY+y) >= 64 || getBitNr(move.fromX+x, move.fromY+y) < 0) continue;    //if clause is true the square is outside of the board
            if(x == 0 && y == 0) continue;
            if(tempBoard[move.fromY+y][move.fromX+x] == 0){
                tempBoard[move.fromY+y][move.fromX+x] = board[move.fromY][move.fromX];
                tempBoard[move.fromY][move.fromX] = 0;
                if(!kingInCheck(tempBoard, color)){
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
            if(!kingInCheck(tempBoard, color)){
                moves.push_back({move.fromX, move.fromY, move.fromX+x, move.fromY+y, NULL});
            }
            tempBoard[move.fromY+y][move.fromX+x] = board[move.fromY+y][move.fromX+x];
            tempBoard[move.fromY][move.fromX] = board[move.fromY][move.fromX];    
        }
    }

    int rookNr = color ? 2 : 8;
    int kingNr = color ? 6 : 12;
    int rank = color ? 7 : 0;

    if(((color && castlingRights.whiteRight) || (!color && castlingRights.blackRight)) && !kingInCheck(board, color)){
        if(move.fromX == 4 && move.fromY == rank){
            if(tempBoard[move.fromY][move.fromX+1] == 0 && tempBoard[move.fromY][move.fromX+2] == 0 && tempBoard[move.fromY][move.fromX+3] == rookNr){
                tempBoard[move.fromY][move.fromX+1] = kingNr;
                tempBoard[move.fromY][move.fromX] = 0;
                if(!kingInCheck(tempBoard, color)){
                    moves.push_back({move.fromX, move.fromY, move.fromX+2, move.fromY, NULL});
                }
                tempBoard[move.fromY][move.fromX+1] = 0;
                tempBoard[move.fromY][move.fromX] = kingNr;
            }
        }
    }

    if(((color && castlingRights.whiteLeft) || (!color && castlingRights.blackLeft)) && !kingInCheck(board, color)){
        if(move.fromX == 4 && move.fromY == rank){
            if(tempBoard[move.fromY][move.fromX-1] == 0 && tempBoard[move.fromY][move.fromX-2] == 0 && tempBoard[move.fromY][move.fromX-3] == 0 && tempBoard[move.fromY][move.fromX-4] == rookNr){
                tempBoard[move.fromY][move.fromX-1] = kingNr;
                tempBoard[move.fromY][move.fromX] = 0;
                if(!kingInCheck(tempBoard, color)){
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

            if(move.fromY-2 >= 0 && board[move.fromY-2][move.fromX] == 0 && move.fromY == 6){
                moves.push_back({move.fromX, move.fromY, move.fromX, move.fromY-2, NULL});
            }
        }
        if(move.fromX+1 < 8 && board[move.fromY-1][move.fromX+1] >= 7){
            moves.push_back({move.fromX, move.fromY, move.fromX+1, move.fromY-1, NULL});
        }
        if(move.fromX-1 >= 0 && board[move.fromY-1][move.fromX-1] >= 7){
            moves.push_back({move.fromX, move.fromY, move.fromX-1, move.fromY-1, NULL});
        }
        if(move.prev->fromY == 1 && move.prev->toY == 3 && board[move.prev->toY][move.prev->toX] == 7){
            if(move.fromY == 3 && abs(move.fromX-move.prev->toX)==1){
                moves.push_back({move.fromX, move.fromY, move.prev->fromX, move.fromY-1, NULL});
            }
        }
    }else { 
        if(board[move.fromY+1][move.fromX] == 0){
            moves.push_back({move.fromX, move.fromY, move.fromX, move.fromY+1, NULL});

            if(move.fromY+2 < 8 && board[move.fromY+2][move.fromX] == 0 && move.fromY == 1){
                moves.push_back({move.fromX, move.fromY, move.fromX, move.fromY+2, NULL});
            }
        }
        if(move.fromX+1 < 8 && board[move.fromY+1][move.fromX+1] <= 6 && board[move.fromY+1][move.fromX+1] != 0){
            moves.push_back({move.fromX, move.fromY, move.fromX+1, move.fromY+1, NULL});
        }
        if(move.fromX-1 >= 0 && board[move.fromY+1][move.fromX-1] <= 6 && board[move.fromY+1][move.fromX-1] != 0){
            moves.push_back({move.fromX, move.fromY, move.fromX-1, move.fromY+1, NULL});
        }
        if(move.prev->fromY == 6 && move.prev->toY == 4 && board[move.prev->toY][move.prev->toX] == 1){
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

void generateMoves(vector<vector<int>> &board, bool color, vector<Move> &moves, Move &lastMove, CastlingRights &castlingRights){
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
                    int previousPiece = board[moves[a].toY][moves[a].toX];
                    if(!makeMove(tempBoard, moves[a], color)){
                        moves[a].fromX = 0;
                        moves[a].fromY = 0;
                        moves[a].toX = 0;
                        moves[a].toY = 0;
                    }else{
                        forceMove(tempBoard, moves[a].reverseMove(), previousPiece);
                    }
                }
                nrGeneratedMoves = moves.size();
            }
        }
    }
}

void kings(vector<vector<int>> &board, Move &move, CastlingRights &castlingRights){
    bool color = board[move.fromY][move.fromX]==6 ? true : false;

    if(move.fromX-move.toX == 2) {
        board[move.fromY][move.fromX-1] = board[move.fromY][move.fromX-4];
        board[move.fromY][move.fromX-4] = 0;
    }
    if(move.toX-move.fromX == 2) {
        board[move.fromY][move.fromX+1] = board[move.fromY][move.fromX+3];
        board[move.fromY][move.fromX+3] = 0;
    }        
    if(color) {castlingRights.whiteRight = false; castlingRights.whiteLeft = false;}
    else {castlingRights.blackRight = false; castlingRights.blackRight = false;}
    makeMove(board, move, color);
}

void pawns(vector<vector<int>> &board, Move move, bool color){
    if(color && move.prev->fromY==1 && move.prev->toY==3){
        if(move.toX == move.prev->toX && move.toY == 2 && move.toY+1 < 8){
            board[move.toY+1][move.toX] = 0;
        }
    }
    if(!color && move.prev->fromY==6 && move.prev->toY==4){
        if(move.toX == move.prev->toX && move.toY == 5 && move.toY-1 >= 0){
            board[move.toY-1][move.toX] = 0;
        }
    }
    if(move.toY == 0 || move.toY == 7){
        board[move.toY][move.toX] = color ? 5 : 11;
    }
    makeMove(board, move, color);
}

void rooks(vector<vector<int>> &board, Move move, bool color, CastlingRights &castlingRights){
    if(move.fromX==7 && move.fromY==7) {castlingRights.whiteRight=false;}
    if(move.fromX==0 && move.fromY==7) {castlingRights.whiteLeft=false;}
    if(move.fromX==0 && move.fromY==0) {castlingRights.blackLeft=false;}
    if(move.fromX==7 && move.fromY==0) {castlingRights.blackRight=false;}

    makeMove(board, move, color);
 }

bool evalCurMove(vector<vector<int>> &board, Move move, bool color, CastlingRights &castlingRights) { // returns true if move was made
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
        case 2:
            rooks(board, move, color, castlingRights);
            break;             
        case 3:                         //knights bishops and queens dont have special moves that have to be handled seperatl
        case 4:
        case 5:
            makeMove(board, move, color);
            break;
    }

    return valid;
}

void getMove(string &strMove, vector<vector<int>> &board, vector<Move> &moves, int typeOfGame, bool color){
    if(typeOfGame == USER_V_USER){
        strMove = "";
        cin >> strMove;
    }else if(typeOfGame == USER_V_BOT){
        if(color){
            cin >> strMove;
        }else{
            strMove = getBotMove(board, moves);
            while(strMove == "a8a8"){
                strMove = getBotMove(board, moves);
            }
            cout << strMove;
        }
    }
    else if(typeOfGame == BOT_V_BOT){
        strMove = getBotMove(board, moves);
        cout << strMove;
    }
}

int gameLoop(vector<vector<int>> &board, int typeOfGame, int color, CastlingRights &castlingRights ) {
    int gameState = IN_GAME;
    string moveString;
    vector<Move> moves;
    Move nullMove = {0, 0, 0, 0, NULL};
    Move curMove = {0, 0, 0, 0, &nullMove};
    int moveNr = 1;

    while(gameState == IN_GAME){
        cout << "\033[2J\033[1;1H";
        printBoard(board);
        generateMoves(board, color, moves, curMove, castlingRights);

        // for(auto i : moves){
        //     cout << i.toString();
        // }

        if(allMovesNull(moves)){
            if(kingInCheck(board, color)){
                gameState = color ? BLACK_WIN : WHITE_WIN;
            }else {
                gameState = STALEMATE;
            }
            break;
        }
        
        if(color){ cout << "White to move:\n";}
        else { cout << "Black to move:\n";}

        curMove.updatePrevMove();
        getMove(moveString, board, moves, typeOfGame, color);
        curMove.string2move(moveString);
        while(!checkIfMoveInVector(curMove, moves)){
            if(moveString == "q") {gameState = STALEMATE; break;}

            cout << "invalid Move\n";
            if(color){ cout << "White to move:\n";}
            else { cout << "Black to move:\n";}

            getMove(moveString, board, moves, typeOfGame, color);
            curMove.string2move(moveString);
        }

        evalCurMove(board, curMove, color, castlingRights);
        color = !color;
        moveNr++;
    }

    return gameState;
}

void postGameOutput(int endState){
    if(endState == STALEMATE){
        cout << "STALEMATE\n";
    }else if(endState == WHITE_WIN){
        cout << "WHITE WINS\n";
    }else if(endState == BLACK_WIN){
        cout << "BLACK WINS\n";
    }
}
//white: pawns:1 rooks:2 knight:3 bishop:4 queen:5 king:6
//black: pawns:7 rooks:8 knight:9 bishop:10 queen:11 king:12

int main() {
    vector<vector<int>> board(8, vector<int>(8, 0));
    setUpBoard(board);

    CastlingRights castlingRights;
    resetCastlingRights(&castlingRights);

    postGameOutput(gameLoop(board, USER_V_BOT, true, castlingRights));
}