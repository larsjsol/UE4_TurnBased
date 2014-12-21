// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "TB_Weapon.h"

#include "TB_Character.generated.h"

/**
 * 
 */

class ATB_TeamController;

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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game")
	int32 MaxActionPoints = 2;

	/* Hit Points */
	UPROPERTY(BlueprintReadWrite, Category = "Game")
	int32 HitPoints = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game")
	int32 MaxHitPoints = 10;

	// How far the actor can move when spending one AP
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game")
	float Movement = 1000;

	/* Skills */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game")
	int32 RangedAttackSkill = 50;

	/* Overhead and FP camera */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	USpringArmComponent *OverheadSpringArm;
	UPROPERTY(BlueprintReadWrite, Category = "Default")
	ACameraActor *OverheadCamera;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	USpringArmComponent *FPSpringArm;
	UPROPERTY(BlueprintReadWrite, Category = "Default")
	ACameraActor *FPCamera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flavour")
	FText CharacterName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flavour")
	FText SpeciesName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flavour")
	FName TeamName = "Team AI";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	UClass *WeaponClass = NULL;

	UPROPERTY(BlueprintReadWrite, Category = "Equipment")
	ATB_Weapon *Weapon = NULL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	TArray<FTB_WeaponAnimSet> WeaponHandlingAnimations;

	/* Which enemy is targeted */
	UPROPERTY(BlueprintReadWrite, Category = "Game")
	ATB_Character *EnemyTarget = NULL;

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game ")
	void PrepareForNextTurn();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void PrepareForThisTurn();

	//Is the character performing an action right now?
	UFUNCTION(BluePrintNativeEvent, BlueprintPure, Category = "Game")
	bool IsBusy();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void SetBusy(float BusyDuration);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void ClearBusy();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void Reload();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	float PlayAnimation(UAnimSequence *Animation);

	/* Navigation */
	UFUNCTION(BluePrintNativeEvent, BlueprintPure, Category = "Navigation")
	bool CanMoveTo(FVector Destination);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Navigation")
	void MoveTo(FVector Destination);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Navigation")
	void LookAt(AActor *Target);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void TargetNextEnemy();

	/* Hit chance after applying all modifiers */
	UFUNCTION(BluePrintNativeEvent, BlueprintPure, Category = "Game")
	int32 HitChance();

	/* Expected damage dealt after all modifiers */
	UFUNCTION(BluePrintNativeEvent, BlueprintPure, Category = "Game")
	int32 Damage();

private:
	bool Busy = false;

	UPROPERTY()
	ATB_TeamController *TeamController;
};
