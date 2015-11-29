// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Actor.h"
#include "PuzzleProjectBlock.generated.h"

/** A block that can be clicked */
UCLASS(minimalapi)
class APuzzleProjectBlock : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** StaticMesh component for the clickable block */
	UPROPERTY(Category = Block, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* BlockMesh;

public:
	APuzzleProjectBlock();

	//Declaration of variables for block color and surrounding blocks
	bool bIsDark = true; //starts dark
	bool bIsRed = false;

	//Variables that hold pointers to the surrounding blocks
	APuzzleProjectBlock* top;
	APuzzleProjectBlock* bot;
	APuzzleProjectBlock* left;
	APuzzleProjectBlock* rght;

	UPROPERTY()
	class UMaterialInstance* RedMaterial;
	UPROPERTY()
	class UMaterialInstance* DarkMaterial;

	/** Grid that owns us */
	UPROPERTY()
	class APuzzleProjectBlockGrid* OwningGrid;

	//Changes the material of the surrounding blocks
	void SwitchSurround(APuzzleProjectBlock* ptr);

	/** Handle the block being clicked */
	UFUNCTION()
	void BlockClicked(UPrimitiveComponent* ClickedComp);

	/** Handle the block being touched  */
	UFUNCTION()
	void OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent);

public:
	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns BlockMesh subobject **/
	FORCEINLINE class UStaticMeshComponent* GetBlockMesh() const { return BlockMesh; }
};



