// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
private:

};