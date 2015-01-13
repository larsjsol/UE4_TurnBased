// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "TB_WeaponFX.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class UE4_TURNBASED_API UTB_WeaponFX : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BluePrintNativeEvent, BlueprintPure, Category = "Effects")
	UParticleSystem *ImpactParticleSystem(EPhysicalSurface SurfaceType);
	
	UFUNCTION(BluePrintNativeEvent, BlueprintPure, Category = "Effects")
	USoundCue *ImpactSoundCue(EPhysicalSurface SurfaceType);
	
};