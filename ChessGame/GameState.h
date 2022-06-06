#pragma once
#include <vector>
#include "Board.h"

class GameState
{
public:
	Board* board;
	long value;
	std::vector<GameState *> nextStates;
	std::vector<Piece*>pieces;

	GameState();
	 GameState(const Board &);
	GameState(const GameState &);

	~GameState();

	void sortNextStates(bool);
	 
	bool isLeaf();
};

