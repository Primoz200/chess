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

array<int, 4> enumerateMove(string move) { //vrne indekse v tabeli. pozcija 0 stolpec,1 vrstica (ker e4 in ne 4e) za zacetni square, 2,3 za koncni
    return {move[0] - 'a', 7 - (move[1] - '1'), move[2] - 'a', 7 - (move[3] - '1')};
}

bool kings(vector<vector<int>> &board, array<int, 4> move, bool color){return false;}

bool pawns(vector<vector<int>> &board, array<int, 4> move, bool color){  //returns true if move is valid and moves piece if it is
    // for(int i: move) {
    //     cout << i << " ";
    // }
    if(!color) {
        if(move[3] - move[1] >= 3) {
            return false;
        }else if(move[3] - move[1] == 2){       //move by 2
            if(move[1] != 1) return false;   //not seventh rank
            else if(move[0] != move[2]) return false;   //not same column   
            else if(board[move[3]][move[2]] != 0 || board[move[3]+1][move[2]] != 0) {return false; } //check if space is empty
            else {
                board[move[3]][move[2]] = board[move[1]][move[0]];
                board[move[1]][move[0]] = 0;
                return true;
            }
        }
    }
    
    return false;
}

bool rooks(vector<vector<int>> &board, array<int, 4> move, bool color){return false;}
bool knights(vector<vector<int>> &board, array<int, 4> move, bool color){return false;}
bool bishops(vector<vector<int>> &board, array<int, 4> move, bool color) {return false;}
bool queens(vector<vector<int>> &board, array<int, 4> move, bool color) {return false;}

bool makeCurMove(vector<vector<int>> &board, array<int, 4> enumMove, bool color) { // returns true if move was made
    int figura = board[enumMove[1]][enumMove[0]];
    bool valid = false;
    switch (figura % 6) {
        case 0:
            valid = kings(board, enumMove, color);
            break;
        case 1:
            valid = pawns(board, enumMove, color);
            break;
        case 2:
            valid = rooks(board, enumMove, color);
            break;
        case 3:
            valid = knights(board, enumMove, color);
            break;
        case 4:
            valid = bishops(board, enumMove, color);
            break;
        case 5:
            valid = queens(board, enumMove, color);
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
        makeCurMove(board, enumerateMove(curMove), nOfMoves % 2 == 1? true : false);
        
        nOfMoves++;
    }
}