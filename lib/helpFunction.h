#ifndef struct_H
#define struct_H
#include "../lib/struct_H.h"
#include "../lib/signalHandler.h"
#endif


bool checkFlip(char board[][8], int x, int y, int deltaX, int deltaY, char myPiece, char opponentPiece);

void flipPieces(char board[][8], int x, int y, int deltaX, int deltaY, char myPiece, char opponentPiece);

void makeMove(char board[][8], int x, int y, char piece);

bool validMove(char board[][8], int x, int y, char piece);

int score(char board[][8], char piece);

int getMoveList(char board[][8], move *moveList, char piece);

void copyBoard(char src[][8], char dest[][8]);

int heuristic(char board[][8], char whoseTurn);

threadArguments *minimaxDecision(threadArguments *args);

int minimaxValue(threadArguments *args, char tempBoard[8][8], char currentTurn, int searchPly);
