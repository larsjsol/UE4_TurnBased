// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TeamController.h"
#include "TB_GameState.h"


ATeamController::ATeamController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TeamName = this->GetFName();
}

void ATeamController::PlayTurn_Implementation() 
{
	;
}

void ATeamController::EndTurn_Implementation()
{
	UWorld* world = GetWorld();
	ATB_GameState* gamestate = (ATB_GameState *) world->GameState;
	gamestate->EndTurn();
}

void ATeamController::ActivateNextCharacter_Implementation()
{
	CurrentCharacterId += 1;
	if (CurrentCharacterId >= (uint32) Characters.Num()) 
	{
		CurrentCharacterId = 0;
	}
}

ACharacter* ATeamController::GetActiveCharacter_Implementation()
{
	if (Characters.Num()) {
		return Characters[CurrentCharacterId];
	}
	else
	{
		return NULL;
	}
}

void ATeamController::RegisterCharacter_Implementation(ACharacter* NewCharacter)
{
	Characters.Push(NewCharacter);
}