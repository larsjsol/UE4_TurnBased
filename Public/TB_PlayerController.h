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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Controller")
	UClass *TeamControllerClass = NULL;
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
	void CharacterMoveTo(FVector Destination);

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "User Actions")
	void CharacterReload();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "User Actions")
	void CharacterAttack();

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "User Actions")
	void CharacterTargetNextEnemy();

	/* Events */

	/* Fired when the TeamController stars a new turn */
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Events")
	void OnBeginTurn();

	/* Fired when the TeamController ends a turn */
	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Events")
	void OnEndTurn();

private:
	UPROPERTY()
	ATB_GameState *GameState = NULL;
};
