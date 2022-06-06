#pragma once
#include <vector>
#include "GameState.h"
#include "Board.h"
#include <algorithm>

class ChessAI
{
public:
	GameState* gameState;
	int moveNumber = 0;

	ChessAI();
	ChessAI(const GameState &, int m);
	ChessAI(const ChessAI &);
	~ChessAI();

	std::vector<GameState*> getNextPositions(const GameState&, bool);
	GameState* buildTree(GameState*, int, bool);
	long evaluateState(const GameState &);
	long evaluatePieceValues(const Piece &);
	long evaluateDevelopment(const Piece &);
	long evaluateSquaresControlled(const Piece &, const GameState &);

	long evaluateThreats(const GameState &);
	long evaluatePawnStructure(const GameState &);



	long miniMax(GameState &, int, long, long, bool);

	Board* makeMove(bool);
};

