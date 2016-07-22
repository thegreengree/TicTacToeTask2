// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "TTTP.h"
#include "TTTPBlock.h"
#include "TTTPBlockGrid.h"
#include "Kismet/GameplayStatics.h"
#include "TTTPGameMode.h"

ATTTPBlock::ATTTPBlock()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> XMesh;
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> OMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> BlueMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> OrangeMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> RedMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, XMesh(TEXT("/Game/Puzzle/Meshes/shape_x.Shape_x"))
			, OMesh(TEXT("/Game/Puzzle/Meshes/Shape_o.Shape_O"))
			, BlueMaterial(TEXT("/Game/Puzzle/Meshes/BlueMaterial.BlueMaterial"))
			, OrangeMaterial(TEXT("/Game/Puzzle/Meshes/OrangeMaterial.OrangeMaterial"))
			, RedMaterial(TEXT("/Game/Puzzle/Meshes/RedMaterial.RedMaterial"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	BlockMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh0"));
	BlockMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());
	BlockMesh->SetRelativeScale3D(FVector(1.f, 1.f, 0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f, 0.f, 25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.BlueMaterial.Get());
	BlockMesh->SetupAttachment(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &ATTTPBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &ATTTPBlock::OnFingerPressedBlock);

	// Save a pointer to the materials
	OrangeMaterial = ConstructorStatics.OrangeMaterial.Get();
	RedMaterial = ConstructorStatics.RedMaterial.Get();
	BlueMaterial = ConstructorStatics.BlueMaterial.Get();
	// Save a pointer to the meshes
	XMesh = ConstructorStatics.XMesh.Get();
	OMesh = ConstructorStatics.OMesh.Get();
	DefMesh = ConstructorStatics.PlaneMesh.Get();

	// Set starting state
	SetBlockState(0);

}

void ATTTPBlock::BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked)
{
	OnClickAction();	
}

/** Add function so we can call it without performing a click*/
void ATTTPBlock::OnClickAction()
{
	// Get reference to GameMode
	ATTTPGameMode* MyGameMode = Cast<ATTTPGameMode>(UGameplayStatics::GetGameMode(this));

	// Check what state is that and change state accordinally
	if (MyGameMode && MyGameMode->GetCurrentState() == EPlayState::EPlaying)
	{
		// Check we are not already active
		if (!bIsActive)
		{
			bIsActive = true;

			if (MyGameMode->GetCurrentPlayer() == MyGameMode->GetPlayer1())
			{
				SetBlockState(1);
				// Update state
				MyGameMode->CheckState();
				// if playing - change current player
				if (MyGameMode->GetCurrentState() == EPlayState::EPlaying)
				MyGameMode->SetCurrentPlayer(MyGameMode->GetPlayer2());
			}
			else if (MyGameMode->GetCurrentPlayer() == MyGameMode->GetPlayer2())
			{
				SetBlockState(2);
				// Update state
				MyGameMode->CheckState();
				// if playing - change current player
				if (MyGameMode->GetCurrentState() == EPlayState::EPlaying)
				MyGameMode->SetCurrentPlayer(MyGameMode->GetPlayer1());

			}

			// If AI turn - call a function to perform move
			if (MyGameMode->GetCurrentPlayer() == EPlayTurn::EAI_Turn && MyGameMode->GetCurrentState() == EPlayState::EPlaying)
			{
				MyGameMode->PerformAITurn();
			}

		}
	}

}


void ATTTPBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	BlockClicked(TouchedComponent, EKeys::Invalid);
}

/** Set's the Mesh and Material based on the State
0 - Default State
1 - X State
2 - O State
*/
void ATTTPBlock::SetBlockState(int NewState)
{
	BlockState = NewState;
	switch (NewState)
	{
	case 0:
	{
		if (DefMesh)
			BlockMesh->SetStaticMesh(DefMesh);

		BlockMesh->SetMaterial(0, BlueMaterial);
		break;
	}
	case 1:
	{
		 if (XMesh)
			 BlockMesh->SetStaticMesh(XMesh);
		 
		BlockMesh->SetMaterial(0, OrangeMaterial);
		break;
	}
	case 2:
	{
		 if (OMesh)
			 BlockMesh->SetStaticMesh(OMesh);

		BlockMesh->SetMaterial(0, RedMaterial);
		break;
	}
	default:
		break;
	}
}

// returns BlockState
int ATTTPBlock::GetBlockState()
{
	return BlockState;
}
