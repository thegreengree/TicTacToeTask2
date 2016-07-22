// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/GameMode.h"
#include "TTTPBlock.h"
#include "TTTPGameMode.generated.h"

// Structure to hold AI moves
struct AiMove {
	AiMove() {};
	AiMove(int Score) : score(Score) {}
	int x;
	int y;
	int score;
};

// Values on the board 
const int TIE_VAL = -1; // Tie
const int NO_VAL = 0; // Nothing
const int X_VAL = 1; // X value
const int O_VAL = 2; // O value

// Horizontal and vertical size of the board
const int _size = 3;



// Enum to store who is current player
UENUM(BlueprintType)
enum class EPlayTurn {
	EP1_Turn,
	EP2_Turn,
	EAI_Turn,
};

// Enum to store game states
UENUM(BlueprintType)
enum class EPlayState {
	EPlaying,
	EGameOver,
	ETie,
	EUnknown
};


/** GameMode class to specify pawn and playercontroller */
UCLASS(minimalapi)
class ATTTPGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATTTPGameMode();

	// Overriden BeginPlay 
	virtual void BeginPlay() override;

	// Returns the CurrentState
	UFUNCTION(BlueprintPure, Category = "PlayState")
	EPlayState GetCurrentState() const;

	// Returns the CurrentPlayer
	UFUNCTION(BlueprintPure, Category = "PlayState")
	EPlayTurn GetCurrentPlayer() const;

	// Returns Player1
	EPlayTurn GetPlayer1();
	//ReturnsPlayer2
	EPlayTurn GetPlayer2();

	// Sets the CurrentState
	UFUNCTION(BlueprintCallable, Category = "PlayState")
	void SetCurrentState(EPlayState NewState);

	// Sets the CurrentPlayer
	UFUNCTION(BlueprintCallable, Category = "PlayState")
	void SetCurrentPlayer(EPlayTurn NewPlayer);

	// Sets Player1
	UFUNCTION(BlueprintCallable, Category = "PlayState")
	void SetPlayer1(EPlayTurn p);
	// Sets Player2
	UFUNCTION(BlueprintCallable, Category = "PlayState")
	void SetPlayer2(EPlayTurn p);

	// Adds a new reference to BlockActors
	void AddButton(ATTTPBlock* NewButton);

	// Moves top row to the bottom and adds references to 
	void CheckBlockActors();

	// Updates local [int][int] Board array
	void UpdateBoard();

	// Updates the board and changes CurrentState if someone won 
	void CheckState();

	// Get's the best move position and performs that move
	void PerformAITurn();

	// Game Reset function - Resets all blocks and updates players
	UFUNCTION(BlueprintCallable, Category = "PlayState")
	void ResetGame();

protected:
	/* The widget class to use for our HUD screed*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayState", Meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	/* The instance of the HUD*/
	UPROPERTY()
		class UUserWidget* CurrentWidget;

public:
	EPlayState CurrentState;
	EPlayTurn CurrentPlayer;
	EPlayTurn Player1;
	EPlayTurn Player2;

private:
	AiMove getBestMove(int player);
	// Index of the AI
	int _aiPlayer; 
	// Index of the player
	int _humanPlayer; 

	// Check for GameState
	int checkVictory();
	void setVal(int x, int y, int player);

	// Stores references to the buttons 
	ATTTPBlock* _board[_size][_size];

	// Stores BlockState values of blocks in array
	int Board[_size][_size];

	// Stores all the block actors 
	TArray<class ATTTPBlock*> BlockActors;
};



