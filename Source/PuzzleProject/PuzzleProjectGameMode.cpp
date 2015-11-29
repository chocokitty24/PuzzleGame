// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "PuzzleProject.h"
#include "PuzzleProjectGameMode.h"
#include "PuzzleProjectPlayerController.h"

APuzzleProjectGameMode::APuzzleProjectGameMode()
{
	// no pawn by default
	DefaultPawnClass = NULL;
	// use our own player controller class
	PlayerControllerClass = APuzzleProjectPlayerController::StaticClass();
}
