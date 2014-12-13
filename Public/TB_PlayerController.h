// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TB_TeamController.h"
#include "TB_GameState.h"
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
	ATB_TeamController *TeamController = NULL;

	UFUNCTION(BlueprintCallable, Category = "Utilities")
	bool LineTraceFromScreenPos(float ScreenX, float ScreenY, FHitResult& WorldHit);

	/* User actions */
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "User Actions")
	void EndTurn();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "User Actions")
	void SelectNextCharacter();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "User Actions")
	void MoveCharacterTo(FVector Destination);

private:
	UPROPERTY()
	ATB_GameState *GameState = NULL;
};
