// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TB_Weapon.generated.h"

class ATB_Character;
class UTB_WeaponFX;

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTB_WeaponAnimSet
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	UAnimSequence *Reload = NULL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	UAnimSequence *ReloadKneel = NULL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	UAnimSequence *ReloadProne = NULL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimSequence *Attack = NULL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimSequence *AttackKneel = NULL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	UAnimSequence *AttackProne = NULL;
};

UENUM(BlueprintType)
enum class ETB_WeaponAnimType : uint8
{
	VE_GunShoulder	= 0			UMETA(DisplayName = "Gun Shoulder"),
	VE_GunHip		= 1			UMETA(DisplayName = "Gun Hip"),
	VE_Pistol		= 2			UMETA(DisplayName = "Pistol"),
	VE_Length					UMETA(Hidden)
};

UCLASS()
class UE4_TURNBASED_API ATB_Weapon : public AActor
{
	GENERATED_BODY()
public:
	ATB_Weapon(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;

	/* Range */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
	float MinRange = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
	float MaxRange = 0;
	/* Damage */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
	int32 BaseDamage = 0;

	/* Ammo */
	UPROPERTY(BlueprintReadWrite, Category = "Game Rules")
	int32 Ammo = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Rules")
	int32 MaxAmmo = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flavour")
	FText ModelName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	FName SocketName = "RightHand";

	/* Components */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	UStaticMeshComponent *StaticMeshComponent;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	USkeletalMeshComponent *SkeletalMeshComponent;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	ETB_WeaponAnimType AnimType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation")
	FTB_WeaponAnimSet AnimSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Effects")
	UClass *WeaponFXClass = NULL;
	UPROPERTY(BlueprintReadOnly, Category = "Effects")
	UTB_WeaponFX *WeaponFX = NULL;

	UFUNCTION(BluePrintNativeEvent, BlueprintPure, Category = "Game Rules")
	int32 HitModifier(float range);
	UFUNCTION(BluePrintNativeEvent, BluePrintPure, Category = "Game Rules")
	int32 DamageModifier(float range);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game Rules")
	void Equip(ATB_Character *EquippedBy);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game Rules")
	void UnEquip();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	float PlayAnimation(UAnimSequence *Animation);

	/*
		Start the attack animation and returns its duration
	*/
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Animation")
	float Attack();

	/* 
		Update Ammo and start the reload animation (if any) 
		Returns the duration of the reload animation	
	*/
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Animation")
	float Reload();

private:
	ATB_Character *EquippedBy = NULL;
};
