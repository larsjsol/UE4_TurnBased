// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_Library.h"

#define LOCTEXT_NAMESPACE "TB_Loctext" 

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

void UTB_Library::FloatAsModifierText(float Float, FText &FormattedText)
{

	if (Float > 0)
	{
		FormattedText = FText::Format(LOCTEXT("Number as modifier", "+{0}"), FText::AsNumber(Float));
	}
	else if (Float < 0)
	{
		FormattedText = FText::AsNumber(Float);
	}
	else
	{
		FormattedText = FText::GetEmpty();
	}
}

#undef LOCTEXT_NAMESPACE 