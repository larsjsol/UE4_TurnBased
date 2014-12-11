// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "TB_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class UE4_TURNBASED_API ATB_GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ATB_GameMode(const FObjectInitializer& ObjectInitializer);
};
