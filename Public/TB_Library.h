// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "TB_Library.generated.h"

/**
 * 
 */
UCLASS(MinimalAPI)
class UTB_Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	/* 
	Returns the integer as text with a plus or minus sign in front.
	
	Returns an empty string if the integer is zero
	*/
	UFUNCTION(BlueprintPure, Category = "Formatting")
	static void IntAsModifierText(int32 Integer, FText &FormattedText);
	UFUNCTION(BlueprintPure, Category = "Formatting")
	static void FloatAsModifierText(float Float, FText &FormattedText);
	
};
