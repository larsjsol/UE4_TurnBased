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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString("Starting turn for ") += TeamName.ToString());

	for (auto *c : Characters)
	{
		c->PrepareForNextTurn();
	}

	// Just end the turn if we are not controlled by a human
	if (!PlayerController) {
		EndTurn_Implementation();
	}
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

ATB_Character* ATeamController::GetActiveCharacter_Implementation()
{
	if (Characters.Num()) {
		return Characters[CurrentCharacterId];
	}
	else
	{
		return NULL;
	}
}

void ATeamController::RegisterCharacter_Implementation(ATB_Character* NewCharacter)
{
	Characters.Push(NewCharacter);
}