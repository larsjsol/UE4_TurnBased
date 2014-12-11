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

	//initialize TB_TeamController
	TB_TeamController = world->SpawnActor<ATB_TeamController>(FActorSpawnParameters());

	//set team name and register our teamcontroller with the gamestate
	TB_TeamController->TeamName = FName(TEXT("Team Human"));
	ATB_GameState* gamestate = (ATB_GameState *)world->GameState;
	gamestate->RegisterTB_TeamController(TB_TeamController);

	TB_TeamController->PlayerController = this;
}
