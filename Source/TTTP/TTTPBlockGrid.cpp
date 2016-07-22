// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TTTP.h"
#include "TTTPBlockGrid.h"
#include "TTTPBlock.h"
#include "Components/TextRenderComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TTTPGameMode.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

ATTTPBlockGrid::ATTTPBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Set defaults
	Size = 3;
	BlockSpacing = 300.f;
}


void ATTTPBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
	const int32 NumBlocks = Size * Size;

	ATTTPGameMode* MyGameMode = Cast<ATTTPGameMode>(UGameplayStatics::GetGameMode(this));

	// Loop to spawn each block
	for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex/Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

		// Make postion vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		ATTTPBlock* NewBlock = GetWorld()->SpawnActor<ATTTPBlock>(BlockLocation, FRotator(0,0,0));

		// Add button to GameMode array
		MyGameMode->AddButton(NewBlock);
	}

	MyGameMode->CheckBlockActors();

}

#undef LOCTEXT_NAMESPACE
