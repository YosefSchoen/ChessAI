#pragma once
#include<vector>
#include<iostream>

enum PieceType
{
	NoPiece,
	Pawn,
	Knight, 
	Bishop,
	Rook,
	Queen,
	King
};

class Piece
{
public:
	std::string color;
	int type;
	int rank;
	int file;

	//these are for the AI
	std::vector<std::pair<int, int>> moves;
	int numOfMoves = 0;

	Piece();
	Piece(std::string, int, int, int, int);
	Piece(const Piece&);
	~Piece();

	void print(char);
	bool moveLegal(int, int, int, int, bool);
};

