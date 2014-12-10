// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Controller.h"
#include "TB_Character.h"
#include "TeamController.generated.h"

/**
 * 
 */
UCLASS()
class UE4_TURNBASED_API ATeamController : public AController
{
	GENERATED_BODY()

public:
	ATeamController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
	void PlayTurn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
	void EndTurn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
	void ActivateNextCharacter();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
	ATB_Character* GetActiveCharacter();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void RegisterCharacter(ATB_Character* NewCharacter);

	UPROPERTY(BlueprintReadWrite, Category = "Game")
	TArray<ATB_Character*> Characters;

	UPROPERTY(BlueprintReadWrite, Category = "Game")
	APlayerController* PlayerController = NULL;

	UPROPERTY(BlueprintReadWrite, Category = "Game")
	FName TeamName;

private:
	UPROPERTY()
	uint32 CurrentCharacterId = 0;

};

