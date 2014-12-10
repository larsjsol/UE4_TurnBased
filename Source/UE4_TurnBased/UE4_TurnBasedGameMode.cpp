// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_PlayerController.h"
#include "UE4_TurnBasedGameMode.h"



AUE4_TurnBasedGameMode::AUE4_TurnBasedGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) 
{
	PlayerControllerClass = ATB_PlayerController::StaticClass();
}

