// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TB_TeamController.h"
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
	void RegisterTB_TeamController(ATB_TeamController* TB_TeamController);

	UFUNCTION(BlueprintCallable, Category = "Game")
	ATB_TeamController* GetTB_TeamController(FName TeamName);

	void BeginPlay();
private:
	TArray<ATB_TeamController*> TB_TeamControllers;
	ATB_TeamController *AiTB_TeamController = NULL;
	uint32 CurrentTeamId = 0;
};
