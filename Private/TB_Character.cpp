// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_Character.h"
#include "TB_GameState.h"
#include "TB_Weapon.h"

#include "AIController.h"
#include "AI/Navigation/NavigationPath.h"

ATB_Character::ATB_Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIControllerClass = AAIController::StaticClass();

}

void ATB_Character::OnConstruction(const FTransform& Transform)
{
	ACharacter::OnConstruction(Transform);

	/* Use the actor name if no other is given */
	if (Name.IsNone())
	{
		Name = this->GetFName();
	}

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, Name.ToString());

	/* Equip weapon if we have one */
	if (!Weapon && WeaponClass)
	{
		UWorld *World = GetWorld();
		Weapon = World->SpawnActor<ATB_Weapon>(WeaponClass);
	}
	if (Weapon)
	{
		Weapon->Equip(this);
	}
}

void ATB_Character::BeginPlay()
{
	ACharacter::BeginPlay();

	/* Join a team */
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
	auto Vel = GetVelocity();
	return Busy || Vel.Size() > 0;
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
	if (ActionPoints <= 0)
	{
		return;
	}
	ActionPoints--;

	auto *aic = (AAIController*) Controller;
	aic->MoveToLocation(Destination, 0.05, false);
}
