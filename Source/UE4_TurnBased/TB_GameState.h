// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TeamController.h"
#include "GameFramework/GameState.h"
#include "TB_GameState.generated.h"


/**
 * 
 */
UCLASS()
class UE4_TURNBASED_API ATB_GameState : public AGameState
{
	GENERATED_BODY()

public:	
	ATB_GameState(const FObjectInitializer& ObjectInitializer);
	
	UPROPERTY(BlueprintReadOnly, Category="Game")
	int32 Turn;

	UFUNCTION(BlueprintCallable, Category="Game")
	void EndTurn();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void RegisterTeamController(ATeamController* TeamController);

	UFUNCTION(BlueprintCallable, Category = "Game")
	ATeamController* GetTeamController(FName TeamName);

	void BeginPlay();
private:
	TMap<FName, ATeamController*> TeamControllers;
	ATeamController *AiTeamController = NULL;
};
