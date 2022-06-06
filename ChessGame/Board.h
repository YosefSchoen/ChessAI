#pragma once
#include<vector>
#include "Piece.h"
#include<iostream>
#include<string>
#include <algorithm>

class Board
{
public:
	Piece* pieces[8][8];
	bool checkmate = false;
	bool whiteCheck = false;
	bool blackCheck = false;
	bool enPassante = false;
	Piece* whiteKing; 
	Piece* blackKing;

	Board();
	Board(const Board &);
	~Board();

	void print();
	void printRow(int, int);
	void movePiece(int, int, int, int);

	bool moveLegal(int, int, int, int);
	bool outOfBounds(int, int, int, int);
	bool pieceInPath(int, int, int, int);
	bool creatingCheck(int, int, bool);
	bool gettingOutOfCheck(int, int, int, int, bool);
	bool inCheck(int, int);
	bool canCastle(int, int, int, int);
};

