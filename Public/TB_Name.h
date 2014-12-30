// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "TB_Character.h" // for ETB_Gender

#include "TB_Name.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class UE4_TURNBASED_API UTB_Name : public UObject
{
	GENERATED_BODY()
public:
	virtual void PostInitProperties() override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Files")
	FString MaleFirstNameFile;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Files")
	FString FemaleFirstNameFile;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Input Files")
	FString LastNameFile;

	UPROPERTY(BlueprintReadWrite, Category = "Default")
	FString FirstName;
	UPROPERTY(BlueprintReadWrite, Category = "Default")
	FString LastName;

	UFUNCTION(BluePrintNativeEvent, BlueprintCallable, Category = "Default")
	void Generate(ETB_Gender Gender);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Default")
	FString ToString();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Default")
	FText ToText();

private:
	void LoadNames(FString &NameFile, TArray<FString> &Names);
	void PickRandomName(TArray<FString> &NameList, FString &Name);
	FCriticalSection Mutex;
	static TArray<FString> MaleFirstNames;
	static TArray<FString> FemaleFirstNames;
	static TArray<FString> LastNames;
};
