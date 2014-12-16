// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_Weapon.h"
#include "TB_Character.h"

ATB_Weapon::ATB_Weapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	/* Add a static and skeletal mesh component */
	RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("SceneComponent"));
	StaticMeshComponent = ObjectInitializer.CreateAbstractDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMesh"));
	StaticMeshComponent->AttachParent = RootComponent;
	SkeletalMeshComponent = ObjectInitializer.CreateAbstractDefaultSubobject<USkeletalMeshComponent>(this, TEXT("SkeletalMesh"));
	SkeletalMeshComponent->AttachParent = RootComponent;
}

void ATB_Weapon::Equip_Implementation(ATB_Character *EquippedBy) {
	EquippedBy = EquippedBy;
	AttachRootComponentTo(EquippedBy->Mesh, SocketName);
}

void ATB_Weapon::UnEquip_Implementation()
{
	EquippedBy = NULL;
}

int32 ATB_Weapon::HitModifier_Implementation(float range)
{
	return 0;
}

int32 ATB_Weapon::DamageModifier_Implementation(float range)
{
	return 0;
}

float ATB_Weapon::PlayAnimation(UAnimationAsset *AnimationAsset) {
	if (SkeletalMeshComponent && AnimationAsset)
	{
		SkeletalMeshComponent->PlayAnimation(AnimationAsset, false);
		UAnimInstance *AnimInstance = SkeletalMeshComponent->GetAnimInstance();
		return AnimInstance->GetAnimAssetPlayerLength(AnimationAsset);
	}
	return 0;
}

float ATB_Weapon::Attack_Implementation()
{
	return PlayAnimation(AnimSet.Attack);
}

float ATB_Weapon::Reload_Implementation()
{
	Ammo = MaxAmmo;
	return PlayAnimation(AnimSet.Reload);
}

