#include "pch.h"
#include "ChessAI.h"

const int Depth = 3;
const int Breadth = 20;
const int MinAlpha = -2147483647 - 1;
const int MaxBeta = 2147483647;

ChessAI::ChessAI() { 
	gameState = new GameState();
	moveNumber = 0;
}


ChessAI::ChessAI(const GameState& gs, int m = 0) {
	gameState = new GameState(gs);
	moveNumber = 0;
}

ChessAI::ChessAI(const ChessAI& other) {
	gameState = new GameState(*other.gameState);
	moveNumber = other.moveNumber;
}

ChessAI::~ChessAI()
{
}

std::vector<GameState*> ChessAI::getNextPositions(const GameState& g, bool maximizingPlayer) {
	GameState gCopy = GameState(g);
	
	for (int i = 0; i < g.pieces.size(); i++) {
		int startRank = g.pieces[i]->rank;
		int startFile = g.pieces[i]->file;
		
		if (maximizingPlayer && g.pieces[i]->color == "Black") { continue; }
		else if(!maximizingPlayer && g.pieces[i]->color == "White") { continue; }

		
		std::vector<std::pair<int, int>> potentialMoves = {};
		
		for (int j = 0; j < g.pieces[i]->moves.size(); j++) {
			int rankDifference = g.pieces[i]->moves[j].first;
			int fileDifference = g.pieces[i]->moves[j].second;
			int endRank1 = startRank + rankDifference;
			int endRank2 = startRank - rankDifference;
			int endFile1 = startFile + fileDifference;
			int endFile2 = startFile - fileDifference;

			if (g.board->moveLegal(startRank, startFile, endRank1, endFile1)) {
				Board* board = new Board(*g.board);
				board->movePiece(startRank, startFile, endRank1, endFile1);
				GameState* nextState = new GameState(*board);
				gCopy.nextStates.push_back(nextState);
			}

			if (g.pieces[i]->type != PieceType::Pawn && g.board->moveLegal(startRank, startFile, endRank1, endFile2)) {
				Board* board = new Board(*g.board);
				board->movePiece(startRank, startFile, endRank1, endFile2);
				GameState* nextState = new GameState(*board);
				gCopy.nextStates.push_back(nextState);
			}

			if (g.pieces[i]->type != PieceType::Pawn && g.board->moveLegal(startRank, startFile, endRank2, endFile1)) {
				Board* board = new Board(*g.board);
				board->movePiece(startRank, startFile, endRank2, endFile1);
				GameState* nextState = new GameState(*board);
				gCopy.nextStates.push_back(nextState);
			}

			if (g.pieces[i]->type != PieceType::Pawn && g.board->moveLegal(startRank, startFile, endRank2, endFile2)) {
				Board* board = new Board(*g.board);
				board->movePiece(startRank, startFile, endRank2, endFile2);
				GameState* nextState = new GameState(*board);
				gCopy.nextStates.push_back(nextState);
			}
		}
	}
	


	if (gCopy.nextStates.size() == 0) { return gCopy.nextStates; }
	if (gCopy.nextStates.size() > Breadth) {
		for (int i = 0; i < gCopy.nextStates.size(); i++) {
			gCopy.nextStates[i]->value = evaluateState(*gCopy.nextStates[i]);
		}
		//gCopy.sortNextStates(maximizingPlayer);
		//gCopy.nextStates.erase(gCopy.nextStates.begin() + Breadth, gCopy.nextStates.end());
	}

	return gCopy.nextStates;
}

long ChessAI::evaluateState(const GameState& state) {
	long value = 0;

	long whiteMaterial = 0;
	long whiteDevelopment = 0;
	long whitePosition = 0;
	long whiteValue = 0;

	long blackMaterial = 0;
	long blackDevelopment = 0;
	long blackPosition = 0;
	long blackValue = 0;

	for (int i = 0; i < state.pieces.size(); i++) {
		if (state.pieces[i]->color == "White") {
			whiteMaterial = whiteMaterial + evaluatePieceValues(*state.pieces[i]);
			whiteDevelopment = whiteDevelopment + evaluateDevelopment(*state.pieces[i]);
			//whitePosition = whitePosition + evaluateSquaresControlled(*state.pieces[i], state);
		}
		else if (state.pieces[i]->color == "Black") {
			blackMaterial = blackMaterial + evaluatePieceValues(*state.pieces[i]);
			blackDevelopment = blackDevelopment + evaluateDevelopment(*state.pieces[i]);
			//blackPosition = blackPosition + evaluateSquaresControlled(*state.pieces[i], state);
		}
	}

	whiteValue = (100*whiteMaterial) + (whiteDevelopment) + (whitePosition);
	blackValue = (100*blackMaterial) + (blackDevelopment) + (blackPosition);
	
	value = whiteValue - blackValue;
	return value;
}

long ChessAI::evaluatePieceValues(const Piece& piece)
{
	if (piece.type == PieceType::Pawn) { return 1; }
	else if (piece.type == PieceType::Knight || piece.type == PieceType::Bishop) { return 3; }
	else if (piece.type == PieceType::Rook) { return 5; }
	else if (piece.type == PieceType::Queen) { return 9; }
	return 0;
}

long ChessAI::evaluateThreats(const GameState& state)
{
	return 0;
}

long ChessAI::evaluateDevelopment(const Piece & piece)
{
	auto moveValue = [](int t, int m) -> long {
		if(t < 20 && m <= 1) {
			return m;
		}

		else if (t < 20 && m > 1) {
			return -1 * 2*m;
		}

		return 0;
	};

	if (piece.type == PieceType::Pawn) {
		return 1* moveValue(moveNumber, piece.numOfMoves);
	}

	else if (piece.type == PieceType::Knight || piece.type == PieceType::Bishop) {
		return 3 * moveValue(moveNumber, piece.numOfMoves);
	}

	else if (piece.type == PieceType::Rook) {
		return 5 * moveValue(moveNumber, piece.numOfMoves);
	}

	else if (piece.type == PieceType::Queen) {
		return 9 * moveValue(moveNumber, piece.numOfMoves);
	}

	return 0;
}

long ChessAI::evaluatePawnStructure(const GameState& state)
{
	auto adjacentPawns = [](int x, int y) {};
	auto pawnIslands = [](const Board& b) {}; 
	return 0;
}

long ChessAI::evaluateSquaresControlled(const Piece & piece, const GameState & state)
{
	auto distanceFromCenter = [](int n)->long {if (n >= 4) { return abs(4 - n) + 3; } else { return abs(n - 3) + 4; } };
	auto squareValue = [distanceFromCenter](int x, int y)->long {return distanceFromCenter(x)*distanceFromCenter(y); };
	long value = 0;

	int startRank = piece.rank;
	int startFile = piece.file;
	
	for (int i = 0; i < piece.moves.size(); i++) {
		int rankDifference = piece.moves[i].first;
		int fileDifference = piece.moves[i].second;

		int endRank1 = startRank + rankDifference;
		int endRank2 = startRank - rankDifference;
		int endFile1 = startFile + fileDifference;
		int endFile2 = startFile - fileDifference;

		if (state.board->moveLegal(startRank,startFile, endRank1, endFile1)) {
			value = value + (squareValue(endRank1, endFile1));
		}

		if (piece.type != PieceType::Pawn 
			&& state.board->moveLegal(startRank, startFile, endRank1, endFile2)) {
			value = value + (squareValue(endRank1, endFile2));
		}

		if (piece.type != PieceType::Pawn
			&& state.board->moveLegal(startRank, startFile, endRank2, endFile1)) {
			value = value + (squareValue(endRank2, endFile1));
		}

		if (piece.type != PieceType::Pawn
			&& state.board->moveLegal(startRank, startFile, endRank2, endFile2)) {
			value = value + (squareValue(endRank2, endFile2));
		}
	}

	return value;
}

long ChessAI::miniMax(GameState& gameState, int depth, long alpha, long beta, bool maximizingPlayer) {
	moveNumber++;

	long minEva = MaxBeta;
	long maxEva = MinAlpha;
	if (depth == 0) {
		return gameState.value;
	}

	if (gameState.isLeaf()) {
		gameState.nextStates = getNextPositions(gameState, maximizingPlayer);
		for (int i = 0; i < gameState.nextStates.size(); i++)
		{
			gameState.nextStates[i]->value = evaluateState(*gameState.nextStates[i]);

			if (gameState.nextStates.size() == 0) { 
				gameState.board->checkmate = true; 
				return gameState.value;
			}

			if (gameState.nextStates.size() > Breadth) {
				gameState.sortNextStates(maximizingPlayer);
				gameState.nextStates.erase(gameState.nextStates.begin() + Breadth, gameState.nextStates.end());
			}
			
		}
	}

	if (maximizingPlayer) {
		gameState.value = maxEva;
		GameState* temp = new GameState(gameState);
		for (int i = 0; i < temp->nextStates.size(); i++) {
			temp->nextStates[i]->value = miniMax(*temp->nextStates[i], depth - 1, alpha, beta, false);
			moveNumber--;
			gameState.value = std::max(gameState.value, temp->nextStates[i]->value);

			//std::cout << "Maxing, " <<"depth: " << depth << ", value: " << gameState.value << ", next value: " << temp->nextStates[i]->value << "\n";

			if (gameState.value >= beta) {
				//std::cout << "Max Pruning\n";
				break; }
			alpha = std::max(alpha, gameState.value);
		}

		return gameState.value;
	}

	else {
		gameState.value = minEva;
		GameState* temp = new GameState(gameState);

		for (int i = 0; i < temp->nextStates.size(); i++) {
			temp->nextStates[i]->value = miniMax(*temp->nextStates[i], depth - 1, alpha, beta, true);
			moveNumber--;
			gameState.value = std::min(gameState.value, temp->nextStates[i]->value);

			//std::cout << "Mining, " << "depth: " << depth << ", value: " << gameState.value << ", next value: " << temp->nextStates[i]->value << "\n";

			if (gameState.value <= alpha) { 
				//std::cout << "Min Pruning\n";
				break; }
			beta = std::min(beta, gameState.value);
		}

		return gameState.value;
	}
}

Board* ChessAI::makeMove(bool maximizingPlayer) {
	//bugs when in check
	miniMax(*gameState, Depth, MinAlpha, MaxBeta, maximizingPlayer);
	GameState* currentBest = new GameState(*gameState->nextStates[0]);

	for (int i = 1; i < gameState->nextStates.size(); i++) {
		if (maximizingPlayer  && gameState->nextStates[i]->value > currentBest->value) {
			currentBest = new GameState(*gameState->nextStates[i]);
		}

		else if (!maximizingPlayer && gameState->nextStates[i]->value < currentBest->value) {
			currentBest = new GameState(*gameState->nextStates[i]);
		}
	}

	std::cout << currentBest->value << "\n";
	return currentBest->board;
}
