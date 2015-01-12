// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "TB_AIController.generated.h"

/**
 * 
 */
UCLASS()
class UE4_TURNBASED_API ATB_AIController : public AAIController
{
	GENERATED_BODY()

public:
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Ai")
	void DoSingleAction();
};
