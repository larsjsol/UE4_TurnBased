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

	void BeginPlay();

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

	UPROPERTY(BlueprintReadWrite, Category = "Game Rules")
	float Movement = 1000; //10m

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flavour")
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flavour")
	FName TeamName;

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game Rules")
	void PrepareForNextTurn();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game Rules")
	void PrepareForThisTurn();

	/* Is character performing an action right now? */
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	bool IsBusy();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void SetBusy(float BusyDuration);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void ClearBusy();

	/* Navigation */
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Navigation")
	bool CanMoveTo(FVector Destination);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Navigation")
	void MoveTo(FVector Destination);

private:
	bool Busy = false;
};
