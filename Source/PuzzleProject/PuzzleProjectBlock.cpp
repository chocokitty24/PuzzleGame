// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "PuzzleProject.h"
#include "PuzzleProjectBlock.h"
#include "PuzzleProjectBlockGrid.h"

APuzzleProjectBlock::APuzzleProjectBlock()
{
	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> DarkMaterial;
		ConstructorHelpers::FObjectFinderOptional<UMaterialInstance> RedMaterial;
		FConstructorStatics()
			: PlaneMesh(TEXT("/Game/Puzzle/Meshes/PuzzleCube.PuzzleCube"))
			, DarkMaterial(TEXT("/Game/Puzzle/Meshes/DarkMaterial.DarkMaterial"))
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
	BlockMesh->SetRelativeScale3D(FVector(1.f,1.f,0.25f));
	BlockMesh->SetRelativeLocation(FVector(0.f,0.f,25.f));
	BlockMesh->SetMaterial(0, ConstructorStatics.DarkMaterial.Get());
	BlockMesh->AttachTo(DummyRoot);
	BlockMesh->OnClicked.AddDynamic(this, &APuzzleProjectBlock::BlockClicked);
	BlockMesh->OnInputTouchBegin.AddDynamic(this, &APuzzleProjectBlock::OnFingerPressedBlock);

	// Save pointers to the red and dark materials
	RedMaterial = ConstructorStatics.RedMaterial.Get();
	DarkMaterial = ConstructorStatics.DarkMaterial.Get();
}

void APuzzleProjectBlock::BlockClicked(UPrimitiveComponent* ClickedComp)
{
	// Checks the current material
	if (bIsDark){
	
		bIsDark = false;
		bIsRed = true;
		//Change material
		BlockMesh->SetMaterial(0, RedMaterial);
	
	}else{
	
		bIsDark = true;
		bIsRed = false;
		BlockMesh->SetMaterial(0, DarkMaterial);
	
	}
	
	//Change the material of the surrounding blocks
	SwitchSurround(top);
	SwitchSurround(bot);
	SwitchSurround(left);
	SwitchSurround(rght);

	// Tell the Grid
	if(OwningGrid != NULL){
		OwningGrid->SubScore();
		OwningGrid->CheckWin();
	}

}

void APuzzleProjectBlock::SwitchSurround(APuzzleProjectBlock* ptr)
{
	if (ptr!= NULL){
		if(ptr->bIsDark){

			ptr->bIsDark = false;
			ptr->bIsRed = true;
			ptr->BlockMesh->SetMaterial(0, RedMaterial);

		} else {

			ptr->bIsDark = true;
			ptr->bIsRed = false;
			ptr->BlockMesh->SetMaterial(0, DarkMaterial);
		
		}
	}
}

void APuzzleProjectBlock::OnFingerPressedBlock(ETouchIndex::Type FingerIndex, UPrimitiveComponent* TouchedComponent)
{
	BlockClicked(TouchedComponent);
}
