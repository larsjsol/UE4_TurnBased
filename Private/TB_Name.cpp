// Fill out your copyright notice in the Description page of Project Settings.

#include "UE4_TurnBased.h"
#include "TB_Name.h"

//init static members
TArray<FString> UTB_Name::MaleFirstNames = TArray<FString>();
TArray<FString> UTB_Name::FemaleFirstNames = TArray<FString>();
TArray<FString> UTB_Name::LastNames = TArray<FString>();

void UTB_Name::PostInitProperties()
{
	Super::PostInitProperties();

	//Load namelists if they are not already loaded
	Mutex.Lock();
	if (!MaleFirstNameFile.IsEmpty() && MaleFirstNames.Num() == 0)
	{
		LoadNames(MaleFirstNameFile, MaleFirstNames);
	}
	if (!FemaleFirstNameFile.IsEmpty() && FemaleFirstNames.Num() == 0)
	{
		LoadNames(FemaleFirstNameFile, FemaleFirstNames);
	}
	if (!LastNameFile.IsEmpty() && LastNames.Num() == 0)
	{
		LoadNames(LastNameFile, LastNames);
	}
	Mutex.Unlock();

	FGenericPlatformMath::RandInit(GetUniqueID()); // use a different seed for each thread
}

void UTB_Name::Generate_Implementation(ETB_Gender Gender)
{
	switch (Gender)
	{
	case ETB_Gender::VE_Male:
		PickRandomName(MaleFirstNames, FirstName);
		break;
	case ETB_Gender::VE_Female:
		PickRandomName(FemaleFirstNames, FirstName);
		break;
	case ETB_Gender::VE_None:
		PickRandomName(MaleFirstNames, FirstName); //i don't know.... 
		break;
	};

	PickRandomName(LastNames, LastName);
}

FString UTB_Name::ToString()
{
	return FString::Printf(TEXT("%s %s"), *FirstName, *LastName);
}

FText UTB_Name::ToText()
{
	return FText::FromString(ToString());
}

void UTB_Name::LoadNames(FString &NameFile, TArray<FString> &Names)
{
	FString FullPath = FPaths::GameContentDir() + NameFile;
	UE_LOG(TB_Log, Log, TEXT("Loading names from %s"), *FullPath);

	bool Result = FFileHelper::LoadANSITextFileToStrings(*FullPath, NULL, Names);
	if (!Result)
	{
		UE_LOG(TB_Log, Error, TEXT("Could not open %s"), *FullPath);
	}
}

void UTB_Name::PickRandomName(TArray<FString> &NameList, FString &Name)
{
	if (NameList.Num() > 0)
	{
		int32 RandomNumber = FGenericPlatformMath::Rand();
		Name = NameList[RandomNumber % NameList.Num()];
	}
}