// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SceneComponent.h"
#include "TB_AimComponent.generated.h"

class ATB_Weapon;
class ATB_Character;


/**
 * 
 */
UCLASS()
class UE4_TURNBASED_API UTB_AimComponent : public USceneComponent
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	ATB_Weapon *Weapon = NULL;
	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	ATB_Character *Character = NULL;
	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	ATB_Character *EnemyTarget = NULL;

	/* Hit and damage modifiers */
	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	int32 RangeHitModifier = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	float RangeDamageModifier = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	int32 CoverHitModifier = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	float CoverDamageModifier = 0;

	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	float RangeToTarget = 0;
	
	/* Living enemies that this actor can see */
	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	TArray<ATB_Character *> VisibleEnemies;

	/* Final hit chance and expected damage */
	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	int32 FinalHitChance = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Aim")
	float FinalDamage = 0;

	/* Update all properties */
	UFUNCTION(BlueprintCallable, Category = "Aim")
	void UpdateValues();
	/* Find covermodifer for Target */
	UFUNCTION(BlueprintCallable, Category = "Aim")
	int32 CoverModifier(ATB_Character *Target);
	UFUNCTION(BlueprintCallable, Category = "Aim")
	void TargetNextEnemy();
	UFUNCTION(BlueprintCallable, Category = "Aim")
	void UpdateVisibleEnemies();
};
