#include "pch.h"
#include "Board.h"


Board::Board()
{
	Piece* piece;

	piece = new Piece("White", PieceType::Rook, 0, 0, 0);
	pieces[0][0] = piece;

	piece = new Piece("White", PieceType::Rook, 0, 7, 0);
	pieces[0][7] = piece;
							
	piece = new Piece("White", PieceType::Knight, 0, 1, 0);
	pieces[0][1] = piece;

	piece = new Piece("White", PieceType::Knight, 0, 6, 0);
	pieces[0][6] = piece;
							
	piece = new Piece("White", PieceType::Bishop, 0, 2, 0);
	pieces[0][2] = piece;

	piece = new Piece("White", PieceType::Bishop, 0, 5, 0);
	pieces[0][5] = piece;
							
	piece = new Piece("White", PieceType::Queen, 0, 3, 0);
	pieces[0][3] = piece;
				
	piece = new Piece("White", PieceType::King, 0, 4, 0);
	pieces[0][4] = piece;
	whiteKing = new Piece(*piece);

	for (int i = 0; i < 8; i++)
	{
		piece = new Piece("White", PieceType::Pawn, 1, i, 0);
		pieces[1][i] = piece;
	}
	
	for (int i = 0; i < 8; i++)
	{
		piece = new Piece("Black", PieceType::Pawn, 6, i, 0);
		pieces[6][i] = piece;
	}
			
	piece = new Piece("Black", PieceType::Rook, 7, 0, 0);
	pieces[7][0] = piece;

	piece = new Piece("Black", PieceType::Rook, 7, 7, 0);
	pieces[7][7] = piece;
				
	piece = new Piece("Black", PieceType::Knight, 7, 1, 0);
	pieces[7][1] = piece;

	piece = new Piece("Black", PieceType::Knight, 7, 6, 0);
	pieces[7][6] = piece;
				
	piece = new Piece("Black", PieceType::Bishop, 7, 2, 0);
	pieces[7][2] = piece;

	piece = new Piece("Black", PieceType::Bishop, 7, 5, 0);
	pieces[7][5] = piece;
				
	piece = new Piece("Black", PieceType::Queen, 7, 3, 0);
	pieces[7][3] = piece;
				
	piece = new Piece("Black", PieceType::King, 7, 4, 0);
	pieces[7][4] = piece;
	blackKing = new Piece(*piece);

	for (int i = 2; i < 6; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			pieces[i][j] = new Piece();
		}
	}
}

Board::Board(const Board &other) {
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			pieces[i][j] = new Piece(*other.pieces[i][j]);
		}
	}

	whiteKing = new Piece(*other.whiteKing);
	blackKing = new Piece(*other.blackKing);
}

Board::~Board()
{
}


void Board::printRow(int rank, int line)
{
	int blackSquare = 0xFF;
	int whiteSquare = 0xDB;
	int boardColor;
	for (int i = 0; i < 8; i++)
	{
		if ((i + rank) % 2 == 1) { boardColor = whiteSquare; }
		else { boardColor = blackSquare; }

		if (i == 0 && line == 2) std::cout << rank+1 << " ";
		else if(i == 0 && line != 2) { std::cout << "  "; }
		for (int j = 0; j < 10; j++)
		{

			if (j == 2 && pieces[rank][i]->type != PieceType::NoPiece && line == 1) { std::cout << pieces[rank][i]->color; j += 5; }
			if (j == 2 && pieces[rank][i]->type != PieceType::NoPiece && line == 2) { pieces[rank][i]->print(char(boardColor)); j += 5; }
			else { std::cout << char(boardColor); }
		}
	}

	std::cout << "\n";
}

void Board::print() {
	std::cout << "\n\n";
	for (int i = 7; i >= 0; i--)
	{
		for (int j = 0; j < 5; j++)
		{
			printRow(i, j);
		}
	}

	std::cout << "  ";
	for (int i = 'a'; i <= 'h'; i++)
	{
		std::cout << "    "<< char(i) << "     ";
	}
	std::cout << "\n\n";
}

void Board::movePiece(int startRank, int startFile , int endRank, int endFile) {
	Piece* startPiece = new Piece(*pieces[startRank][startFile]);
	pieces[startRank][startFile] = new Piece();

	pieces[endRank][endFile] = new Piece(*startPiece);
	pieces[endRank][endFile]->numOfMoves += 1;
	pieces[endRank][endFile]->rank = endRank;
	pieces[endRank][endFile]->file = endFile;

	if (pieces[endRank][endFile]->type == PieceType::King) {
		if (pieces[endRank][endFile]->color == "White") { whiteKing = new Piece(*pieces[endRank][endFile]); }
		else if (pieces[endRank][endFile]->color == "Black") { blackKing = new Piece(*pieces[endRank][endFile]); }
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (pieces[i][j]->type != PieceType::NoPiece){
				if (pieces[endRank][endFile]->color == "White" && pieces[i][j]->color == "White") {
					if(pieces[i][j]->moveLegal(i, j, blackKing->rank, blackKing->file, true)
						&& !pieceInPath(i, j, blackKing->rank, blackKing->file)) {
						blackCheck = true;
					}
				}

				else if (pieces[endRank][endFile]->color == "Black" && pieces[i][j]->color == "Black") {
					if (pieces[i][j]->moveLegal(i, j, whiteKing->rank, whiteKing->file, true)
						&& !pieceInPath(i, j, whiteKing->rank, whiteKing->file)) {
						whiteCheck = true;
					}
				}
			}
		}
	}
}

bool Board::moveLegal(int startRank, int startFile, int endRank, int endFile) {
	
	//these conditions are related to chess as a whole
	if (pieces[startRank][startFile]->type == PieceType::NoPiece) { return false; }
	if (outOfBounds(startRank, startFile, endRank, endFile)) { return false; }
	if (pieces[startRank][startFile]->type != PieceType::NoPiece && pieces[endRank][endFile] ->type != PieceType::NoPiece
		&& pieces[startRank][startFile]->color == pieces[endRank][endFile]->color) { 
		return false;
	}
	if (startRank == endRank && startFile == endFile) { return false; }

	bool enemyPiece = pieces[endRank][endFile]->type != PieceType::NoPiece 
		&& pieces[startRank][startFile]->color != pieces[endRank][endFile]->color;

	if ((whiteCheck && pieces[startRank][startFile]->color == "White")
		|| (blackCheck && pieces[startRank][startFile]->color == "Black")) {
		return gettingOutOfCheck(startRank, startFile, endRank, endFile, enemyPiece);
	}

	if (creatingCheck(startRank, startFile, enemyPiece)) { return false; }
	if (pieceInPath(startRank, startFile, endRank, endFile)) { return false; }

	
	return pieces[startRank][startFile]->moveLegal(startRank, startFile, endRank, endFile, enemyPiece);
}


//cant move off the board
bool Board::outOfBounds(int startRank, int startFile, int endRank, int endFile) {
	auto bounds = [](int n)->bool {return n < 0 || n > 7; };
	return bounds(startRank) || bounds(startFile) || bounds(endRank) || bounds(endFile);
}

//can't go through a piece unless its a knight
bool Board::pieceInPath(int startRank, int startFile, int endRank, int endFile) {
	auto changeInDirection = [](int n1, int n2) -> int {return n2 - n1; };
	auto getDirection = [](int n) -> int {if (n) { return abs(n) / n; } return 0; };

	int rankMagnitude = changeInDirection(startRank, endRank);
	int rankDirection = getDirection(rankMagnitude);
	int fileMagnitude = changeInDirection(startFile, endFile);
	int fileDirection = getDirection(fileMagnitude);

	
	if (pieces[startRank][startFile]->type == PieceType::Knight) { return false; }
	
	//no piece moves like this except knights but they were checked above
	if(changeInDirection(startRank, endRank) != 0 && changeInDirection(startFile, endFile) != 0 
		&& abs(changeInDirection(startRank, endRank)) != abs(changeInDirection(startFile, endFile))) {
		return true;
	}

	for (int i = 1; i < std::max(abs(rankMagnitude), abs(fileMagnitude)); i++) {	
		if (pieces[startRank + i * rankDirection][startFile + i * fileDirection]->type != PieceType::NoPiece) {
			return true;
		}
	}

	return false;
}

//can't move if a check on your king is created
bool Board::creatingCheck(int startRank, int startFile, bool enemyPiece) {

	//checking if the enemy pieces can attack the king
	int tempType = pieces[startRank][startFile]->type;
	pieces[startRank][startFile]->type = PieceType::NoPiece;

	if (inCheck(startRank, startFile)) {
		pieces[startRank][startFile]->type = tempType;
		return true;
	}

	pieces[startRank][startFile]->type = tempType;
	return false;
}

//getting out of check doesn't work if the move to get out of check creates a new check
bool Board::gettingOutOfCheck(int startRank, int startFile, int endRank, int endFile, bool enemyPiece) {
	int tempTypeStart = pieces[startRank][startFile]->type;
	std::string tempColorStart = pieces[startRank][startFile]->color;

	int tempTypeEnd = pieces[endRank][endFile]->type;
	std::string tempColorEnd = pieces[endRank][endFile]->color;

	if (!pieceInPath(startRank, startFile, endRank, endFile)
		&& pieces[startRank][startFile]->moveLegal(startRank, startFile, endRank, endFile, enemyPiece)) {
		pieces[startRank][startFile]->type = PieceType::NoPiece;
		pieces[endRank][endFile]->color = tempColorStart;
		pieces[endRank][endFile]->type = tempTypeStart;

		if (inCheck(endRank, endFile)) {
			pieces[startRank][startFile]->type = tempTypeStart;
			pieces[startRank][startFile]->color = tempColorStart;
			pieces[endRank][endFile]->type = tempTypeEnd;
			pieces[endRank][endFile]->color = tempColorEnd;
			checkmate = true;
			return false;
		}

	}

	if (pieces[endRank][endFile]->color == "White") { whiteCheck = false; }
	else if(pieces[endRank][endFile]->color == "Black") { blackCheck = false; }
	pieces[startRank][startFile]->type = tempTypeStart;
	pieces[startRank][startFile]->color = tempColorStart;
	pieces[endRank][endFile]->type = tempTypeEnd;
	pieces[endRank][endFile]->color = tempColorEnd;

	return true;
}

bool Board::inCheck(int startRank, int startFile) {
	int kingRank;
	int kingFile;
	if (pieces[startRank][startFile]->color == "White") {
		kingRank = whiteKing->rank;
		kingFile = whiteKing->file;
	}

	else if (pieces[startRank][startFile]->color == "Black") {
		kingRank = blackKing->rank;
		kingFile = blackKing->file;
	}

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++)
		{
			if (pieces[i][j]->type != PieceType::NoPiece && pieces[i][j]->color != pieces[startRank][startFile]->color) {
				if (pieces[i][j]->moveLegal(i, j, kingRank, kingFile, true)
					&& !pieceInPath(i, j, kingRank, kingFile)) {
					return true;
				}
			}
		}
	}

	return false;
}

//not yet implemented
bool Board::canCastle(int startRank, int startFile, int endRank, int endFile) {
	return false;
}