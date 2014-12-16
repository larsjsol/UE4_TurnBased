// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "TB_Weapon.h"

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

	virtual void BeginPlay() override;

	/* Action Points */
	UPROPERTY(BlueprintReadWrite, Category = "Game")
	int32 ActionPoints = 2;
	UPROPERTY(BlueprintReadWrite, Category = "Game")
	int32 MaxActionPoints = 2;

	/* Hit Points */
	UPROPERTY(BlueprintReadWrite, Category = "Game")
	int32 HitPoints = 10;
	UPROPERTY(BlueprintReadWrite, Category = "Game")
	int32 MaxHitPoints = 10;

	// How far the actor can move when spending one AP
	UPROPERTY(BlueprintReadWrite, Category = "Game")
	float Movement = 1000;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flavour")
	FName Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flavour")
	FName TeamName = "Team AI";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	UClass *WeaponClass = NULL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	ATB_Weapon *Weapon = NULL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	TArray<FTB_WeaponAnimSet> WeaponHandlingAnimations;

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game ")
	void PrepareForNextTurn();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void PrepareForThisTurn();

	//Is the character performing an action right now?
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	bool IsBusy();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void SetBusy(float BusyDuration);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void ClearBusy();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	float PlayAnimation(UAnimationAsset *AnimationAsset);

	/* Navigation */
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Navigation")
	bool CanMoveTo(FVector Destination);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Navigation")
	void MoveTo(FVector Destination);

private:
	bool Busy = false;
};
