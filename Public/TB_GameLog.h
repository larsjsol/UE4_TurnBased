// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DateTime.h"

#include "GameFramework/Actor.h"
#include "TB_GameLog.generated.h"

/**
 * 
 */
UENUM(BlueprintType)

enum class ETB_LogCategory : uint8
{
	VE_TurnClock		UMETA(DisplayName = "Turn Clock"),
	VE_CharacterEvent	UMETA(DisplayName = "Character Event"),
	VE_CharacterSpeech	UMETA(DIsplayName = "Character Speech"),
};

USTRUCT(BlueprintType)
struct FTB_LogLine
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Log")
	FDateTime TimeStamp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Log")
	ETB_LogCategory Category;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Log")
	FText Text;
};

UCLASS()
class UE4_TURNBASED_API ATB_GameLog : public AActor
{
	GENERATED_BODY()
public:
	ATB_GameLog(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "Log")
	void Log(ETB_LogCategory Category, FText Text);

	void Log(ETB_LogCategory Category, FString Text);

	UFUNCTION(BlueprintCallable, Category = "Log")
	void GetLine(int32 LineFromBack, FTB_LogLine &LogLine);
	
private:
	TArray<FTB_LogLine> LogLines;
};





