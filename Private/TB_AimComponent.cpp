// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_AimComponent.h"
#include "TB_Character.h"
#include "TB_Weapon.h"

#include <algorithm>

void UTB_AimComponent::UpdateValues()
{
	Character = (ATB_Character *)GetAttachmentRootActor();
	Weapon = Character->Weapon;

	if (EnemyTarget && Weapon)
	{
		FinalHitChance = Character->RangedAttackSkill;
		FinalDamage = Weapon->BaseDamage;
		
		RangeToTarget = Character->GetDistanceTo(EnemyTarget);
		RangeHitModifier = Weapon->HitModifier(RangeToTarget);
		FinalHitChance += RangeHitModifier;
		RangeDamageModifier = Weapon->DamageModifier(RangeToTarget);
		FinalDamage += RangeDamageModifier;

		CoverHitModifier = CoverModifier(EnemyTarget);
		FinalHitChance += CoverHitModifier;
		CoverDamageModifier = 0; 
	}
	else
	{
		RangeToTarget = 0;

		RangeHitModifier = 0;
		RangeDamageModifier = 0;
		CoverHitModifier = 0;
		CoverDamageModifier = 0;

		FinalHitChance = 0;
		FinalDamage = 0;
	}
}

int32 UTB_AimComponent::CoverModifier(ATB_Character *Target)
{
	UWorld *World = GetWorld();

	// Ignore collisions with the aiming character and its weapon
	FCollisionQueryParams Params;
	Params.bTraceComplex = true;
	Params.AddIgnoredActor(Character);
	if (Weapon)
	{
		Params.AddIgnoredActor(Weapon);
	}
	if (Target->Weapon)
	{
		Params.AddIgnoredActor(Target->Weapon);
	}

	// uncomment to draw debug lines
	/*
	FName TraceTag("CoverTrace");
	World->DebugDrawTraceTag = TraceTag;
	Params.TraceTag = TraceTag;
	*/
	FCollisionObjectQueryParams ObjectParams;

	FHitResult HitResult;
	TArray<FVector> HitLocations;
	Target->GetHitLocations(HitLocations);
	FVector StartLocation = GetComponentLocation();

	int Hidden = 0;
	for (auto HitLocation : HitLocations)
	{
		bool HitSomething = World->LineTraceSingle(HitResult, StartLocation, HitLocation, Params, ObjectParams);
		if (HitSomething && Target->GetUniqueID() != HitResult.Actor->GetUniqueID())
		{
			Hidden++;
		}
	}

	return (Hidden * -100) / std::max(HitLocations.Num(), 1);
}

void UTB_AimComponent::TargetNextEnemy()
{

	if (VisibleEnemies.Num())
	{
		int32 Index = VisibleEnemies.Find(EnemyTarget); //INDEX_NONE (-1) if not found
		Index = (Index + 1) % VisibleEnemies.Num();
		EnemyTarget = VisibleEnemies[Index];
	}
	else
	{
		EnemyTarget = NULL;
	}
}

void UTB_AimComponent::UpdateVisibleEnemies()
{
	VisibleEnemies.Empty();
	Character = (ATB_Character *)GetAttachmentRootActor();
	Weapon = Character->Weapon;

	bool EnemyTargetVisible = false;
	
	for (TActorIterator<ATB_Character> Iter(GetWorld(), ATB_Character::StaticClass()); Iter; ++Iter)
	{
		ATB_Character *Enemy = *Iter;
		if (Enemy->HitPoints > 0 && 
			Enemy->TeamName != Character->TeamName &&
			CoverModifier(Enemy) > -100)
		{
			VisibleEnemies.Add(Enemy);
			if (Enemy == EnemyTarget)
			{
				EnemyTargetVisible = true;
			}
		}
	}

	if (!EnemyTargetVisible)
	{
		EnemyTarget = NULL;
	}
}
