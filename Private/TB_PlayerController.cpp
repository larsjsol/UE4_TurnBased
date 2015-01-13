// Fill out your copyright notice in the Description page of Project Settings.


#include "UE4_TurnBased.h"
#include "TB_GameState.h"
#include "TB_PlayerController.h"


ATB_PlayerController::ATB_PlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	TeamControllerClass = ATB_TeamController::StaticClass();
}

void ATB_PlayerController::BeginPlay() 
{
	UWorld* World = GetWorld();
	GameState = (ATB_GameState *) World->GameState;

	//initialize TB_TeamController
	TeamController = World->SpawnActor<ATB_TeamController>(TeamControllerClass);

	//set team name and register our teamcontroller with the gamestate
	TeamController->TeamName = FName(TEXT("Team Human"));
	GameState->RegisterTeamController(TeamController);
	TeamController->PlayerController = this;

	Super::BeginPlay();
}

bool ATB_PlayerController::LineTraceFromScreenPos(float ScreenX, float ScreenY, FHitResult& WorldHit)
{
	FVector Location;
	FVector Direction;
	if (DeprojectScreenPositionToWorld(ScreenX, ScreenY, Location, Direction)) {
		float LineLength = 100000; // guess of what is not to long and not to short...
		if (GetWorld()->LineTraceSingle(WorldHit, Location, Direction * LineLength, ECC_Visibility,
			FCollisionQueryParams(), FCollisionResponseParams()))
		{
			return true;
		}
	}
	return false;
}
/* User actions */

void ATB_PlayerController::EndTurn_Implementation()
{
	TeamController->EndTurn();
}

void ATB_PlayerController::SelectNextCharacter_Implementation()
{
	TeamController->ActivateNextCharacterThatCanAct();
}

void ATB_PlayerController::CharacterMoveTo_Implementation(FVector Destination)
{
	auto *Character = TeamController->GetActiveCharacter();
	if (Character)
	{
		Character->MoveTo(Destination);
	}
}

void ATB_PlayerController::CharacterReload_Implementation()
{
	auto *Character = TeamController->GetActiveCharacter();
	if (Character)
	{
		Character->Reload();
	}
}

void ATB_PlayerController::CharacterAttack_Implementation()
{
	auto *Character = TeamController->GetActiveCharacter();
	if (Character)
	{
		Character->InitiateAttack();
	}
}

void ATB_PlayerController::CharacterTargetNextEnemy_Implementation()
{
	auto *Character = TeamController->GetActiveCharacter();
	if (Character)
	{
		Character->TargetNextEnemy();
	}
}

void ATB_PlayerController::OnBeginTurn_Implementation()
{

}

void ATB_PlayerController::OnEndTurn_Implementation()
{

}