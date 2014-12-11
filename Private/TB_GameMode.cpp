// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_PlayerController.h"
#include "TB_GameMode.h"



ATB_GameMode::ATB_GameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) 
{
	PlayerControllerClass = ATB_PlayerController::StaticClass();
}

