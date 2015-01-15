// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_AimComponent.h"
#include "TB_Character.h"
#include "TB_Weapon.h"
#include "TB_Library.h"

#include <algorithm>
#include <cmath>

void UTB_AimComponent::UpdateValues()
{
	Character = (ATB_Character *)GetAttachmentRootActor();
	Weapon = Character->Weapon;

	if (EnemyTarget && Weapon)
	{
		// Base Values
		FinalHitChance = Character->RangedAttackSkill;
		FinalDamage = Weapon->BaseDamage;

		// Range
		RangeToTarget = Character->GetDistanceTo(EnemyTarget);
		RangeHitModifier = Weapon->HitModifier(RangeToTarget);
		FinalHitChance += RangeHitModifier;
		RangeDamageModifier = Weapon->DamageModifier(RangeToTarget);
		FinalDamage += RangeDamageModifier;

		// Cover
		CoverHitModifier = CoverModifier(EnemyTarget);
		FinalHitChance += CoverHitModifier;
		CoverDamageModifier = 0;

		// Final sanity check
		FinalHitChance = std::max(0, FinalHitChance);
		if (FinalDamage < 0)
		{
			FinalDamage = 0;
		}
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

	if (Hidden < HitLocations.Num())
	{
		// reduce cover by half if we can see the enemy at all
		// a penalty above 60 makes an enemy virtually impossible to hit
		Hidden -= (Hidden / 3);
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

void UTB_AimComponent::HitLineTrace(FHitResult &OutHit)
{
	if (!EnemyTarget)
	{
		UE_LOG(TB_Log, Error, TEXT("HitLineTrace(): EnemyTarget == NULL"));
		return;
	}

	//find the hit locations
	TArray<FVector> HitLocations;
	EnemyTarget->GetHitLocations(HitLocations);

	// shuffle them and do line traces until we get a hit
	UTB_Library::Shuffle(HitLocations);

	FCollisionQueryParams Params;
	InitCollisionQueryParams(Params);
	FCollisionObjectQueryParams ObjectParams;
	FVector StartLocation = GetComponentLocation();
	
	for (auto HitLocation : HitLocations)
	{
		bool HitSomething = World->LineTraceSingle(OutHit, StartLocation, HitLocation, Params, ObjectParams);
		if (HitSomething && EnemyTarget->GetUniqueID() == OutHit.Actor->GetUniqueID())
		{
			return;
		}
	}
	UE_LOG(TB_Log, Error, TEXT("HitLineTrace(): Could not find a hitting line trace"));
}

bool UTB_AimComponent::MissLineTrace(FHitResult &OutHit)
{
	if (!EnemyTarget)
	{
		UE_LOG(TB_Log, Error, TEXT("MissLineTrace(): EnemyTarget == NULL"));
		return false;
	}

	//Pick a HitLocation at random
	TArray<FVector> HitLocations;
	EnemyTarget->GetHitLocations(HitLocations);
	UTB_Library::Shuffle(HitLocations);

	FCollisionQueryParams Params;
	InitCollisionQueryParams(Params);
	FCollisionObjectQueryParams ObjectParams;
	FVector StartLocation = GetComponentLocation();

	float Offset = 30;

	for (auto HitLocation : HitLocations)
	{
		bool HitSomething = World->LineTraceSingle(OutHit, StartLocation, HitLocation, Params, ObjectParams);
		if (HitSomething)
		{
			if (EnemyTarget->GetUniqueID() != OutHit.Actor->GetUniqueID())
			{
				//we've hit something we didn't aim for \o/
				return true;
			}
			else
			{
				//we need to adjust the trace so it misses the target

				// Create a new vector for the trace 
				FVector TraceVector = HitLocation - StartLocation;
				TraceVector.Normalize();
				TraceVector *= Weapon->MaxRange;

				// Rotate it by 0-5 deg in all directions
				FRotator Offset(FMath::FRand() * 1000, FMath::FRand() * 1000, 0);
				if (FMath::FRand() < 0.5)
				{
					Offset.Yaw *= -1;
				}
				if (FMath::FRand() < 0.5)
				{
					Offset.Pitch *= -1;
				}
				TraceVector = Offset.RotateVector(TraceVector);

				HitSomething = World->LineTraceSingle(OutHit, StartLocation, StartLocation + TraceVector, Params, ObjectParams);
				if (HitSomething && EnemyTarget->GetUniqueID() == OutHit.Actor->GetUniqueID())
				{
					//we're still hitting the target, pray that we'll miss it when we look at the other targetpoints
					continue;
				}
				else
				{
					return HitSomething;
				}

			}
		}
	}

	UE_LOG(TB_Log, Warning, TEXT("MissLineTrace(): Can't find a way to miss"));
	return false;
}
void UTB_AimComponent::InitCollisionQueryParams(FCollisionQueryParams &Params)
{
	// remember to set the physical material in phat for skeletal meshes
	// (the collision channels for the character might also need to be tuned)
	Params.bTraceComplex = true; 
	Params.bReturnPhysicalMaterial = true;

	// Ignore collisions with the aiming character and its weapon
	Params.AddIgnoredActor(Character);
	if (Weapon)
	{
		Params.AddIgnoredActor(Weapon);
	}
	if (EnemyTarget->Weapon)
	{
		Params.AddIgnoredActor(EnemyTarget->Weapon);
	}
}