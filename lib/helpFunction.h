#ifndef struct_H
#define struct_H
#include "../lib/struct_H.h"
#endif
bool checkFlip(char board[][8], int x, int y, int deltaX, int deltaY, 
	           char myPiece, char opponentPiece);
void flipPieces(char board[][8], int x, int y, int deltaX, int deltaY, 
                char myPiece, char opponentPiece);
void makeMove(char board[][8], int x, int y, char piece);

bool validMove(char board[][8], int x, int y, char piece);

int score(char board[][8], char piece);

int getMoveList(char board[][8], int moveX[], int moveY[], int numMoves, 
                char piece);
void copyBoard(char src[][8], char dest[][8]);

int heuristic(char board[][8], char whoseTurn);

void minimaxDecision(char board[][8], char whoseTurn, int* x, int* y);

int minimaxValue(char board[][8], char originalTurn, char currentTurn, 
                int searchPly);
