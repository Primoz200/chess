#include <iostream>
#include <vector>
#include <string>
#include <map> 
#include <array>

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
            cout << fromX << " " << fromY  << " " << toX << " " << toY << endl;
            cout << prev->fromX << " " << prev->fromY << " " << prev->toX << " " << prev->toY << endl;
        }

};


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

void makeMove(vector<vector<int>> &board, Move &move) {
    board[move.toY][move.toX] = board[move.fromY][move.fromX];
    board[move.fromY][move.fromX] = 0;
}

bool checkDestSquareAndMove(vector<vector<int>> &board, Move &move, bool color){
    if (color) {
        if (board[move.toY][move.toX] > 6 || board[move.toY][move.toX] == 0) {
            makeMove(board, move);
            return true;
        }
    }else{
        if(board[move.toY][move.toX] < 7){
            makeMove(board, move);
            return true;
        }
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

bool kings(vector<vector<int>> &board, Move &move, bool color){
    if(!insideBoard(move)) return false;

    return checkDestSquareAndMove(board, move, color);
}

bool pawns(vector<vector<int>> &board, Move &move, bool color){      //returns true if move is valid and moves piece if it is
    if(!insideBoard(move)) return false;

    if(color) {     //white pawns
        if(board[move.fromY][move.fromX] != 1) return false;
        if(move.fromY - move.toY > 2) {
            return false;
        }else if(move.fromY - move.toY == 2){       //move by 2
            if(move.fromY != 6) return false;   //not seventh rank
            else if(move.fromX != move.toX) return false;   //not same column   
            else if(board[move.toY][move.toX] != 0 || board[move.toY-1][move.toX] != 0) return false; //check if space is empty
            else {
                makeMove(board, move);
                return true;
            }
        }else if(move.fromY - move.toY == 1){           //move by 1
            if(move.fromX == move.toX){                 //same file
                if(board[move.toY][move.toX] != 0) return false;
                else {
                    makeMove(board, move);
                    return true;
                }
            }else if(abs(move.fromX - move.toX) != 1) return false;
            else if(board[move.toY][move.toX] >= 7) {     
                makeMove(board, move);               
                return true;
            }
        } 
    }
    else {          //black pawns
        if(board[move.fromY][move.fromX] != 7) return false;
        if(move.toY - move.fromY > 2) {
            return false;
        }else if(move.toY - move.fromY == 2){       //move by 2
            if(move.fromY != 1) return false;   //not seventh rank
            else if(move.fromX != move.toX) return false;   //not same column   
            else if(board[move.toY][move.toX] != 0 || board[move.toY-1][move.toX] != 0) {return false; } //check if space is empty
            else {
                makeMove(board, move);
                return true;
            }
        }else if(move.toY - move.fromY == 1){           //move by 1
            if(move.fromX == move.toX){                 //same file
                if(board[move.toY][move.toX] != 0) return false;
                else {
                    makeMove(board, move);
                    return true;
                }
            }else if(abs(move.fromX - move.toX) != 1) return false;
            else if(board[move.toY][move.toX] <= 6) {     //if smaller than 6 => is a white piece
                makeMove(board, move);               
                return true;
            }
        } 
    }
    return false;
}

bool rookCheckPath(vector<vector<int>> &board, Move &move) {
    for(int i = 1; i < abs(move.fromX-move.toX); i++){
        if(move.toX > move.fromX) {
            if(board[move.fromY][move.fromX+i] != 0) return false; 
        }else {
            if(board[move.fromY][move.fromX-i] != 0) return false; 
        }
    }

    for(int i = 1; i < abs(move.fromY-move.toY); i++){
        if(move.toY > move.fromY) {
            if(board[move.fromY+i][move.fromX] != 0) return false; 
        }else {
            if(board[move.fromY-i][move.fromX] != 0) return false; 
        }
    }

    return true;
}

bool rooks(vector<vector<int>> &board, Move &move, bool color){
    if(!insideBoard(move)) return false;

    if(!rookCheckPath(board, move)) return false; 

    return checkDestSquareAndMove(board, move, color);
}

bool knights(vector<vector<int>> &board, Move &move, bool color){
    if(!insideBoard(move)) return false;

    if (abs(move.toX - move.fromX) * abs(move.toY - move.fromY) != 2) return false;

    return checkDestSquareAndMove(board, move, color);
}

bool bishopCheckPath(vector<vector<int>> &board, Move &move){
    for(int i = 1; i < abs(move.fromX-move.toX); i++){
        if(move.fromX-move.toX > 0) {
            if(move.fromY-move.toY > 0){
                if(board[move.fromY-i][move.fromX-i] != 0) return false;
            }else{
                if(board[move.fromY+i][move.fromX-i] != 0) return false;
            }
        }else{
            if(move.fromY-move.toY > 0){
                if(board[move.fromY-i][move.fromX+i] != 0) return false;
            }else{
                if(board[move.fromY+i][move.fromX+i] != 0) return false;
            }
        }
    }

    return true;
}
bool bishops(vector<vector<int>> &board, Move &move, bool color) {
    if(!insideBoard(move)) return false;

    if(abs(move.fromX-move.toX) != abs(move.fromY-move.toY)) return  false;

    if(!bishopCheckPath(board, move)) return false;

    return checkDestSquareAndMove(board, move, color);
}
bool queens(vector<vector<int>> &board, Move &move, bool color) {
    if(!insideBoard(move)) return false;

    if(!bishopCheckPath(board, move) && !rookCheckPath(board, move)) return false;

    return checkDestSquareAndMove(board, move, color);

}

bool evalCurMove(vector<vector<int>> &board, Move move, bool color) { // returns true if move was made
    int figura = board[move.fromY][move.fromX];
    bool valid = false;
    switch (figura % 6) {
        case 0:
            valid = kings(board, move, color);
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
        move.toString();
        while(!evalCurMove(board, move, nOfMoves % 2 == 1? true : false)){

            cout << "invalid move. "; 
            if(nOfMoves % 2 == 1) {
                cout << "White to move: \n";
            }
            else{
                cout << "Black to move: \n";
            }
            cin >> strMove;
            string2Move(strMove, &move);
            move.toString();
        }
        
        nOfMoves++;
    }
}