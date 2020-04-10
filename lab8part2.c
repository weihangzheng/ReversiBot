#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
//#include "lab8part2lib.h"

void printBoard(char board[][26], int n);
bool positionInBounds(int n, int row, int col);
bool checkLegalInDirection(char board[][26], int n, int row, int col, char colour, int deltaRow, int deltaCol);
void flipInDirection(char board[][26], int row, int col, char colour, int deltaRow, int deltaCol);
int countInDirection(char board[][26], int row, int col, char colour, int deltaRow, int deltaCol);
int countTotal(char board[][26], int row, int col, char colour, int n);
void blackTurn(char comp, bool* valid, int legalMoves[676], int moveCount, int n, char board[][26]);
void whiteTurn(char comp, bool* valid, int legalMoves[676], int moveCount, int n, char board[][26]);
int countPieces(char board[][26], int n, char colour);
void changeValidBlackMoves(char board[][26], int n, int validMoves[676], int* blackMovesCount);
void changeValidWhiteMoves(char board[][26], int n, int validMoves[676], int* whiteMovesCount);
int countValidMoves(char board[][26], int n, char colour);
void makeBestMove(char board[][26], int n, int validMoves[676], int moveCount, char colour, bool outPut);
int makeMove(char board[][26], int n, char colour, int decidedMove, bool outPut);
void randomBot(char board[][26], int n, int validMoves[676], int moveCount, char colour);
int negamax(char simBoard[][26], int n, int validMoves[676], int moveCount, char colour, int a, int b, double timeLeft);
int countEndPieces(char simBoard[][26], int n, char colour);
int max(int a, int b);
void WeihangBot(char comp, int legalMoves[676], int moveCount, int n, char board[][26]);
void playerBot(char board [][26], int n, char colour, int legalMoves[676], int moveCount, bool *valid);
//from https://kartikkukreja.wordpress.com/2013/03/30/heuristic-function-for-reversiothello/
//with modifications
double heuristic(char grid[26][26], char comp);

int main(int argc, char **argv)
{
	//srand(time(NULL));
	char board[26][26];
	int boardDimension, blackCount, whiteCount = 0;
	char comp;
	printf("Enter the board dimension: ");
	scanf("%d", &boardDimension);
	printf("Computer plays (B/W): ");
	scanf(" %c", &comp);
	for(int i = 0; i < boardDimension; i++){
    	for(int j = 0; j < boardDimension; j++)
        	board[i][j] = 'U';
	}
	board[(boardDimension/2)-1][(boardDimension/2)-1] = 'W';
	board[(boardDimension/2)][(boardDimension/2)] = 'W';
	board[(boardDimension/2)-1][(boardDimension/2)] = 'B';
	board[(boardDimension/2)][(boardDimension/2)-1] = 'B';
	printBoard(board, boardDimension);
	int blackMovesCount = 0;
	int whiteMovesCount = 0;
	int validBlackMoves[676];
	int validWhiteMoves[676];
	bool validMove = true;
	changeValidBlackMoves(board, boardDimension, validBlackMoves, &blackMovesCount);
	while(blackMovesCount != 0 || whiteMovesCount != 0){
    	if(blackMovesCount != 0){
        	blackTurn(comp, &validMove, validBlackMoves, blackMovesCount, boardDimension, board);
        	if(!validMove){
            	printf("Invalid move.\n");
            	printf("W player wins.\n");
            	return 0;
        	}
        	printBoard(board, boardDimension);
    	}else
        	printf("B player has no valid move.\n");
    	if(countPieces(board, boardDimension, 'B') + countPieces(board, boardDimension, 'W') == boardDimension*boardDimension)
        	break;
    	changeValidWhiteMoves(board, boardDimension, validWhiteMoves, &whiteMovesCount);
    	if(whiteMovesCount != 0){
      	whiteTurn(comp, &validMove, validWhiteMoves, whiteMovesCount, boardDimension, board);
        	if(!validMove){
            	printf("Invalid move.\n");
            	printf("B player wins.\n");
            	return 0;
        	}
        	printBoard(board, boardDimension);
    	}else
        	printf("W player has no valid move.\n");
    	changeValidBlackMoves(board, boardDimension, validBlackMoves, &blackMovesCount);
    	changeValidWhiteMoves(board, boardDimension, validWhiteMoves, &whiteMovesCount);
	}
	blackCount = countPieces(board, boardDimension, 'B');
	whiteCount = countPieces(board, boardDimension, 'W');
	printf("black count: %d\n", blackCount);
	printf("white count: %d\n", whiteCount);
	if(whiteCount > blackCount){
    	printf("W player wins.\n");
	}
	else if(blackCount > whiteCount){
    	printf("B player wins.\n");
    
	}
	else
    	printf("Draw!\n");
	return 0;
}

void printBoard(char board[][26], int n){
	printf("  ");
	for (int i = 0; i < n; i++)
    	printf("%c", 97 + i);
	printf("\n");
	for(int i = 0; i < n; i++){
    	printf("%c ", 97 + i);
    	for(int j = 0; j < n; j++)
        	printf("%c", board[i][j]);
    	printf("\n");
	}
}

bool positionInBounds(int n, int row, int col){
	if(row < 0 || row >= n || col < 0 || col >= n)
    	return false;
	return true;
}

bool checkLegalInDirection(char board[][26], int n, int row, int col,
char colour, int deltaRow, int deltaCol){
	if(colour == 'W' && board[row + deltaRow][col + deltaCol] == 'B'){
    	row += deltaRow;
    	col += deltaCol;
    	while(positionInBounds(n, row, col) && board[row][col] != 'U'){
        	row += deltaRow;
        	col += deltaCol;
        	if(board[row][col] == 'W')
            	return true;
    	}
	}
	else if(colour == 'B' && board[row + deltaRow][col + deltaCol] == 'W'){
    	row += deltaRow;
    	col += deltaCol;
    	while(positionInBounds(n, row, col) && board[row][col] != 'U'){
        	row += deltaRow;
        	col += deltaCol;
        	if(board[row][col] == 'B')
            	return true;
    	}
	}
	return false;
}

void flipInDirection(char board[][26], int row, int col, char colour, int deltaRow, int deltaCol){
	row += deltaRow;
	col += deltaCol;
	while(board[row][col] != colour){
    	board[row][col] = colour;
    	row += deltaRow;
    	col += deltaCol;
	}
}

int countInDirection(char board[][26], int row, int col, char colour, int deltaRow, int deltaCol){
	int score = 0;
	row += deltaRow;
	col += deltaCol;
	while(board[row][col] != colour){
    	score++;
    	row += deltaRow;
    	col += deltaCol;
	}
	return score;
}

int countTotal(char board[][26], int row, int col, char colour, int n){
	int score = 0;
	if(checkLegalInDirection(board, n, row, col, colour, -1, -1))
    	score += countInDirection(board, row, col, colour, -1, -1);
	if(checkLegalInDirection(board, n, row, col, colour, -1, 0))
    	score += countInDirection(board, row, col, colour, -1, 0);
	if(checkLegalInDirection(board, n, row, col, colour, -1, 1))
    	score += countInDirection(board, row, col, colour, -1, 1);
	if(checkLegalInDirection(board, n, row, col, colour, 0, -1))
    	score += countInDirection(board, row, col, colour, 0, -1);
	if(checkLegalInDirection(board, n, row, col, colour, 0, 1))
    	score += countInDirection(board, row, col, colour, 0, 1);
	if(checkLegalInDirection(board, n, row, col, colour, 1, -1))
    	score += countInDirection(board, row, col, colour, 1, -1);
	if(checkLegalInDirection(board, n, row, col, colour, 1, 0))
    	score += countInDirection(board, row, col, colour, 1, 0);
	if(checkLegalInDirection(board, n, row, col, colour, 1, 1))
    	score += countInDirection(board, row, col, colour, 1, 1);
	return score;
}


void blackTurn(char comp, bool* valid, int legalMoves[676], int moveCount, int n, char board[][26]){
	if(comp == 'B')
    	WeihangBot('B', legalMoves, moveCount, n, board);
	else
playerBot(board, n, 'B', legalMoves, moveCount, valid);
//  randomBot(board, n, legalMoves, moveCount, 'B');
//makeBestMove(board, n, legalMoves, moveCount, 'B', false);
}

void whiteTurn(char comp, bool* valid, int legalMoves[676], int moveCount, int n, char board[][26]){
	if(comp == 'W')
    	WeihangBot('W', legalMoves, moveCount, n, board);
	else
 playerBot(board, n, 'W', legalMoves, moveCount, valid);
  //  randomBot(board, n, legalMoves, moveCount, 'W');
//makeBestMove(board, n, legalMoves, moveCount, 'W', false);
}

int countPieces(char board[][26], int n, char colour){
	int count = 0;
	for(int i = 0; i < n; i++){
    	for(int j = 0; j < n; j++){
        	if(board[i][j] == colour)
            	count++;
    	}
	}
	return count;
}

void changeValidBlackMoves(char board[][26], int n, int validMoves[676], int* blackMovesCount){
	*blackMovesCount = 0;
	for(int i = 0; i < n; i++){
    	for(int j = 0; j < n; j++){
        	if(board[i][j] == 'U'){
            	if(checkLegalInDirection(board, n, i, j, 'B',-1,-1)||
            	checkLegalInDirection(board, n, i, j, 'B',-1,0) ||
            	checkLegalInDirection(board, n, i, j, 'B',-1, 1) ||
            	checkLegalInDirection(board, n, i, j, 'B', 0,-1) ||
            	checkLegalInDirection(board, n, i, j, 'B', 0, 1) ||
            	checkLegalInDirection(board, n, i, j, 'B', 1,-1) ||
            	checkLegalInDirection(board, n, i, j, 'B', 1, 0) ||
            	checkLegalInDirection(board, n, i, j, 'B', 1, 1))
                	validMoves[(*blackMovesCount)++] = i * n + j;
        	}
    	}
	}
}

void changeValidWhiteMoves(char board[][26], int n, int validMoves[676], int* whiteMovesCount){
	*whiteMovesCount = 0;
	for(int i = 0; i < n; i++){
    	for(int j = 0; j < n; j++){
        	if(board[i][j] == 'U'){
            	if(checkLegalInDirection(board, n, i, j, 'W',-1,-1)||
            	checkLegalInDirection(board, n, i, j, 'W',-1,0) ||
            	checkLegalInDirection(board, n, i, j, 'W',-1, 1) ||
            	checkLegalInDirection(board, n, i, j, 'W', 0,-1) ||
            	checkLegalInDirection(board, n, i, j, 'W', 0, 1) ||
            	checkLegalInDirection(board, n, i, j, 'W', 1,-1) ||
            	checkLegalInDirection(board, n, i, j, 'W', 1, 0) ||
            	checkLegalInDirection(board, n, i, j, 'W', 1, 1))
                	validMoves[(*whiteMovesCount)++] = i * n + j;
        	}
    	}
	}
}

int countValidMoves(char board[][26], int n, char colour){
	int moveCount = 0;
	for(int i = 0; i < n; i++){
    	for(int j = 0; j < n; j++){
        	if(board[i][j] == 'U'){
            	if(checkLegalInDirection(board, n, i, j, colour,-1,-1)||
            	checkLegalInDirection(board, n, i, j, colour,-1,0) ||
            	checkLegalInDirection(board, n, i, j, colour,-1, 1) ||
            	checkLegalInDirection(board, n, i, j, colour, 0,-1) ||
            	checkLegalInDirection(board, n, i, j, colour, 0, 1) ||
            	checkLegalInDirection(board, n, i, j, colour, 1,-1) ||
            	checkLegalInDirection(board, n, i, j, colour, 1, 0) ||
            	checkLegalInDirection(board, n, i, j, colour, 1, 1))
                	moveCount++;
        	}
    	}
	}
	return moveCount;
}

void makeBestMove(char board[][26], int n, int validMoves[676], int moveCount, char colour, bool outPut){
    	int bestMoveCount = 0;
    	int bestMoveIndex = 0;
    	for(int i = 0; i < moveCount; i++){
        	int move = validMoves[i];
        	int row = move / n;
        	int col = move % n;
        	if(countTotal(board, row, col, colour, n) > bestMoveCount){
            	bestMoveCount = countTotal(board, row, col, colour, n);
            	bestMoveIndex = i;
        	}
    	}
    	makeMove(board, n, colour, validMoves[bestMoveIndex], outPut);
}

int makeMove(char board[][26], int n, char colour, int decidedMove, bool outPut){
    	int decidedRow = decidedMove / n;
    	int decidedCol = decidedMove % n;
    	if(checkLegalInDirection(board, n, decidedRow, decidedCol, colour,-1,-1))
        	flipInDirection(board, decidedRow, decidedCol, colour,-1, -1);
    	if(checkLegalInDirection(board, n, decidedRow, decidedCol, colour,-1, 0))
        	flipInDirection(board, decidedRow, decidedCol,colour,-1, 0);
    	if(checkLegalInDirection(board, n, decidedRow, decidedCol, colour,-1, 1))
        	flipInDirection(board, decidedRow, decidedCol,colour,-1, 1);
    	if(checkLegalInDirection(board, n, decidedRow, decidedCol, colour, 0,-1))
        	flipInDirection(board, decidedRow, decidedCol,colour, 0, -1);
    	if(checkLegalInDirection(board, n, decidedRow, decidedCol, colour, 0, 1))
        	flipInDirection(board, decidedRow, decidedCol,colour, 0, 1);
    	if(checkLegalInDirection(board, n, decidedRow, decidedCol, colour, 1,-1))
        	flipInDirection(board, decidedRow, decidedCol,colour, 1, -1);
    	if(checkLegalInDirection(board, n, decidedRow, decidedCol, colour, 1, 0))
        	flipInDirection(board, decidedRow, decidedCol,colour, 1, 0);
    	if(checkLegalInDirection(board, n, decidedRow, decidedCol, colour, 1, 1))
        	flipInDirection(board, decidedRow, decidedCol,colour, 1, 1);
    	board[decidedRow][decidedCol] = colour;
    	if(outPut)
         	printf("Computer places %c at %c%c.\n", colour, 97 + decidedRow, 97 + decidedCol);
    	return countTotal(board, decidedRow, decidedCol, colour, n);
}

void randomBot(char board[][26], int n, int validMoves[676], int moveCount, char colour){
	int randomIndex = rand() % moveCount;
	int decidedMove = validMoves[randomIndex];
	makeMove(board, n, colour, decidedMove, true);
}


int negamax(char simBoard[][26], int n, int validMoves[676], int moveCount, char colour, int a, int b, double timeLeft){
	//base case
	//if(countPieces(simBoard, n, 'B') + countPieces(simBoard, n, 'W') == n*n)
  	//  return 10000*countEndPieces(simBoard, n, colour);
	if(timeLeft <= 0.00001)
    	return heuristic(simBoard, colour);
	int value = -9999999;
	//save the current node
	char savedBoard[8][8];
	for(int i = 0; i < n; i++){
    	for(int j = 0; j < n; j++)
        	savedBoard[i][j] = simBoard[i][j];
    	}
	timeLeft = timeLeft/moveCount;
	if(colour == 'B'){
    	for(int i = 0; i < moveCount; i++){
        	//reset simBoard to current node
        	for(int i = 0; i < n; i++){
            	for(int j = 0; j < n; j++)
            	simBoard[i][j] = savedBoard[i][j];
        	}
        	makeMove(simBoard, n, 'B', validMoves[i], false);
        	int validWhiteMoves[676];
        	int whiteMovesCount = 0;
        	changeValidWhiteMoves(simBoard, n, validWhiteMoves, &whiteMovesCount);
       	 
        	//if white has no moves black moves again
        	if(whiteMovesCount == 0){
            	int validBlackMoves[676];
            	int blackMovesCount = 0;
            	changeValidBlackMoves(simBoard, n, validBlackMoves, &blackMovesCount);
           	 
            	//if black has no moves as well the game is over
            	if(blackMovesCount == 0){
                	return 10000*countEndPieces(simBoard, n, colour);
            	}else{
            	value = max(value, negamax(simBoard, n, validBlackMoves, blackMovesCount,'B', a, b, timeLeft));
          	 
            	}
        	}else{
        	value = max(value, -negamax(simBoard, n, validWhiteMoves, whiteMovesCount, 'W', -b, -a, timeLeft));
        	a = max(a, value);
        	if(a >= b)
            	break;
        	}
    	}
	}else{
    	for(int i = 0; i < moveCount; i++){
         	//reset simBoard to current node
        	for(int i = 0; i < n; i++){
            	for(int j = 0; j < n; j++)
        	simBoard[i][j] = savedBoard[i][j];
        	}
        	makeMove(simBoard, n, 'W', validMoves[i], false);
        	int validBlackMoves[676];
        	int blackMovesCount = 0;
        	changeValidBlackMoves(simBoard, n, validBlackMoves, &blackMovesCount);
       	 
        	//if black has no moves white moves again
        	if(blackMovesCount == 0){
            	int validWhiteMoves[676];
            	int whiteMovesCount = 0;
            	changeValidWhiteMoves(simBoard, n, validWhiteMoves, &whiteMovesCount);
           	 
            	//if black has no moves as well the game is over
            	if(whiteMovesCount == 0){
                	return 10000*countEndPieces(simBoard, n, colour);
            	}else{
            	value = max(value, negamax(simBoard, n, validWhiteMoves, whiteMovesCount, 'W', a, b, timeLeft));
      	 
            	}
        	}else{
        	value = max(value, -negamax(simBoard, n, validBlackMoves, blackMovesCount, 'B', -b, -a, timeLeft));
        	a = max(a, value);
        	if(a >= b)
            	break;
        	}
    	}
	}
	return value;
}


int countEndPieces(char simBoard[][26], int n, char colour){
	int blackPieces = countPieces(simBoard, n, 'B');
	int whitePieces = countPieces(simBoard, n,'W');
	int value = 100*(blackPieces - whitePieces)/(blackPieces + whitePieces);
	if(colour == 'B')
    	return value;
	return -value;
}

int max(int a, int b){
	if(a > b)
    	return a;
	return b;
}

void WeihangBot(char comp, int legalMoves[676], int moveCount, int n, char board[][26]){
	int highestScore = -999999999;
   	   	int index = 0;
   	   	for(int i = 0; i < moveCount; i++){
   	 int score = 0;

        	char simBoard[26][26];
        	for(int i = 0; i < n; i++){
            	for(int j = 0; j < n; j++)
                	simBoard[i][j] = board[i][j];
        	}
        	int firstMove[1];
        	firstMove[0] = legalMoves[i];
    
        	score = negamax(simBoard, n, firstMove, 1, comp, -999999, 999999, 1.0/moveCount);
	       	// printf("%c%c had a score of %d\n", (legalMoves[i]/n)+97, (legalMoves[i]%n)+97, scores[i]);
        	if(score > highestScore){
           	highestScore = score;
           	index = i;
           	 
   	}
    	}
	makeMove(board, n, comp, legalMoves[index], true);
}

void playerBot(char board [][26], int n, char colour, int legalMoves[676], int moveCount, bool *valid){
   /*
    	int row, col;
    	findSmarterMove(board, n, colour, &row, &col);
    	printf("Testing AI move (row, col): %c%c\n", row + 'a', col + 'a');
	*/
    	printf("Enter move for colour %c (RowCol): ", colour);
    	char coord1, coord2;
    	scanf(" %c%c", &coord1, &coord2);
    	int row = coord1 - 'a';
    	int col = coord2 - 'a';
   
    	int coordNum = row * n + col;
    	*valid = false;
    	for(int k = 0; k < moveCount; k++){
        	if(coordNum == legalMoves[k]){
            	*valid = true;
             	makeMove(board, n, colour, coordNum, false);
             	return;
        	}
    	}
}


//from https://kartikkukreja.wordpress.com/2013/03/30/heuristic-function-for-reversiothello/
//with modifications
double heuristic(char grid[26][26], char comp){
	char my_color, opp_color;
	if(comp == 'B'){
     	my_color = 'B';
     	opp_color = 'W';
	}
	else{
     	my_color = 'W';
     	opp_color = 'B';
	}
	int my_tiles = 0, opp_tiles = 0, i, j, k, my_front_tiles = 0, opp_front_tiles = 0, x, y;
	double p = 0, c = 0, l = 0, m = 0, f = 0, d = 0;

	int X1[] = {-1, -1, 0, 1, 1, 1, 0, -1};
	int Y1[] = {0, 1, 1, 1, 0, -1, -1, -1};

	int V[][8] = {{20, -3, 11, 8, 8, 11, -3, 20},
    	{-3, -7, -4, 1, 1, -4, -7, -3},
    	{11, -4, 2, 2, 2, 2, -4, 11},
    	{8, 1, 2, -3, -3, 2, 1, 8},
     	{8, 1, 2, -3, -3, 2, 1, 8},
    	{11, -4, 2, 2, 2, 2, -4, 11},
	{-3, -7, -4, 1, 1, -4, -7, -3},
	{20, -3, 11, 8, 8, 11, -3, 20}};


	for(i=0; i<8; i++)
    	for(j=0; j<8; j++)  {
        	if(grid[i][j] == my_color)  {
            	d += V[i][j];
            	my_tiles++;
        	} else if(grid[i][j] == opp_color)  {
            	d -= V[i][j];
            	opp_tiles++;
        	}
        	if(grid[i][j] != 'U')   {
            	for(k=0; k<8; k++)  {
                	x = i + X1[k]; y = j + Y1[k];
                	if(x >= 0 && x < 8 && y >= 0 && y < 8 && grid[x][y] == 'U') {
                    	if(grid[i][j] == my_color)  my_front_tiles++;
                    	else opp_front_tiles++;
                    	break;
                	}
            	}
        	}
    	}
	if(my_tiles > opp_tiles)
    	p = (100.0 * my_tiles)/(my_tiles + opp_tiles);
	else if(my_tiles < opp_tiles)
    	p = -(100.0 * opp_tiles)/(my_tiles + opp_tiles);
	else p = 0;

	if(my_front_tiles > opp_front_tiles)
    	f = -(100.0 * my_front_tiles)/(my_front_tiles + opp_front_tiles);
	else if(my_front_tiles < opp_front_tiles)
    	f = (100.0 * opp_front_tiles)/(my_front_tiles + opp_front_tiles);
	else f = 0;

	my_tiles = opp_tiles = 0;
	if(grid[0][0] == my_color) my_tiles++;
	else if(grid[0][0] == opp_color) opp_tiles++;
	if(grid[0][7] == my_color) my_tiles++;
	else if(grid[0][7] == opp_color) opp_tiles++;
	if(grid[7][0] == my_color) my_tiles++;
	else if(grid[7][0] == opp_color) opp_tiles++;
	if(grid[7][7] == my_color) my_tiles++;
	else if(grid[7][7] == opp_color) opp_tiles++;
	c = 25 * (my_tiles - opp_tiles);

	my_tiles = opp_tiles = 0;
	if(grid[0][0] == 'U')   {
    	if(grid[0][1] == my_color) my_tiles++;
    	else if(grid[0][1] == opp_color) opp_tiles++;
    	if(grid[1][1] == my_color) my_tiles++;
    	else if(grid[1][1] == opp_color) opp_tiles++;
    	if(grid[1][0] == my_color) my_tiles++;
    	else if(grid[1][0] == opp_color) opp_tiles++;
	}
	if(grid[0][7] == 'U')   {
    	if(grid[0][6] == my_color) my_tiles++;
    	else if(grid[0][6] == opp_color) opp_tiles++;
    	if(grid[1][6] == my_color) my_tiles++;
    	else if(grid[1][6] == opp_color) opp_tiles++;
    	if(grid[1][7] == my_color) my_tiles++;
    	else if(grid[1][7] == opp_color) opp_tiles++;
	}
	if(grid[7][0] == 'U')   {
    	if(grid[7][1] == my_color) my_tiles++;
    	else if(grid[7][1] == opp_color) opp_tiles++;
    	if(grid[6][1] == my_color) my_tiles++;
    	else if(grid[6][1] == opp_color) opp_tiles++;
    	if(grid[6][0] == my_color) my_tiles++;
    	else if(grid[6][0] == opp_color) opp_tiles++;
	}
	if(grid[7][7] == 'U')   {
    	if(grid[6][7] == my_color) my_tiles++;
    	else if(grid[6][7] == opp_color) opp_tiles++;
    	if(grid[6][6] == my_color) my_tiles++;
    	else if(grid[6][6] == opp_color) opp_tiles++;
    	if(grid[7][6] == my_color) my_tiles++;
    	else if(grid[7][6] == opp_color) opp_tiles++;
	}
	l = -12.5 * (my_tiles - opp_tiles);

    
	if(comp == 'B'){
    	my_tiles = countValidMoves(grid, 8, 'B');
    	opp_tiles = countValidMoves(grid, 8, 'W');
	}else{
    	my_tiles = countValidMoves(grid, 8, 'W');
    	opp_tiles = countValidMoves(grid, 8, 'B');
	}
	if(my_tiles > opp_tiles)
    	m = (100.0 * my_tiles)/(my_tiles + opp_tiles);
	else if(my_tiles < opp_tiles)
    	m = -(100.0 * opp_tiles)/(my_tiles + opp_tiles);
	else m = 0;

	return (10 * p) + (801.724 * c) + (382.026 * l) + (78.922 * m) + (74.396 * f) + (10 * d);

}



































