// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Controller.h"
#include "TB_Character.h"
#include "TB_TeamController.generated.h"

/**
 * 
 */
UCLASS()
class UE4_TURNBASED_API ATB_TeamController : public AController
{
	GENERATED_BODY()

public:
	ATB_TeamController(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
	void PlayTurn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
	void EndTurn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Team Members")
	void ActivateNextCharacter();

	/* Select the next living character, no effect if everyone is dead */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Team Members")
	void ActivateNextLivingCharacter();

	/* 
	Select the next living character with remaining APs, 
	no effect if no chars fit match criteria
	*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Team Members")
	void ActivateNextCharacterThatCanAct();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Team Members")
	ATB_Character* GetActiveCharacter();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Team Members")
	void RegisterCharacter(ATB_Character* NewCharacter);

	UPROPERTY(BlueprintReadWrite, Category = "Team Members")
	TArray<ATB_Character*> Characters;

	UPROPERTY(BlueprintReadWrite, Category = "Game")
	APlayerController* PlayerController = NULL;

	UPROPERTY(BlueprintReadWrite, Category = "Game")
	FName TeamName;

private:
	UPROPERTY()
	uint32 CurrentCharacterId = 0;

};

