// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimInstance.h"
#include "Templates/SharedPointer.h"
/*
I can't get forward declaration of enums to work with the UPROPERTIES macro so I include the header insted.
*/
#include "TB_Weapon.h"

#include "TB_AnimInstance.generated.h"

class ATB_Character;


/**
 * 
 */
USTRUCT(BlueprintType)
struct FTB_AnimSet
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	UAnimSequence *Idle = NULL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	UAnimSequence *Attack = NULL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	UAnimSequence *Reload = NULL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	UAnimSequence *Hurt = NULL;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	UAnimSequence *Die = NULL;
	// Driven by speed and direction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	UBlendSpace *IdleWalk = NULL;
};


UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class UE4_TURNBASED_API UTB_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	
public:
	UTB_AnimInstance(const FObjectInitializer & ObjectInitializer);
	void BlueprintInitializeAnimation() override;
	void BlueprintUpdateAnimation(float DeltaTimeX) override;

	UPROPERTY(BlueprintReadWrite, Category = "Default")
	ATB_Character *Character = NULL;
	UPROPERTY(BlueprintReadWrite, Category = "Default")
	ATB_Weapon *Weapon = NULL;

	UPROPERTY(BlueprintReadWrite, Category = "Animation Input")
	float Speed;
	UPROPERTY(BlueprintReadWrite, Category = "Animation Input")
	float Direction;
	UPROPERTY(BlueprintReadWrite, Category = "Animation Input")
	bool Alive;

	UPROPERTY(BlueprintReadWrite, Category = "Animation Input")
	ETB_WeaponAnimType WeaponAnimType;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Animation Sets")
	FTB_AnimSet StandingGunShoulderAnims;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Animation")
	float PlayAnimation(UAnimSequence *Animation);

	/* 
	Getters for CurrentAnimSet which UE won't let us expose.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animations")
	UAnimSequence *GetIdleAnim();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animations")
	UAnimSequence *GetAttackAnim();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animations")
	UAnimSequence *GetReloadAnim();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animations")
	UAnimSequence *GetHurtAnim();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animations")
	UAnimSequence *GetDieAnim();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Animations")
	UBlendSpace *GetIdleWalkBlendSpace();

private:
	FTB_AnimSet *CurrentAnimSet = &StandingGunShoulderAnims;
};
