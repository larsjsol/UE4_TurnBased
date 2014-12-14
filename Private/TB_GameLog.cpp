// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_GameLog.h"

ATB_GameLog::ATB_GameLog(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer), LogLines()
{

}

void ATB_GameLog::Log(ETB_LogCategory Category, FText Text) 
{
	FTB_LogLine LogLine; 
	LogLine.TimeStamp = FDateTime::Now();
	LogLine.Category = Category;
	LogLine.Text = Text;

	LogLines.Add(LogLine);
}

void ATB_GameLog::Log(ETB_LogCategory Category, FString Text)
{
	Log(Category, FText::FromString(Text));
}

void ATB_GameLog::GetLine(int32 LineFromBack, FTB_LogLine &LogLine) {
	int32 i = (LogLines.Num() - 1) - LineFromBack;
	if (i >= 0)
	{
		LogLine = LogLines[i];
	}
}
