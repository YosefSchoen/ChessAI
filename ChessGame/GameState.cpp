#include "pch.h"
#include "GameState.h"


GameState::GameState()
{
	board = new Board();
	nextStates = std::vector<GameState*> {};
	pieces = std::vector<Piece*>{};
	value = 0;
}

GameState::GameState(const Board& other) {
	board = new Board(other);
	nextStates = std::vector<GameState*>{};
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (board->pieces[i][j]->type != PieceType::NoPiece) {
				pieces.push_back(new Piece(*board->pieces[i][j]));
			}
		}
	}
	value = 0;
}

GameState::GameState(const GameState& other) {
	board = new Board(*other.board);
	nextStates = other.nextStates;
	pieces = other.pieces;
	value = other.value;
}
 
GameState::~GameState()
{
}


void GameState::sortNextStates(bool reverse) {
	for (int i = 0; i < nextStates.size() - 1; i++)
	{
		for (int j = i + 1; j < nextStates.size(); j++)
		{
			if (reverse)
			{
				if (nextStates[i]->value <= nextStates[j]->value) {
					long temp = nextStates[i]->value;
					nextStates[i]->value = nextStates[j]->value;
					nextStates[j]->value = temp;
				}
			}
			else
			{
				if (nextStates[i]->value >= nextStates[j]->value) {
					long temp = nextStates[i]->value;
					nextStates[i]->value = nextStates[j]->value;
					nextStates[j]->value = temp;
				}
			}
		}
	}
}


bool GameState::isLeaf() { return(nextStates.size() == 0); }