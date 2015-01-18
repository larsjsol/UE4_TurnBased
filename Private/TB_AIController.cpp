// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_AIController.h"
#include "TB_Character.h"
#include "TB_AimComponent.h"

void ATB_AIController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	ATB_Character *Pawn = (ATB_Character *)GetCharacter();
	Pawn->ClearBusy();
}

void ATB_AIController::DoSingleAction_Implementation()
{
	ATB_Character *Pawn = (ATB_Character *) GetCharacter();
	
	// reload if we're out of ammo
	if (Pawn->Weapon->Ammo == 0)
	{
		Pawn->Reload();
		return;
	}
	
	// if not, fire at an enemy
	Pawn->TargetEnemy();
	if (Pawn->AimComponent->EnemyTarget)
	{
		Pawn->InitiateAttack();
		return;
	}

	// waste an AP if we do not can't do anything
	Pawn->ActionPoints--;
	Pawn->ClearBusy();
}


