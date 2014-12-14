// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_TeamController.h"
#include "TB_GameState.h"


ATB_GameState::ATB_GameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Turn = 1;
}

void ATB_GameState::EndTurn()
{
	CurrentTeamId++;
	if (CurrentTeamId >= (uint32)TeamControllers.Num())
	{
		CurrentTeamId = 0;
		Turn += 1;
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString("Starting turn ") += FString::FromInt(Turn));
	}
	TeamControllers[CurrentTeamId]->PlayTurn();
}

void ATB_GameState::RegisterTeamController(ATB_TeamController* TeamController)
{
	TeamControllers.Add(TeamController);
}

ATB_TeamController* ATB_GameState::GetTeamController(FName TeamName)
{
	for (auto *tc : TeamControllers)
	{
		if (tc->TeamName == TeamName)
		{
			return tc;
		}
	}
	return NULL; // not found
}

void ATB_GameState::BeginPlay()
{
	AGameState::BeginPlay();

	UWorld* world = GetWorld();

	//initialize TB_TeamController
	ATB_TeamController *controller = world->SpawnActor<ATB_TeamController>(FActorSpawnParameters());

	//make sure that there are at least one enemy team
	controller->TeamName = FName(TEXT("Team AI"));
	RegisterTeamController(controller);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString("Starting turn ") += FString::FromInt(Turn));
	TeamControllers[CurrentTeamId]->PlayTurn();
}

const TArray<ATB_TeamController *>& ATB_GameState::GetTeamControllers()
{
	return TeamControllers;
}