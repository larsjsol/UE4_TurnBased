// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Controller.h"
#include "TB_Character.h"
#include "TB_TeamController.generated.h"

/**
 * 
 */
class ATB_PlayerController;

UCLASS(Blueprintable, BlueprintType)
class UE4_TURNBASED_API ATB_TeamController : public AController
{
	GENERATED_BODY()

public:
	ATB_TeamController(const FObjectInitializer& ObjectInitializer);

	void BeginPlay();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
	void PlayTurn();

	UFUNCTION(BlueprintCallable, Category = "Game")
	void CharacterPlayTurn();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Game")
	void EndTurn();

	/* Get the next character in the team, does not call OnActivation()*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Team Members")
	void NextCharacter();

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

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Target Selection")
	void RegisterEnemy(ATB_Character* Enemy);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Target Selection")
	void GetKnownEnemies(TArray<ATB_Character*>& Enemies);

	UPROPERTY(BlueprintReadWrite, Category = "Team Members")
	TArray<ATB_Character*> Characters;

	UPROPERTY(BlueprintReadWrite, Category = "Game")
	ATB_PlayerController* PlayerController = NULL;

	UPROPERTY(BlueprintReadWrite, Category = "Game")
	FName TeamName;

	UPROPERTY(BlueprintReadOnly, Category = "Game")
	bool MyTurn = false;

private:
	UPROPERTY()
	uint32 CurrentCharacterId = 0;

	UPROPERTY()
	TArray<ATB_Character *> KnownEnemies;
};

