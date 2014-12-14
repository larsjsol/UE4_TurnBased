// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_Character.h"
#include "TB_GameState.h"

#include "AIController.h"
#include "AI/Navigation/NavigationPath.h"

ATB_Character::ATB_Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIControllerClass = AAIController::StaticClass();
	Name = this->GetFName();
}

void ATB_Character::BeginPlay()
{
	ACharacter::BeginPlay();

	auto *World = GetWorld();
	auto *GameState = (ATB_GameState*) World->GetGameState();
	auto *TeamController = GameState->GetTeamController(TeamName);
	if (TeamController)
	{
		TeamController->RegisterCharacter(this);
	}
	else 
	{
		UE_LOG(TB_Log, Warning, TEXT("%s is part of team %s, but no such team exist"), *Name.ToString(), *TeamName.ToString());
	}
}

void ATB_Character::PrepareForNextTurn_Implementation()
{
	ActionPoints = MaxActionPoints;
}

void ATB_Character::PrepareForThisTurn_Implementation()
{
	;
}

bool ATB_Character::IsBusy_Implementation()
{
	return Busy;
}

void ATB_Character::SetBusy_Implementation(float BusyDuration)
{
	Busy = true;
	GetWorldTimerManager().SetTimer(this, &ATB_Character::ClearBusy, BusyDuration, false);
}

void ATB_Character::ClearBusy_Implementation()
{
	Busy = false;
}

bool ATB_Character::CanMoveTo_Implementation(FVector Destination)
{
	auto *World = GetWorld();
	auto *NavigationSystem = World->GetNavigationSystem();
	auto *Path = NavigationSystem->FindPathToLocationSynchronously(World, GetActorLocation(), Destination);
	auto Length = Path->GetPathLength();

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString("Path length: ") += FString::SanitizeFloat(Length / 100) += FString("m"));

	return Length >= 0 && Length <= Movement;
}

void ATB_Character::MoveTo_Implementation(FVector Destination)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString("Moving to ") += Destination.ToString());
	auto *aic = (AAIController*) Controller;
	aic->MoveToLocation(Destination, 0.05, false);
}
