// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_Character.h"
#include "TB_GameState.h"
#include "TB_Weapon.h"

#include "AIController.h"
#include "AI/Navigation/NavigationPath.h"

#include <algorithm>

ATB_Character::ATB_Character(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AIControllerClass = AAIController::StaticClass();
	WeaponHandlingAnimations.AddZeroed((int32) ETB_WeaponAnimType::VE_Length);
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

	/* Use the actor name if no other is given */
	if (Name.IsNone())
	{
		Name = this->GetFName();
	}

	/* Equip weapon if we have one  */
	if (WeaponClass && Weapon == NULL)
	{
		Weapon = GetWorld()->SpawnActor<ATB_Weapon>(WeaponClass);
		Weapon->Equip(this);
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

void ATB_Character::Reload_Implementation()
{
	/* Do we have enough APs and a weapon? */
	if (ActionPoints <= 0 || Weapon == NULL)
	{
		return;
	}

	/* figure out how much time we will spend on this */
	float ReloadTime = PlayAnimation(WeaponHandlingAnimations[(int32) Weapon->AnimType].Reload);
	float WeaponReloadTime = Weapon->Reload();
	SetBusy(std::max(ReloadTime, WeaponReloadTime));
	
	ActionPoints--;
}

float ATB_Character::PlayAnimation(UAnimationAsset *AnimationAsset) {
	if (Mesh && AnimationAsset)
	{
		Mesh->PlayAnimation(AnimationAsset, false);
		UAnimInstance *AnimInstance = Mesh->GetAnimInstance();
		return AnimInstance->GetAnimAssetPlayerLength(AnimationAsset);
	}
	return 0;
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
