#include "../lib/helpFunction.h"
extern sharedMemory *sm;
extern move pos[60];

int searchPly;

// Checks a direction from x,y to see if we can make a move
bool checkFlip(char board[][8], int x, int y, int deltaX, int deltaY, 
	           char myPiece, char opponentPiece){
	
	// has to be done like this because otherwise line 11 wouldnt work because it could happen that x < 0 or ...
	if(!((x >= 0) && (x < 8) && (y >= 0) && (y < 8))) {
		return false;
	}
	if (board[x][y] == opponentPiece)
	{
		while ((x >= 0) && (x < 8) && (y >= 0) && (y < 8))
		{
			x += deltaX;
			y += deltaY;
			// BUG in original code, allows -1 or 8. For example
			// if x = 0 and deltaX is -1, then now x = -1
			// I patched this with another if statement, but it might be better
			// to move the += to the bottom of the loop
			if ((x >= 0) && (x < 8) && (y >= 0) && (y < 8))
			{
				if (board[x][y] == '*')	// not consecutive
					return false;
				if (board[x][y] == myPiece)
					return true;		// At least one piece we can flip
				else
				{
					// It is an opponent piece, just keep scanning in our direction
				}
			}
		}
	}
	return false; // Either no consecutive opponent pieces or hit the edge of the board
}
// Flips pieces in the given direction until we don't hit any more opponent pieces.
// Assumes this is a valid direction to flip (we eventually hit one of our pieces).
void flipPieces(char board[][8], int x, int y, int deltaX, int deltaY, char myPiece, char opponentPiece)
{
	while (board[x][y] == opponentPiece)
	{
		board[x][y] = myPiece;
		x += deltaX;
		y += deltaY;
	}
}
// Makes a move on the board, assumes the move is valid.
void makeMove(char board[][8], int x, int y, char piece)
{
	// Put the piece at x,y
	board[x][y] = piece;

	// Figure out the character of the opponent's piece
	char opponent = 'W';
	if (piece == 'W')
		opponent = 'B';

	// Check to the left
	if (checkFlip(board, x - 1, y, -1, 0, piece, opponent))
		flipPieces(board, x - 1, y, -1, 0, piece, opponent);
	// Check to the right
	if (checkFlip(board, x + 1, y, 1, 0, piece, opponent))
		flipPieces(board, x + 1, y, 1, 0, piece, opponent);
	// Check down
	if (checkFlip(board, x, y-1, 0, -1, piece, opponent))
		flipPieces(board, x, y-1, 0, -1, piece, opponent);
	// Check up
	if (checkFlip(board, x, y + 1, 0, 1, piece, opponent))
		flipPieces(board, x, y + 1, 0, 1, piece, opponent);
	// Check down-left	
	if (checkFlip(board, x-1, y - 1, -1, -1, piece, opponent))
		flipPieces(board, x-1, y - 1, -1, -1, piece, opponent);
	// Check down-right
	if (checkFlip(board, x + 1, y - 1, 1, -1, piece, opponent))
		flipPieces(board, x + 1, y - 1, 1, -1, piece, opponent);
	// Check up-left	
	if (checkFlip(board, x - 1, y + 1, -1, 1, piece, opponent))
		flipPieces(board, x - 1, y + 1, -1, 1, piece, opponent);
	// Check up-right
	if (checkFlip(board, x + 1, y + 1, 1, 1, piece, opponent))
		flipPieces(board, x + 1, y + 1, 1, 1, piece, opponent);
}


// Returns true if the move is valid, false if invalid
bool validMove(char board[][8], int x, int y, char piece)
{
	// Check that the coordinates are empty
	if (board[x][y] != '*')
		return false;

	// Figure out the character of the opponent's piece
	char opponent = 'W';
	if (piece == 'W')
		opponent = 'B';

	// If we can flip in any direction, it is valid
	// Check to the left
	if (checkFlip(board, x - 1, y, -1, 0, piece, opponent))
		return true;
	// Check to the right
	if (checkFlip(board, x + 1, y, 1, 0, piece, opponent))
		return true;
	// Check down
	if (checkFlip(board, x, y - 1, 0, -1, piece, opponent))
		return true;
	// Check up
	if (checkFlip(board, x, y + 1, 0, 1, piece, opponent))
		return true;
	// Check down-left	
	if (checkFlip(board, x - 1, y - 1, -1, -1, piece, opponent))
		return true;
	// Check down-right
	if (checkFlip(board, x + 1, y - 1, 1, -1, piece, opponent))
		return true;
	// Check up-left	
	if (checkFlip(board, x - 1, y + 1, -1, 1, piece, opponent))
		return true;
	// Check up-right
	if (checkFlip(board, x + 1, y + 1, 1, 1, piece, opponent))
		return true;

	return false; // If we get here, we didn't find a valid flip direction
}
// Returns the score for the piece
int score(char board[][8], char piece){
    int total = 0;
    for (int x = 0; x < 8; x++){
        for (int y = 0; y < 8; y++){
			if((x==0 && y==0) || (x==0 && y==7) ||
			   (x==7 && y==0) || (x==7 && y==7) ) {
				   if (board[x][y] == piece){
                		total += 120;
			   		}	
			}
			if((x==2 && y==2) || (x==2 && y==5) ||
			   (x==5 && y==2) || (x==5 && y==5) ) {
				   if (board[x][y] == piece){
                		total += 15;
				   }
			}
			if((x==1 && y==1) || (x==1 && y==6) ||
			   (x==6 && y==1) || (x==6 && y==6) ) {
				   if (board[x][y] == piece){
                		total -= 40;
				   }
			}
			if((x==0 && y==1) || (x==1 && y==0) ||
			   (x==7 && y==1) || (x==1 && y==7) ||
			   (x==0 && y==6) || (x==6 && y==0) ||
			   (x==6 && y==7) || (x==7 && y==6)) {
				   if (board[x][y] == piece){
                		total -= 20;
				   }
			}
			if((x==0 && y==2) || (x==2 && y==0) ||
			   (x==0 && y==5) || (x==5 && y==0) ||
			   (x==2 && y==7) || (x==7 && y==2) ||
			   (x==7 && y==5) || (x==5 && y==7)) {
				   if (board[x][y] == piece){
                		total += 20;
				   }
			}
			if((x==0 && y==3) || (x==3 && y==0) ||
			   (x==0 && y==4) || (x==4 && y==0) ||
			   (x==7 && y==3) || (x==3 && y==7) ||
			   (x==4 && y==7) || (x==7 && y==4)){
				   if (board[x][y] == piece){
                		total += 5;
				   }
			}
			if((x==1 && y==2) || (x==1 && y==3) || (x==1 && y==4) || (x==1 && y==5) ||
			   (x==6 && y==2) || (x==6 && y==3) || (x==6 && y==4) || (x==6 && y==5)	||
			   (x==2 && y==1) || (x==3 && y==1) || (x==4 && y==1) || (x==5 && y==1)	||
			   (x==2 && y==6) || (x==3 && y==6) || (x==4 && y==6) || (x==5 && y==6)	) {
				if (board[x][y] == piece){
                		total -= 5;
				   }
			}
			if((x==2 && y==3) || (x==2 && y==4) || (x==5 && y==3) || (x==5 && y==4) ||
			   (x==3 && y==2) || (x==3 && y==3) || (x==3 && y==4) || (x==3 && y==5)	||
			   (x==4 && y==2) || (x==4 && y==3) || (x==4 && y==4) || (x==4 && y==5) ) {
				if (board[x][y] == piece){
                		total += 3;
				   }
			}												
        }
    }    
    return total;
}
// Fills in the arrays with valid moves for the piece.  numMoves is the number of valid moves.
int getMoveList(char board[][8], move *moveList, char piece)
{
	int numMoves = 0;  // Initially no moves found

	// Check each square of the board and if we can move there, remember the coordinates
	for (int x = 0; x < 8; x++){
		for (int y = 0; y < 8; y++)
		{
			if (validMove(board, x, y, piece)) // If find valid move, remember coordinates
			{
				moveList[numMoves].x = x;
				moveList[numMoves].y = y;
				numMoves++;		// Increment number of moves found
                    
			}
		}
    }
    return numMoves;        
}
// This is an efficient way to copy one array to another, rather than a nested loop,
// but it is potentially dangerous if sending in the wrong items
void copyBoard(char src[][8], char dest[][8])
{
	memcpy(dest, src, 8 * 8 * sizeof(char));  // C style for copying 64 bytes from src to dest
}
// The simple heuristic is simply the number of our pieces - the number of opponent pieces.
// Weighting the edges and corners will result in a better player.
int heuristic(char board[][8], char whoseTurn)
{
	char opponent = 'W';
	if (whoseTurn == 'W')
		opponent = 'B';
	int ourScore = score(board, whoseTurn);
	int opponentScore = score(board, opponent);
	return (ourScore - opponentScore);
}
// This is the minimax decision function. It calls minimaxValue for each position
// on the board and returns the best move (largest value returned) in x and y.
threadArguments *minimaxDecision(threadArguments *args)
{   
	char opponent = {0};

	if(args->whoseTurn == 'W') opponent = 'B';
	else opponent = 'W';


	// Try out every single move
	char tempBoard[8][8] = {0};
	copyBoard(args->feld, tempBoard);

	move moveList[60];
	getMoveList(args->feld, moveList, args->whoseTurn);
	// Apply the move to a new board
	int x = moveList[args->i].x;
	int y = moveList[args->i].y;
	makeMove(tempBoard, x, y, args->whoseTurn);
	// Recursive call
	pos[args->i].val = minimaxValue(args, tempBoard, opponent, 1);
	return args;
}


// minimaxValue makes a recursive call to itself to search another ply in the tree.
// It is hard-coded here to look 5 ply ahead.  originalTurn is the original player piece
// which is needed to determine if this is a MIN or a MAX move.  It is also needed to 
// calculate the heuristic. currentTurn flips between W and B.
int minimaxValue(threadArguments *args, char tempBoard[8][8], char currentTurn, int searchPly)
{
	
	signal(SIGALRM, signalAlarm);

	//char originalTurn = args->whoseTurn;
	char opponent = 'W';
	if (currentTurn == 'W')
		opponent = 'B';

	int numMoves = 0;
	move moveList[60];
	numMoves = getMoveList(tempBoard, moveList, currentTurn);
	if (searchPly == MAX) // Change to desired ply lookahead
	{
		return heuristic(tempBoard, currentTurn); // Termination criteria
	}

	bool isFull = true;
	for(int i = 0; i < 8; i++) {
		for(int j = 0; j < 9; j++) {
			if(tempBoard[i][j] == '*') {
				isFull = false;
				break;
			}
		}
	}
	if (numMoves == 0 && !isFull) // if no moves skip to next player's turn
	{
		return minimaxValue(args, tempBoard, opponent, searchPly + 1);
	} else if(numMoves == 0 && isFull) {
		return heuristic(tempBoard, currentTurn);
	}
	else
	{

		
		for(int i = 0; i < numMoves; i++) {
			// Apply the move to a new board
			char tempBoardNew[8][8] = {0};
			copyBoard(tempBoard, tempBoardNew);
			//int x = moveList[args->i].x;
			//int y = moveList[args->i].y;
			int x = moveList[i].x;
			int y = moveList[i].y;
			makeMove(tempBoardNew, x, y, currentTurn);
			// Recursive call
			int val = minimaxValue(args, tempBoardNew, opponent, searchPly + 1);
			// Remember best move
			return val;
		}
	}
	return -1;  // Should never get here
}
