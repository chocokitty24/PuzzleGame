// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "PuzzleProject.h"
#include "PuzzleProjectBlockGrid.h"
#include "PuzzleProjectBlock.h"
#include "Components/TextRenderComponent.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

//Global variable that points to a 2-D array of pointers to each grid block
APuzzleProjectBlock*** blockptrs;

APuzzleProjectBlockGrid::APuzzleProjectBlockGrid()
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Moves Remaining: {0}"), FText::AsNumber(50)));
	ScoreText->AttachTo(DummyRoot);

	// Set defaults
	Size = 5;
	BlockSpacing = 300.f;
	Score = 50;

}


void APuzzleProjectBlockGrid::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
	const int32 NumBlocks = Size * Size;

	//Dynamically set up the 2D array of pointers 1 block bigger grid than the size
	blockptrs = new APuzzleProjectBlock**[Size + 2];
	for (int i = 0; i < Size + 2; i++){
		blockptrs[i] = new APuzzleProjectBlock*[Size + 2];
	}

	//Initializes all block pointers to NULL
	for (int i = 0; i < Size + 2; i++){
		for (int j = 0; j < Size + 2; j++){
			blockptrs[i][j] = NULL;
		}
	}

	// Loop to spawn each block
	for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
	{
		const int x = (BlockIndex / Size);
		const int y = (BlockIndex % Size);
		const float XOffset = x * BlockSpacing; // Divide by dimension
		const float YOffset = y * BlockSpacing; // Modulo gives remainder


		// Make postion vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block, keep track of it in blockptrs array
		APuzzleProjectBlock* NewBlock = GetWorld()->SpawnActor<APuzzleProjectBlock>(BlockLocation, FRotator(0,0,0));
		blockptrs[x+1][y+1] = NewBlock;
	
		// Tell the block about its owner
		if(NewBlock != NULL)
		{
			NewBlock->OwningGrid = this;
		}
	}
	
	//Initialize the block's pointers to its neighbors using the 2D array of blocks
	for (int i = 1; i < Size + 1; i++){
		for (int j = 1; j < Size + 1; j++){
			blockptrs[i][j]->top = blockptrs[i][j + 1];
			blockptrs[i][j]->bot = blockptrs[i][j - 1];
			blockptrs[i][j]->left = blockptrs[i - 1][j];
			blockptrs[i][j]->rght = blockptrs[i + 1][j];
		}
	}
}


void APuzzleProjectBlockGrid::SubScore()
{
	// Decrement score
	Score--;

	if (Score <= 0){
		ScoreText->SetText("Out of moves! GAME OVER");
		//END GAME CODE
	} else {
		// Update text
		ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Moves Remaining: {0}"), FText::AsNumber(Score)));
	}
}

void APuzzleProjectBlockGrid::CheckWin()
{
	int Redcnt = 0;

	//Checks if each block is red
	for (int i = 1; i < Size + 1; i++){
		for (int j = 1; j < Size + 1; j++){
			if (blockptrs[i][j]->bIsRed){
				Redcnt++;
			}
		}
	}

	//If every block is red, player has won 
	if (Redcnt == Size*Size){
		ScoreText->SetText("Yay! You won!!");
	}
}

#undef LOCTEXT_NAMESPACE
