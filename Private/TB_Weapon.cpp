// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_Weapon.h"
#include "TB_WeaponFX.h"
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

void ATB_Weapon::BeginPlay()
{
	Ammo = MaxAmmo;

	if (WeaponFXClass)
	{
		WeaponFX = NewObject<UTB_WeaponFX>(this, WeaponFXClass);
	}
}

void ATB_Weapon::Equip_Implementation(ATB_Character *EquippedBy)
{
	this->EquippedBy = EquippedBy;
	AttachRootComponentTo(EquippedBy->Mesh, SocketName);
}

void ATB_Weapon::UnEquip_Implementation()
{
	EquippedBy = NULL;
}

int32 ATB_Weapon::HitModifier_Implementation(float range)
{
	/* 
	default to a small penalty of -2 per 10 meters on the account 
	of "things getting smaller as they are further away"
	*/
	return range / -500;
}

int32 ATB_Weapon::DamageModifier_Implementation(float range)
{
	return 0;
}

float ATB_Weapon::PlayAnimation(UAnimSequence *Animation) {
	if (SkeletalMeshComponent && Animation)
	{
		SkeletalMeshComponent->PlayAnimation(Animation, false);
		UAnimInstance *AnimInstance = SkeletalMeshComponent->GetAnimInstance();
		return AnimInstance->GetAnimAssetPlayerLength(Animation);
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

void ATB_Weapon::SpawnImpactEffects_Implementation(const FHitResult &HitResult)
{	
	if (WeaponFX)
	{
		if (HitResult.PhysMaterial.IsValid()) {
			EPhysicalSurface Surface = HitResult.PhysMaterial->SurfaceType;
			UE_LOG(TB_Log, Error, TEXT("PhysMaterial: %s"), *HitResult.PhysMaterial->StaticClass()->GetName());
			UE_LOG(TB_Log, Error, TEXT("SurfaceType: %i"), (int) Surface);

			USoundCue *SoundCue = WeaponFX->ImpactSoundCue(Surface);
			UGameplayStatics::PlaySoundAtLocation(this, SoundCue, HitResult.ImpactPoint);
		}
	}
}


