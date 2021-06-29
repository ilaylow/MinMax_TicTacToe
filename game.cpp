/* Following code is property of: Chuen Ley Low

The code proceeds to simulate a game of tic tac toe, allowing a player to play with a bot that
utilises the Min Max Algorithm. */

#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <cstdlib>

using namespace std;

#define GRID_SIZE 3
#define PLAYER 1
#define BOT 2
#define EMPTY ' '

#define FULL 10

#define TRUE 1
#define FALSE -1

#define WIN 1
#define LOSE -1
#define NEUTRAL 0

typedef tuple<int, int> coords;

class Board{
    private:
        int valid_marker(char type, coords pos){
            if (get<0>(pos) < 0 || get<0>(pos) > 2 || get<1>(pos) < 0 || get<1>(pos) > 2){
                return FALSE;
            }
            if (type != 'X' && type != 'O'){
                return FALSE;
            }
            if (this->board[get<0>(pos)][get<1>(pos)] != EMPTY){
                return FALSE;
            }
            return TRUE;
        }

    public:
        char **board;

        Board(){
            this->board = (char **)malloc(sizeof(char *) * GRID_SIZE);
            for (int i = 0; i < GRID_SIZE; i++){
                this->board[i] = (char *)malloc(sizeof(char) * GRID_SIZE);
                for (int j = 0; j < GRID_SIZE; j++){
                    this->board[i][j] = EMPTY;
                }
            }
        }

        Board* clone(){
            Board *newBoard = new Board();
            for (int i = 0; i < GRID_SIZE; i++){
                for (int j = 0; j < GRID_SIZE; j++){
                    newBoard->board[i][j] = this->board[i][j];
                }
            }
            return newBoard;
        }

        int place_marker(char marker, coords pos){
            if (valid_marker(marker, pos) != FALSE){
                this->board[get<0>(pos)][get<1>(pos)] = marker;
                return TRUE;
            }
            cout << "Invalid Coordinates Given. Please enter again." << endl;
            return FALSE;
        }

        int is_empty(){
            for (int i = 0 ; i < GRID_SIZE; i++){
                for (int j = 0; j < GRID_SIZE; j++){
                    if (this->board[i][j] != EMPTY){
                        return FALSE;
                    }
                }
            }
            return TRUE;
        }

        int game_finished(){
            // Check for any rows completed
            for (int i = 0; i < GRID_SIZE; i++){
                if (this->board[i][0] == this->board[i][1] && this->board[i][1] == this->board[i][2] &&
                this->board[i][0] != EMPTY){
    
                        return TRUE;
                }
            }

            // Check for any columns completed
            for (int j = 0; j < GRID_SIZE; j++){
                if (this->board[0][j] == this->board[1][j] && this->board[1][j] == this->board[2][j] &&
                this->board[0][j] != EMPTY){

                    return TRUE;
                }
            }

            // Check for Diagonals (right)
            if (this->board[0][0] == this->board[1][1] && this->board[1][1] == this->board[2][2] 
            && this->board[0][0] != EMPTY){
                return TRUE;
            }

            if (this->board[0][2] == this->board[1][1] && this->board[1][1] == this->board[2][0]
            && this->board[0][2] != EMPTY){
                return TRUE;
            }

            for (int i = 0 ; i < GRID_SIZE; i++){
                for (int j = 0 ; j < GRID_SIZE; j++){
                    if (this->board[i][j] == EMPTY){
                        return FALSE;
                    }
                }
            }

            return FULL;

        }

        void print_state(){
            for (int i = 0; i < GRID_SIZE; i++){
                
                for (int z = 0; z < GRID_SIZE; z++){
                    
                    for (int j = 0; j < GRID_SIZE; j++){

                        if (z == 1){
                            cout << " " << this->board[i][j] << " ";
                        }
                        else{
                            cout << "   ";
                        }
                        if (j != GRID_SIZE - 1){
                            cout << "|";
                        }
                    }
                    cout << endl;
                    
                }
                for (int k = 0; k < GRID_SIZE * 4 - 1; k++){
                    cout << "-";
                }
                cout << endl;
            }
        }
};

tuple<int, coords> perform_min_max(Board *board, char type);
    

int main(int argc, char *argv[]){

    Board *board = new Board();
    int turn = (rand() % 2) + 1; // Generate who's turn it is, either 1 or 2

    while (TRUE){   
        cout << "Turn: " << turn << endl;
        board->print_state();

        if (turn == BOT){
            tuple<int, coords> play = perform_min_max(board, 'X');
            cout << get<0>(get<1>(play)) << ", " << get<1>(get<1>(play))<< endl;
            cout << "Score: " << get<0>(play) << endl;
            board->place_marker('X', get<1>(play));
        }

        else if (turn == PLAYER){
            int row, col;
            coords player_marker;
            while (TRUE){
                char garbage;
                cout << "Enter coordinates in the form (row,column) to place marker: ";
                cin >> row >> garbage >> col;
                player_marker = make_tuple(row - 1, col - 1);
                if (board->place_marker('O', player_marker) == TRUE){
                    break;
                }
            }                
        }
        
        // Check the status of the board after each turn
        int status = board->game_finished();
        if (status == TRUE){
            break;
        }
        else if (status == FULL){
            turn = -1;
            break;
        }

        // Swap Turns
        turn = ((turn) % 2) + 1;
        cout << endl << endl;
    }

    board->print_state();

    if (turn == PLAYER){
        cout << "Player has won!" << endl;
    }
    else if (turn == BOT){
        cout << "Bot has won!" << endl;
    }
    else{
        cout << "Game has ended in a draw!" << endl;
    }
    
    return 0;
}

tuple<int, coords> perform_min_max(Board *board, char type){

    /* if (board->is_empty()){
        return make_tuple(0, make_tuple(0, 0));
    } */

    if (board->game_finished() == TRUE){
        if (type == 'X'){
            return make_tuple(-1, make_tuple(0, 0));
        }
        else{
            return make_tuple(1, make_tuple(0, 0));
        }
    }
    else if (board->game_finished() == FULL){
        return make_tuple(0, make_tuple(0, 0));
    }

    
    tuple<int, coords> min_max_play = (type=='X')?make_tuple(-1, make_tuple(0, 0)):make_tuple(2, make_tuple(0, 0));
    for (int i = 0 ; i < GRID_SIZE; i++){
        for (int j = 0 ; j < GRID_SIZE; j++){
            if (board->board[i][j] == EMPTY){
                Board *newBoard = board->clone();
                newBoard->board[i][j] = type;
                tuple<int, coords> play = perform_min_max(newBoard, (type=='X')?'O':'X');
                get<1>(play) = make_tuple(i, j);
                if (type == 'X'){
                    min_max_play = (get<0>(play) > (get<0>(min_max_play)))?play:min_max_play;
                }
                else if (type == 'O'){
                    min_max_play = (get<0>(play) < (get<0>(min_max_play)))?play:min_max_play;
                }
            }
        }
    }

    return min_max_play;
}