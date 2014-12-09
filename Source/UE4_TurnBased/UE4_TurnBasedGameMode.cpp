// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "UE4_TurnBasedGameMode.h"


/*
* As seen on: https://wiki.unrealengine.com/Game_Mode,_Linking_to_Player_Controller_Blueprint
*
*/
AUE4_TurnBasedGameMode::AUE4_TurnBasedGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) 
{
	//backup
	PlayerControllerClass = APlayerController::StaticClass();

	//Blueprinted Version, relies on the "Copy Reference" asset path you get from Editor
	static ConstructorHelpers::FObjectFinder<UBlueprint> VictoryPCOb(TEXT("Blueprint'/Game/TB_PlayerController.TB_PlayerController'"));
	if (VictoryPCOb.Object != NULL)
	{
		PlayerControllerClass = (UClass*)VictoryPCOb.Object->GeneratedClass;
	}
}

