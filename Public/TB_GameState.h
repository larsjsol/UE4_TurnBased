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

	UFUNCTION(BlueprintCallable, Category="Teams")
	void EndTurn();

	UFUNCTION(BlueprintCallable, Category = "Teams")
	void RegisterTeamController(ATB_TeamController* TB_TeamController);

	UFUNCTION(BlueprintCallable, Category = "Teams")
	ATB_TeamController* GetTeamController(FName TeamName);

	UFUNCTION(BlueprintCallable, Category = "Teams")
	const TArray<ATB_TeamController *>& GetTeamControllers();

	void BeginPlay();
private:
	TArray<ATB_TeamController*> TeamControllers;
	ATB_TeamController *AiTeamController = NULL;
	uint32 CurrentTeamId = 0;
};
