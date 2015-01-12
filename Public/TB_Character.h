// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "TB_Weapon.h"

#include "TB_Character.generated.h"

/**
 * 
 */

class ATB_TeamController;
class UTB_AnimInstance;
class UTB_Name;
class UTB_AimComponent;

UENUM(BlueprintType)
enum class ETB_Gender : uint8
{
	VE_None                 UMETA(DisplayName = "None / Unimportant"),
	VE_Male                 UMETA(DisplayName = "Male"),
	VE_Female               UMETA(DisplayName = "Female")
};

UCLASS()
class UE4_TURNBASED_API ATB_Character : public ACharacter
{
	GENERATED_BODY()
public:
	ATB_Character(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void PostInitProperties() override;
	virtual float TakeDamage(float Damage, FDamageEvent const &DamageEvent, AController *EventInstigator, AActor *DamageCauser) override;

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
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Base Abilities")
	int32 RangedAttackSkill = 50;

	/* Overhead and FP camera */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	USpringArmComponent *OverheadSpringArm = NULL;
	UPROPERTY(BlueprintReadWrite, Category = "Default")
	ACameraActor *OverheadCamera = NULL;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	USpringArmComponent *FPSpringArm = NULL;
	UPROPERTY(BlueprintReadWrite, Category = "Default")
	ACameraActor *FPCamera = NULL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	UTB_AimComponent *AimComponent = NULL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flavour")
	UTB_Name *CharacterName = NULL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flavour")
	FText SpeciesName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flavour")
	FName TeamName = "Team AI";

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Equipment")
	UClass *WeaponClass = NULL;

	UPROPERTY(BlueprintReadWrite, Category = "Equipment")
	ATB_Weapon *Weapon = NULL;

	UPROPERTY(BlueprintReadOnly, Category = "Equipment")
	UTB_AnimInstance *AnimInstance = NULL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flavour")
	UClass *NameClass = NULL;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Flavour")
	ETB_Gender Gender = ETB_Gender::VE_None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game")
	bool HumanControlled = false;

	/* Called right before the turn starts */
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Events")
	void OnBeginTurn();
	/* Called when the turn ends */
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Events")
	void OnEndTurn();
	/* Called when the turn ends */
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Events")
	void OnSelected();

	//Is the character performing an action right now?
	UFUNCTION(BluePrintNativeEvent, BlueprintPure, Category = "Game")
	bool IsBusy();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void SetBusy(float BusyDuration);
	/* Called when an action is finished, might call PlayTurn() */
	UFUNCTION(BlueprintCallable, Category = "Game")
	void ClearBusy();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void Reload();

	/* 
	Start the attack animatios and set the character as busy.
	The AnimInstace expected to do a callback to Attack() at an apropriate time in 
	order to update the game state.
	*/
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void InitiateAttack();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void Attack();

	/* Navigation */
	UFUNCTION(BluePrintNativeEvent, BlueprintPure, Category = "Navigation")
	bool CanMoveTo(FVector Destination);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Navigation")
	void MoveTo(FVector Destination);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Navigation")
	void LookAt(AActor *Target);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void TargetEnemy();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void TargetNextEnemy();

	/* 
	Get a list of locations that is suitable for determening line-of-sight
	and cover for this character.

	Default behavior is to return the position of all sockets
	*/
	UFUNCTION(BluePrintNativeEvent, BlueprintPure, Category = "Vision")
	void GetHitLocations(TArray<FVector> &WorldLocations);

	/* Play a single turn using the AI */
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Game")
	void PlayTurn();

private:
	bool Busy = false;

	UPROPERTY()
	ATB_TeamController *TeamController = NULL;
};
