// ChessGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "Board.h"
#include "ChessAI.h"
#include "GameState.h"

void twoAIPlayers();
void oneAIPlayer();
void twoHumanPlayers();

int main()
{
	int gameType;

	std::cout << "Press 0 to watch AI play itself\nPress 1 to play against the AI\nPress 2 to play another person\n";
	std::cin >> gameType;

	if (gameType == 0) { twoAIPlayers(); }
	else if (gameType == 1) { oneAIPlayer(); }
	else if (gameType == 2) { twoHumanPlayers(); }
}


void twoAIPlayers() {
	Board board = Board();
	board.print();

	ChessAI ai = ChessAI(GameState(board), 0);


	while (!board.checkmate) {
		ai.gameState = new GameState(board);
		board = *ai.makeMove(true);
		board.print();

		ai.gameState = new GameState(board);
		board = *ai.makeMove(false);
		board.print();
	}
}

void oneAIPlayer() {
	int startRank;
	char startFile;
	int endRank;
	char endFile;

	Board board = Board();
	board.print();

	ChessAI ai = ChessAI(GameState(board), 0);

	while (!board.checkmate) {
		
		bool illegalMove = true;
		while (illegalMove) {
			std::cout << "select a piece you wish to moves by its coordinates\n";
			std::cin >> startFile >> startRank;
			std::cout << "enter the coordinates of the pieces destination\n";
			std::cin >> endFile >> endRank;
			if (board.moveLegal(startRank - 1, startFile - 'a', endRank - 1, endFile - 'a')) {
				illegalMove = false;
				board.movePiece(startRank - 1, startFile - 'a', endRank - 1, endFile - 'a');
				board.print();
			}

			else {
				std::cout << "illegal move\n";
				illegalMove = true;
			}
		}

		illegalMove = true;
		ai.gameState = new GameState(board);
		board = *ai.makeMove(false);
		board.print();
	}
}

void twoHumanPlayers() {
	int startRank;
	char startFile;
	int endRank;
	char endFile;

	Board board = Board();
	board.print();

	while (!board.checkmate) {

		bool illegalMove = true;
		while (illegalMove) {
			std::cout << "select a piece you wish to moves by its coordinates\n";
			std::cin >> startFile >> startRank;
			std::cout << "enter the coordinates of the pieces destination\n";
			std::cin >> endFile >> endRank;
			if (board.moveLegal(startRank - 1, startFile - 'a', endRank - 1, endFile - 'a')) {
				illegalMove = false;
				board.movePiece(startRank - 1, startFile - 'a', endRank - 1, endFile - 'a');
				board.print();
			}

			else {
				std::cout << "illegal move\n";
				illegalMove = true;
			}
		}


		illegalMove = true;
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
