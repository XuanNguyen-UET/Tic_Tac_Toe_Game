// tictactoe.cpp: implement AI engine for tictactoe game
#include<bits/stdc++.h>

using namespace std;

typedef vector<vector<char> > State;
const int SIZE = 3;


// checking if Board is end
// input: State
// output: true if end, false if not yet end
bool CheckFinalState(State Board)
{
    // check rows
    for (int i = 0; i < SIZE; i++)
        if (Board[i][0] == Board[i][1] && Board[i][0] == Board[i][2] && Board[i][0] != ' ') return true;
    // check cols
    for (int i = 0; i < SIZE; i++)
        if (Board[0][i] == Board[1][i] && Board[0][i] == Board[2][i] && Board[0][i] != ' ') return true;
    // check diagonals
    if (Board[0][0] == Board[1][1] && Board[0][0] == Board[2][2] && Board[0][0] != ' ') return true;
    if (Board[0][2] == Board[1][1] && Board[0][2] == Board[2][0] && Board[0][2] != ' ') return true;
    // check full
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (Board[i][j] == ' ') return false;
        }
    }
    return true;
}
/* render the game
    input:  State  
    output: Board game
*/
void renderGame(State state)
{
    cout << endl;
    for (int i = 0; i < 3 ; i++){
        cout << "       |       |       " << endl;
        for (int j = 0; j < 3;j++){
            if (state[i][j] == 'x'){ 
                cout << "   x   ";  // state = 1 --> human
            }
            if (state[i][j] == 'o'){ 
                cout << "   o   ";  // state = -1 --> AI
            }
            if (state[i][j] == ' '){ 
                cout << "       ";  // state = 0 --> empty
            }
            if (j != 2){
                cout << "|"; // add a line between 2 columns
            }
        }
        if (i != 2 ){
            cout << endl << "_______|_______|_______" << endl; // add a line between lines
        } else {
            cout << endl << "       |       |       " << endl; // create lines
        }
    }
    cout << endl;
}

/*  detect Next player
    input: State
    output: x if next player is x
            o if next player is o
*/
char GetNextPlayer(State Board)
{
    int countX = 0, countO = 0;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (Board[i][j] == 'x') countX++;
            else if (Board[i][j] == 'o') countO++;
        }
    }
    if (countX == countO) return 'x';
    else return 'o';
}


// copy the current game
//  input: State, 2 indexes, char currentplayer
//  output: State
State CopyGame(State Board, int i, int j, char player)
{
    State newState = State(3, vector<char>(3, ' '));
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            newState[i][j] = Board[i][j];
        }
    }
    newState[i][j] = player;
    return newState;
}


// Calculate valid moves
// input : State
// output : vector<State>
vector<State> GetNextStates(State Board)
{
    char NextPlayer = GetNextPlayer(Board);
    vector<State> ValidStates;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (Board[i][j] == ' ') {
                State nextState = CopyGame(Board, i, j, NextPlayer);
                ValidStates.push_back(nextState);
            }
        }
    }
    return ValidStates;
}

// Final State score
// input : State
// output : 1 if x wins,0 if draw,-1 if o wins
// only called when IsEnd is true
int FinalStateScore(State Board)
{
    for (int i = 0; i < SIZE; i++)
        if (Board[i][0] == Board[i][1] && Board[i][0] == Board[i][2] && Board[i][0] != ' ') 
            return Board[i][0] == 'x' ? 1 : -1;
    for (int i = 0; i < SIZE; i++)
        if (Board[0][i] == Board[1][i] && Board[0][i] == Board[2][i] && Board[0][i] != ' ')
            return Board[0][i] == 'x' ? 1 : -1;
    if (Board[0][0] == Board[1][1] && Board[0][0] == Board[2][2] && Board[0][0] != ' ')
        return Board[0][0] == 'x' ? 1 : -1;
    if (Board[0][2] == Board[1][1] && Board[0][2] == Board[2][0] && Board[0][2] != ' ')
        return Board[0][2] == 'x' ? 1 : -1;;
    return 0;
}

/* Get best move 
    input: State 
    output: move (pair (i,j) represent move coordinate)
    algorithm: + Generate valid moves
               + Calculate each state score
               + Choose Best score base on Player
*/
pair<int, State> BestMove(State Board)
{
    if (CheckFinalState(Board)) {
        return make_pair(FinalStateScore(Board), Board);
    }
    char player = GetNextPlayer(Board);
    vector<State> ValidStates = GetNextStates(Board);
    int BestScore = -100;
    State BestState;
    for (State ns: ValidStates) {
        pair<int, State> Pair = BestMove(ns);
        int score = Pair.first;
        if (player == 'x') {
            if (BestScore < score || BestScore == -100) {
                BestScore = score;
                BestState = ns;
            }
        } else {
            if (BestScore > score || BestScore == -100) {
                BestScore = score;
                BestState = ns;
            }
        }
    }
    return make_pair(BestScore, BestState);
}

// return coordinate 
pair<int, int> getComputerPlay(State Board)
{
    pair<int, State> Pair = BestMove(Board);
    int score = Pair.first;
    State newState = Pair.second;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (newState[i][j] != Board[i][j]) return make_pair(i, j);
        }
    }
    return make_pair(-1, -1);
}

int main()
{
    State Board = State(3, vector<char>(3, ' '));
    char humanPlayer = ' ', currentPlayer = 'x';
    cout << "You wanna choose x or o ? : " ;
    cin >> humanPlayer;
    cout << "You choose " << humanPlayer << endl;
    cout << endl;

    renderGame(Board);
    while (true) {
        int i = -1, j = -1; 
        if (humanPlayer == currentPlayer) {
             do 
            {
            cout << "Enter Row Index (0 --> 2): ";
            cin >> i;
            cout << "Enter Col Index (0 --> 2): ";
            cin >> j;
            if ( (i < 0 || i > 2)
                || (j < 0 || j > 2) 
                || Board[i][j] != ' ' ){
                cout << "Invalid move, please retry! " << endl;
                cout << endl;
            } else {
                cout << "You play (" << i << ", " << j << ")" << endl;
                break;
            }
            } while (1);         

        } else {
            pair<int, int> move = getComputerPlay(Board);
            i = move.first;
            j = move.second;
            cout << "Computer play (" << i << ", " << j << ")" << endl;
        }
        Board = CopyGame(Board, i, j, currentPlayer);
        renderGame(Board);

        currentPlayer = currentPlayer == 'x' ? 'o' : 'x';
        if (CheckFinalState(Board)) {
            int score = FinalStateScore(Board);
            if (score > 0) {
                cout << "X won !!" << endl; 
            } else if (score < 0) {
                cout << "X lose !!" << endl; 
            } else {
                cout << "A draw !!" << endl; 
            }
            break;
        }
    }
    
    return 0;
    // Cao Xuan Nguyen'Board source code 
    // 22022670@vnu.edu.vn
    
}