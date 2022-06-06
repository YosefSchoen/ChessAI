#include "pch.h"
#include "Piece.h"

Piece::Piece()
{
	color = "None";
	type = PieceType::NoPiece;
	rank = -1;
	file = -1;
	numOfMoves = 0;
	moves = moves = std::vector<std::pair<int, int>>();
}



Piece::Piece(std::string c, int t, int r, int f, int m) {
	color = c;
	type = t;
	numOfMoves = m;
	rank = r;
	file = f;

	if(type == PieceType::Pawn) {
		int direction = 0;
		if (color == "White") { direction = 1; }
		else if (color == "Black") { direction = -1; }
		moves = std::vector<std::pair<int, int>>{ {direction * 1, 0}, {direction * 2, 0}, {direction * 1, 1}, {direction * 1, -1} };
	}

	else if (type == PieceType::Knight) {
		moves = std::vector<std::pair<int, int>>{ {1, 2}, {2, 1} };
	}

	else if (type == PieceType::Bishop) {
		moves = std::vector<std::pair<int, int>>{ 
			{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}, {8, 8},
		};
	}
	else if (type == PieceType::Rook) {
		moves = std::vector<std::pair<int, int>>{
			{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
			{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0},
		};
	}
	else if (type == PieceType::Queen) {
		moves = std::vector<std::pair<int, int>>{
			{1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {7, 7}, {8, 8},
			{0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
			{1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}
		};
	}

	else if (type == PieceType::King) {
		moves = std::vector<std::pair<int, int>>{ {0, 1}, {1, 0}, {1, 1} };
	}
}

Piece::Piece(const Piece &other) {
	color = other.color;
	type = other.type;
	rank = other.rank;
	file = other.file;
	numOfMoves = other.numOfMoves;
	moves = other.moves;
}

Piece::~Piece()
{
}


void Piece::print(char boardColor) {

	switch (type)
	{
	case PieceType::Pawn:
		std::cout << boardColor << "Pawn" << boardColor;
		break;
	case PieceType::Knight:
		std::cout << "Knight";
		break;
	case PieceType::Bishop:
		std::cout << "Bishop";
		break;
	case PieceType::Rook:
		std::cout << boardColor << "Rook" << boardColor;
		break;
	case PieceType::Queen:
		std::cout << boardColor << "Queen";
		break;
	case PieceType::King:
		std::cout << boardColor << "King" << boardColor;
		break;
	default:
		std::cout << "";
		break;
	}
}

bool Piece::moveLegal(int startRank, int startFile, int endRank, int endFile, bool enemyPiece)
{
	for (int i = 0; i < moves.size(); i++)
	{
		if (type == PieceType::Pawn) {
			int direction = 1;
			if (color == "Black") { direction = -1; }

			if (!enemyPiece && endRank - startRank == 1*direction
				&& endFile - startFile == 0) {
				return true;
			}

			else if(!enemyPiece && numOfMoves == 0 && endRank - startRank == 2*direction
				&& endFile - startFile == 0) {
				return true;
			}

			else if(enemyPiece && endRank - startRank == 1*direction
				&& (endFile - startFile == 1 || endFile - startFile == -1)) {
				return true;
			}
		}

		else {
			if ((moves[i].first == endRank - startRank || moves[i].first == startRank - endRank)
				&& (moves[i].second == endFile - startFile || moves[i].second == startFile - endFile)) {
				return true;
			}
		}
	}

	return false;
}