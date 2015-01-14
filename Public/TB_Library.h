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
	
	template <typename T>
	static void Shuffle(TArray<T> &Array);
};


// Template member functions should be put in the header file
template <typename T>
void UTB_Library::Shuffle(TArray<T> &Array)
{
	// https://answers.unrealengine.com/questions/58340/how-to-shuffle-a-tarray.html
	Array.Sort([](T Item1, T Item2) {
		return FMath::FRand() < 0.5f;
	});
}