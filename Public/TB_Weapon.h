// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TB_Weapon.generated.h"

class ATB_Character;

/**
 * 
 */
UENUM(BlueprintType)
enum class ETB_WeaponAnimType : uint8
{
	VE_GunShoulder		UMETA(DisplayName = "Gun Shoulder"),
	VE_GunHip           UMETA(DisplayName = "Gun Hip"),
	VE_Pistol           UMETA(DisplayName = "Pistol"),
};

UCLASS()
class UE4_TURNBASED_API ATB_Weapon : public AActor
{
	GENERATED_BODY()
public:
	ATB_Weapon(const FObjectInitializer& ObjectInitializer);

	/* Range */
	UPROPERTY(BlueprintReadWrite, Category = "Game Rules")
	float MinRange = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Game Rules")
	float MaxRange = 0;
	/* Damage */
	UPROPERTY(BlueprintReadWrite, Category = "Game Rules")
	int32 BaseDamage = 0;
	/* Ammo */
	UPROPERTY(BlueprintReadWrite, Category = "Game Rules")
	int32 Ammo = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Game Rules")
	int32 MaxAmmo = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Flavour")
	FText ModelName;
	UPROPERTY(BlueprintReadWrite, Category = "Flavour")
	TArray<ETB_WeaponAnimType> AnimationTypes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	FName SocketName = "RightHand";

	/* Components */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	UStaticMeshComponent *StaticMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	USkeletalMeshComponent *SkeletalMeshComponent;

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game Rules")
	int32 HitModifier(float range);
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game Rules")
	int32 DamageModifier(float range);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game Rules")
	void Equip(ATB_Character *EquippedBy);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game Rules")
	void UnEquip();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game Rules")
	void Attack(ATB_Character *Target);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game Rules")
	void Reload();



private:
	ATB_Character *EquippedBy = NULL;
};
