// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_TeamController.h"
#include "TB_GameState.h"


ATB_TeamController::ATB_TeamController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TeamName = this->GetFName();
}

void ATB_TeamController::BeginPlay()
{
	/* 
	In the future we will add enemies to KnownEnemies when we see them for the first time
	Just add everyone for now...
	*/
	auto *World = GetWorld();
	auto *GameState = (ATB_GameState *)World->GameState;
	auto TeamControllers = GameState->GetTeamControllers();
	for (auto *tc : TeamControllers) 
	{
		if (tc->TeamName != TeamName)
		{
			for (auto *c : tc->Characters)
			{
				KnownEnemies.Add(c);
			}
		}
	}
}

void ATB_TeamController::PlayTurn_Implementation() 
{
	UWorld* world = GetWorld();
	ATB_GameState* GameState = (ATB_GameState *)world->GameState;
	GameState->GameLog->Log(ETB_LogCategory::VE_TurnClock, FString::Printf(TEXT("Starting turn %d for %s"), GameState->Turn, *TeamName.ToString()));

	for (auto *c : Characters)
	{
		c->PrepareForThisTurn();
	}

	// Just end the turn if we are not controlled by a human
	if (!PlayerController) {
		EndTurn_Implementation();
	}
}

void ATB_TeamController::EndTurn_Implementation()
{
	for (auto *c : Characters)
	{
		c->PrepareForNextTurn();
	}

	UWorld* world = GetWorld();
	ATB_GameState* gamestate = (ATB_GameState *) world->GameState;
	gamestate->EndTurn();
}

void ATB_TeamController::ActivateNextCharacter_Implementation()
{
	if (Characters.Num() > 0)
	{
		CurrentCharacterId = (CurrentCharacterId + 1) % Characters.Num();
	}
}	

void ATB_TeamController::ActivateNextLivingCharacter_Implementation()
{
	auto previd = CurrentCharacterId;
	ActivateNextCharacter();
	while (CurrentCharacterId != previd)
	{
		if (Characters[CurrentCharacterId]->HitPoints > 0)
		{
			break;
		}
		ActivateNextCharacter();
	}
}

void ATB_TeamController::ActivateNextCharacterThatCanAct_Implementation()
{
	auto previd = CurrentCharacterId;
	ActivateNextCharacter();
	while (CurrentCharacterId != previd)
	{
		if (Characters[CurrentCharacterId]->HitPoints > 0 && 
			Characters[CurrentCharacterId]->ActionPoints > 0)
		{
			break;
		}
		ActivateNextCharacter();
	}
}

ATB_Character* ATB_TeamController::GetActiveCharacter_Implementation()
{
	if (Characters.Num()) {
		return Characters[CurrentCharacterId];
	}
	else
	{
		return NULL;
	}
}

void ATB_TeamController::RegisterCharacter_Implementation(ATB_Character* NewCharacter)
{
	Characters.Push(NewCharacter);
}

void ATB_TeamController::RegisterEnemy_Implementation(ATB_Character* Enemy)
{
	KnownEnemies.AddUnique(Enemy);
}

void ATB_TeamController::GetKnownEnemies_Implementation(TArray<ATB_Character*>& Enemies)
{
	//remove anyone that have died since the last time
	TArray<ATB_Character *> Dead;
	for (auto *c : KnownEnemies)
	{
		if (c->HitPoints <= 0)
		{
			Dead.Add(c);
		}
	}
	for (auto *c : Dead)
	{
		KnownEnemies.Remove(c);
	}

	Enemies = KnownEnemies;
}