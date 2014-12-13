// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "TB_Character.generated.h"

/**
 * 
 */
UCLASS()
class UE4_TURNBASED_API ATB_Character : public ACharacter
{
	GENERATED_BODY()
public:
	ATB_Character(const FObjectInitializer& ObjectInitializer);

	/* Action Points */
	UPROPERTY(BlueprintReadWrite, Category = "Game Rules")
	int32 ActionPoints = 2;
	UPROPERTY(BlueprintReadWrite, Category = "Game Rules")
	int32 MaxActionPoints = 2;

	/* Hit Points */
	UPROPERTY(BlueprintReadWrite, Category = "Game Rules")
	int32 HitPoints = 10;
	UPROPERTY(BlueprintReadWrite, Category = "Game Rules")
	int32 MaxHitPoints = 10;

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game Rules")
	void PrepareForNextTurn();

	/* Is character performing an action right now? */
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	bool IsBusy();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void SetBusy(float BusyDuration);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void ClearBusy();


private:
	bool Busy = false;
};
