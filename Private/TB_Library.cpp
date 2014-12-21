// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_Library.h"

void UTB_Library::IntAsModifierText(int32 Integer, FText &FormattedText)
{
	
	if (Integer > 0)
	{
		FormattedText = FText::FromString(FString(TEXT("+")) += FString::FromInt(Integer));
	}
	else if (Integer < 0)
	{
		FormattedText = FText::AsNumber(Integer);
	}
	else
	{
		FormattedText = FText::GetEmpty();
	}
}


