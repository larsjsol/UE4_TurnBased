// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TeamController.h"
#include "GameFramework/PlayerController.h"
#include "TB_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UE4_TURNBASED_API ATB_PlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATB_PlayerController(const FObjectInitializer& ObjectInitializer);

	void BeginPlay();

	UPROPERTY(BlueprintReadWrite, Category = "Controller")
	ATeamController *TeamController = NULL;	
};
