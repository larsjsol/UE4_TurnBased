// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_AnimInstance.h"

#include "TB_Character.h"
#include "TB_Weapon.h"

UTB_AnimInstance::UTB_AnimInstance(const FObjectInitializer & ObjectInitializer)
	: Super(ObjectInitializer)
{

}

void UTB_AnimInstance::BlueprintInitializeAnimation()
{
	Super::BlueprintInitializeAnimation();

	Character = (ATB_Character *)TryGetPawnOwner();
}

void UTB_AnimInstance::BlueprintUpdateAnimation(float DeltaTimeX)
{
	Super::BlueprintUpdateAnimation(DeltaTimeX);

	if (!Character)
	{
		return;
	}

	// update variables
	FVector Velocity = Character->GetVelocity();
	FRotator Rotation = Character->GetActorRotation();
	Speed = Velocity.Size();
	Direction = CalculateDirection(Velocity, Rotation);

	Weapon = Character->Weapon;
	if (Weapon) {
		WeaponAnimType = Weapon->AnimType;
	}

	// set the current animation set
	switch (WeaponAnimType)
	{
	case ETB_WeaponAnimType::VE_GunShoulder:
	default:
		CurrentAnimSet = &StandingGunShoulderAnims;
	}
}


UAnimSequence *UTB_AnimInstance::GetIdleAnim()
{
	return CurrentAnimSet->Idle;
}

UAnimSequence *UTB_AnimInstance::GetAttackAnim()
{
	return CurrentAnimSet->Attack;
}

UAnimSequence *UTB_AnimInstance::GetReloadAnim()
{
	return CurrentAnimSet->Reload;
}

UAnimSequence *UTB_AnimInstance::GetHurtAnim()
{
	return CurrentAnimSet->Hurt;
}

UAnimSequence *UTB_AnimInstance::GetDieAnim()
{
	return CurrentAnimSet->Die;
}

UBlendSpace *UTB_AnimInstance::GetIdleWalkBlendSpace()
{
	return CurrentAnimSet->IdleWalk;
}
