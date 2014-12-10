// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TeamController.h"
#include "TB_GameState.h"


ATB_GameState::ATB_GameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Turn = 1;
}

void ATB_GameState::EndTurn()
{
	Turn += 1;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::FromInt(Turn));
}

void ATB_GameState::RegisterTeamController(ATeamController* TeamController)
{
	TeamControllers.Add(TeamController->TeamName, TeamController);
}

ATeamController* ATB_GameState::GetTeamController(FName TeamName)
{
	ATeamController** t = TeamControllers.Find(TeamName);
	if (t) 
	{
		return *t;
	}
	else 
	{
		return NULL;
	}
}

void ATB_GameState::BeginPlay()
{
	AGameState::BeginPlay();

	UWorld* world = GetWorld();

	//initialize TeamController
	ATeamController *controller = world->SpawnActor<ATeamController>(FActorSpawnParameters());

	//make sure that there are at least one enemy team
	controller->TeamName = FName(TEXT("Team AI"));
	RegisterTeamController(controller);
}
