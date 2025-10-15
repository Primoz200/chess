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

        Move(int x1, int y1, int x2, int y2) {
            fromX = x1;
            fromY = y1;
            toX = x2;
            toY = y2;
        }

};

// array<int, 4> enumerateMove(string move) { //vrne indekse v tabeli. pozcija 0 stolpec,1 vrstica (ker e4 in ne 4e) za zacetni square, 2,3 za koncni
//     return {move[0] - 'a', 7 - (move[1] - '1'), move[2] - 'a', 7 - (move[3] - '1')};
// }

Move string2Move(string moveS) {
    Move move(moveS[0] - 'a', 7 - (moveS[1] - '1'), moveS[2] - 'a', 7 - (moveS[3] - '1'));
    return move;
}

void makeMove(vector<vector<int>> &board, Move &move) {
    board[move.toY][move.toX] = board[move.fromY][move.fromX];
    board[move.fromY][move.fromX] = 0;
}

bool kings(vector<vector<int>> &board, Move move, bool color){return false;}

bool pawns(vector<vector<int>> &board, Move move, bool color){  //returns true if move is valid and moves piece if it is
    if(color) {     //white pawns
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

bool rooks(vector<vector<int>> &board, Move move, bool color){
    return false;
}
bool knights(vector<vector<int>> &board, Move move, bool color){return false;}
bool bishops(vector<vector<int>> &board, Move move, bool color) {return false;}
bool queens(vector<vector<int>> &board, Move move, bool color) {return false;}

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
    string curMove = "";


    while(!endOfGame) {
        printBoard(board);
        if(nOfMoves % 2 == 1) {
            cout << "White to move: \n";
        }
        else{
            cout << "Black to move: \n";
        }

        cin >> curMove;
        while(!evalCurMove(board, string2Move(curMove), nOfMoves % 2 == 1? true : false)){
            cout << "invalid move. "; 
            if(nOfMoves % 2 == 1) {
                cout << "White to move: \n";
            }
            else{
                cout << "Black to move: \n";
            }
            cin >> curMove;
        }
        
        nOfMoves++;
    }
}