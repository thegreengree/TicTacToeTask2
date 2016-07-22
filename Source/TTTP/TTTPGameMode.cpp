// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TTTP.h"
#include "TTTPGameMode.h"
#include "TTTPPlayerController.h"
#include "TTTPBlock.h"
#include "Blueprint/UserWidget.h"
#include "Math/RandomStream.h"

ATTTPGameMode::ATTTPGameMode()
{
	// no pawn by default
	DefaultPawnClass = NULL;
	// use our own player controller class
	PlayerControllerClass = ATTTPPlayerController::StaticClass();

}

void ATTTPGameMode::BeginPlay()
{
	Super::BeginPlay();


	/**Set default values */
	SetCurrentState(EPlayState::EUnknown);
	SetPlayer1(EPlayTurn::EP1_Turn);
	SetPlayer2(EPlayTurn::EAI_Turn);
	SetCurrentPlayer(Player1);
	//AI
	_humanPlayer = 1;
	_aiPlayer = 2;

	// Set our UserWidget on
	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}

}

EPlayState ATTTPGameMode::GetCurrentState() const
{
	return CurrentState;
}

EPlayTurn ATTTPGameMode::GetCurrentPlayer() const
{
	return CurrentPlayer;
}

EPlayTurn  ATTTPGameMode::GetPlayer1()
{
	return Player1;
}

EPlayTurn  ATTTPGameMode::GetPlayer2()
{
	return Player2;
}

void  ATTTPGameMode::SetCurrentState(EPlayState NewState)
{
	// set current state
	CurrentState = NewState;
}

void  ATTTPGameMode::SetCurrentPlayer(EPlayTurn NewPlayer)
{
	// set current state
	CurrentPlayer = NewPlayer;
}

void ATTTPGameMode::SetPlayer1(EPlayTurn p)
{
	Player1 = p;
	if (p == EPlayTurn::EAI_Turn)
		_aiPlayer = 1;
	else
		_humanPlayer = 1;
}

void ATTTPGameMode::SetPlayer2(EPlayTurn p)
{
	Player2 = p;
	if (p == EPlayTurn::EAI_Turn)
		_aiPlayer = 2;
	else
		_humanPlayer = 2;
}

void ATTTPGameMode::AddButton(ATTTPBlock* NewButton)
{
	BlockActors.Add(NewButton);
}

// Moves top row to the bottom and adds references to 
void ATTTPGameMode::CheckBlockActors()
{
	for (int i = 0; i < _size; i++)
	{
		ATTTPBlock* bfr;
		bfr = BlockActors[i];
		BlockActors[i] = BlockActors[i + _size*2];
		BlockActors[i + _size*2] = bfr;
	}

	int bfr = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			_board[i][j] = BlockActors[bfr];
			bfr++;
		}
	}

	UpdateBoard();
}

// Updates local [int][int] Board array
void ATTTPGameMode::UpdateBoard()
{
	int bfr = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			Board[i][j] = _board[i][j]->GetBlockState();
		}
	}

	// print our board 
	for (int i = 0; i < 3; i++)
	{
		UE_LOG(LogClass, Log, TEXT("%i %i %i"), Board[i][0], Board[i][1], Board[i][2]);
	}


}

// Updates the board and changes CurrentState if someone won 
void ATTTPGameMode::CheckState()
{
	UpdateBoard();

	switch (checkVictory())
	{
	case -1:
	{
		SetCurrentState(EPlayState::ETie);
		break;
	}
	case 0:
	{
		SetCurrentState(EPlayState::EPlaying);
		break;
	}
	case 1:
	{
		SetCurrentState(EPlayState::EGameOver);
		break;
	}
	case 2:
	{
		SetCurrentState(EPlayState::EGameOver);
		break;
	}
	default:
		break;
	}

}

// Get's the best move position and performs that move
void ATTTPGameMode::PerformAITurn()
{
	AiMove bestMove = getBestMove(_aiPlayer);
	_board[bestMove.x][bestMove.y]->OnClickAction();
	UE_LOG(LogClass, Log, TEXT("AI move"));
}

// Game Reset function - Resets all blocks and updates players
void ATTTPGameMode::ResetGame()
{
	for (ATTTPBlock* Block : BlockActors)
	{
		Block->SetBlockState(0);
		Block->bIsActive = false;
	}

	SetCurrentState(EPlayState::EPlaying);
	CurrentPlayer = Player1;
	if (CurrentPlayer == EPlayTurn::EAI_Turn)
	{
		UpdateBoard();
		PerformAITurn();
	}
}

// Checks for a victory
/** Returns 
-1 - TIE
 0 - No state
 1 - Player1 victory
 2 - Player2 victory
*/
int ATTTPGameMode::checkVictory()
{
	bool victory;
	int c;
	// Check the rows
	for (int y = 0; y < _size; y++) {
		c = Board[0][y];
		if (c != NO_VAL) {
			victory = true;
			for (int x = 0; x < _size; x++) {
				if (Board[x][y] != c) {
					victory = false;
					break;
				}
			}
			if (victory) return c;
		}
	}

	// Check the columns
	for (int x = 0; x < _size; x++) {
		c = Board[x][0];
		if (c != NO_VAL) {
			victory = true;
			for (int y = 0; y < _size; y++) {
				if (Board[x][y] != c) {
					victory = false;
					break;
				}
			}
			if (victory) return c;
		}
	}

	// Check top left diagonal
	c = Board[0][0];
	if (c != NO_VAL) {
		victory = true;
		for (int xy = 0; xy < _size; xy++) {
			if (Board[xy][xy] != c) {
				victory = false;
				break;
			}
		}
		if (victory) return c;
	}

	// Check top right diagonal
	c = Board[_size - 1][0];
	if (c != NO_VAL) {
		victory = true;
		for (int xy = 0; xy < _size; xy++) {
			if (Board[_size - xy - 1][xy] != c) {
				victory = false;
				break;
			}
		}
		if (victory) return c;
	}

	// Check for tie game
	for (int i = 0; i < _size; i++)
	{
		for (int j = 0; j < _size; j++)
		{
			if (Board[i][j] == NO_VAL) return NO_VAL;
		}
	}
	// If we get here, every spot was filled, so return tie
	return TIE_VAL;
}

// AI

void ATTTPGameMode::setVal(int x, int y, int player)
{
	Board[x][y] = player;
}

// Returns a best move as AiMove
AiMove ATTTPGameMode::getBestMove(int player) {

	// Base case, check for end state
	int rv = checkVictory();
	if (rv == _aiPlayer) {
		return AiMove(10);
	}
	else if (rv == _humanPlayer) {
		return AiMove(-10);
	}
	else if (rv == TIE_VAL) {
		return AiMove(0);
	}

	// Arrays of moves and simular moves
	TArray<AiMove> moves;
	TArray<AiMove> EqualMoves;

	// Do the recursive function calls and construct the moves vector
	for (int y = 0; y < _size; y++) {
		for (int x = 0; x < _size; x++) {
			if (Board[x][y] == NO_VAL) {
				AiMove move;
				move.x = x;
				move.y = y;
				setVal(x, y, player);
				if (player == _aiPlayer) {
					move.score = getBestMove(_humanPlayer).score;
				}
				else {
					move.score = getBestMove(_aiPlayer).score;
				}
				moves.Add(move);
				setVal(x, y, NO_VAL);
			}
		}
	}


	// Pick the best move for the current player
	int bestMove = 0;
	if (player == _aiPlayer) {
		int bestScore = -1000000;
		for (int i = 0; i < moves.Num(); i++) {
			if (moves[i].score > bestScore) {
				bestMove = i;
				bestScore = moves[i].score;
			}
		}
	}
	else {
		int bestScore = 1000000;
		for (int i = 0; i < moves.Num(); i++) {
			if (moves[i].score < bestScore) {
				bestMove = i;
				bestScore = moves[i].score;
			}
		}
	}

	// Add all moves with equal score to EqualMoves
	int equalTurns = 0;
	for (int i = 0; i < moves.Num(); i++) {
		if (moves[i].score == moves[bestMove].score) {
			EqualMoves.Add(moves[i]);
			equalTurns++;
		}
	}
	
	// Changes random move out of EqualMoves
	if (equalTurns > 1)
	{
		bestMove = FMath::RandRange(0, equalTurns - 1);
		return EqualMoves[bestMove];
	}
	// Return the best move
	return moves[bestMove];
}