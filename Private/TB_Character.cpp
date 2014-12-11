// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_Character.h"


void ATB_Character::PrepareForNextTurn_Implementation()
{
	ActionPoints = MaxActionPoints;
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

