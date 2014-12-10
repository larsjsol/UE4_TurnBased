// Fill out your copyright notice in the Description page of Project Settings.


#include "UE4_TurnBased.h"
#include "TB_GameState.h"
#include "TB_PlayerController.h"


ATB_PlayerController::ATB_PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void ATB_PlayerController::BeginPlay() 
{
	APlayerController::BeginPlay();

	UWorld* world = GetWorld();

	//initialize TeamController
	TeamController = world->SpawnActor<ATeamController>(FActorSpawnParameters());

	//set team name and register our teamcontroller with the gamestate
	TeamController->TeamName = FName(TEXT("Team Human"));
	ATB_GameState* gamestate = (ATB_GameState *)world->GameState;
	gamestate->RegisterTeamController(TeamController);
}
