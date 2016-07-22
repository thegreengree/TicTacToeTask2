// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Actor.h"
#include "TTTPBlock.generated.h"

/** A block that can be clicked */
UCLASS(minimalapi)
class ATTTPBlock : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

public:
	ATTTPBlock();

	/** Are we currently active? */
	bool bIsActive;

	/** Pointers to store meshes */
	UPROPERTY()
	class UStaticMesh* DefMesh;
	UPROPERTY()
	class UStaticMesh* XMesh;
	UPROPERTY()
	class UStaticMesh* OMesh;

	/** Pointer to store materials */
	UPROPERTY()
	class UMaterialInstance* BlueMaterial;
	UPROPERTY()
	class UMaterialInstance* OrangeMaterial;
	UPROPERTY()
	class UMaterialInstance* RedMaterial;

	/** Handle the block being clicked */
	UFUNCTION()
		void BlockClicked(UPrimitiveComponent* ClickedComp, FKey ButtonClicked);

	/** Handle the block being touched  */
	UFUNCTION()
		void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

	// returns BlockState
	int GetBlockState();

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }

	// Set's the BlockState
	void SetBlockState(int NewState);

	/** Add function so we can call it without performing a click*/
	void OnClickAction();

private:
	/**Stores a current state, 0 - nothing, 1 - Player 1 (X) , 2 - Player 2 (O)*/
	int BlockState;
};



