// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Controller.h"
#include "TeamController.generated.h"

/**
 * 
 */
UCLASS()
class UE4_TURNBASED_API ATeamController : public AController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "Game")
	void PlayTurn();

	UFUNCTION(BlueprintNativeEvent, Category = "Game")
	void EndTurn();

	UFUNCTION(BlueprintNativeEvent, Category = "Game")
	void ActivateNextCharacter();

	UFUNCTION(BlueprintNativeEvent, Category = "Game")
	ACharacter* GetActiveCharacter();

	UFUNCTION(BluePrintNativeEvent, Category = "Game")
	void AddCharacter(ACharacter* NewCharacter);

	UPROPERTY(BlueprintReadWrite, Category = "Game")
	TArray<ACharacter*> Characters;

	UPROPERTY(BlueprintReadWrite, Category = "Game")
	APlayerController* PlayerController = NULL;

private:
	UPROPERTY()
	uint32 CurrentCharacterId = 0;

};
