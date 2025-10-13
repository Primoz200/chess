#include <iostream>
#include <vector>
#include <string>

using namespace std;

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

void printBoard(vector<vector<int>> v) {
    for(auto vrstica: v){
        for(auto polje: vrstica){
            cout << polje << " ";
            if(polje < 10){ cout << " ";} 
        }
        cout << endl;
    }
}

//white: pawns:1 rooks:2 knight:3 bishop:4 queen:5 king:6
//black: pawns:7 rooks:8 knight:9 bishop:10 queen:11 king:12

int main() {
    vector<vector<int>> board(8, vector<int>(8, 0));
    setUpBoard(board);

    bool endOfGame = false;
    int nOfMoves = 0;
    string curMove = "";


    while(!endOfGame) {
        printBoard(board);
        if(nOfMoves % 2 == 0) {
            cout << "White to move: \n";
        }
        else{
            cout << "Black to move \n";
        }

        cin >> curMove;
        nOfMoves++;
    }
}