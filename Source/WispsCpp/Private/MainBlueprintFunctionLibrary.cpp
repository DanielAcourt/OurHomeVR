// Fill out your copyright notice in the Description page of Project Settings.


#include "MainBlueprintFunctionLibrary.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"

//Early Json logic make use of .txt rather than Json
/*
FString U_ReadWriteFile::ReadStringFromFile(FString FilePath, bool& Success, FString& OutInfoMessage)
{
	//Check if file exists

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
	{
		Success = false;
		//OutInfoMessage = FString::Printf(TEXT("Read From string file Failed"), *FilePath);
		return "";

	}

	FString RelString = "";

	if (!FFileHelper::LoadFileToString(RelString, *FilePath))
	{
		Success = false;
		//OutInfoMessage = FString::Printf(TEXT("Read from File Fail, unable to read file? - '%s'"), *FilePath);
		return "";
	}

	Success = true;
	//OutInfoMessage = FString::Printf(TEXT("Read from file Succeeded - '%s'"), *FilePath);
	return RelString;

}

void  U_ReadWriteFile::WriteStringToFile(FString FilePath, FString String, bool& Success, FString& OutInfoMessage)
{
	if (!FFileHelper::SaveStringToFile(String, *FilePath))
	{
		Success = false;
		//OutInfoMessage = FString::Printf(TEXT("Wrie String failed- Was unable to write to file is he file path valis?  - '%s'"), FilePath);
		return;
	}

	Success = true;
	//OutInfoMessage = FString::Printf(TEXT("Write STring to file Succeeded '%s'"), FilePath);


}
*/

//Early Json logic make use of .txt rather than Json
/*
int UMainBlueprintFunctionLibrary::MainBlueprintFunctionLibraryInput(int MyInt, EMainBlueprintFunctionLibraryInput inputpins)
{
	switch (inputpins)
	{

	case EMainBlueprintFunctionLibraryInput::addOne:
		MyInt++;
		break;
	case EMainBlueprintFunctionLibraryInput::Remove:
		MyInt--;
		break;
	default:
	case EMainBlueprintFunctionLibraryInput::DoNothing:
		break;
	}
	return MyInt;
}

void UMainBlueprintFunctionLibrary::MainBlueprintFunctionLibraryOutput(int MyInt, EMainBlueprintFunctionLibraryOutput& outputpins)
{
	if (MyInt % 2 == 0)
	{
		outputpins = EMainBlueprintFunctionLibraryOutput::isEven;
	}

	else
	{
		outputpins = EMainBlueprintFunctionLibraryOutput::isOdd;
	}
}

int UMainBlueprintFunctionLibrary::MultipleInputAndOutput(int MyInt, EMainBlueprintFunctionLibraryInput inputpins, EMainBlueprintFunctionLibraryOutput& outputpins)
{

	MyInt = MainBlueprintFunctionLibraryInput(MyInt, inputpins);

	MainBlueprintFunctionLibraryOutput(MyInt, outputpins);

	return MyInt;

}

int UMainBlueprintFunctionLibrary::MainBlueprintFunctionDiceInput(int MyDice, EMainBlueprintFunctionLibraryDiceInput DiceInput)
{

	switch (DiceInput)
	{

	case EMainBlueprintFunctionLibraryDiceInput::MinRoll:
		MyDice++;
	
		break;

	case EMainBlueprintFunctionLibraryDiceInput::MaxRoll:
		MyDice--;
		break;
	default:
	case EMainBlueprintFunctionLibraryDiceInput::DoNothing:
		break;
	}
	return MyDice;
}

void UMainBlueprintFunctionLibrary::MainBlueprintFunctionDiceOutput(int MyDiceMin,int MyDiceMax,int& MyDiceResult, EMainBlueprintFunctionLibraryDiceOutput& DiceOutput)
{
	MyDiceResult = FMath::RandRange(MyDiceMin, MyDiceMax);


}

void UMainBlueprintFunctionLibrary::DiceRoller(int DiceMin, int DiceMax, int& DiceResult)
{
	DiceResult = FMath::RandRange(DiceMin, DiceMax);
}

void UMainBlueprintFunctionLibrary::FloatRoller(float ValueMin, float ValueMax, float& ValueResult)
{
	ValueResult = FMath::RandRange(ValueMin, ValueMax);
}
*/

//Experiments making simple function blueprints Focusing on input and output of pins
/*
FString UMainBlueprintFunctionLibrary::ReadStringFromFile(FString FilePath, bool& Success, FString& OutInfoMessage)
{

	//Check if file exists

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
	{
		Success = false;
		//OutInfoMessage = FString::Printf(TEXT("Read From string file Failed"), *FilePath);
		return "";

	}

	FString RelString = "";

	if (!FFileHelper::LoadFileToString(RelString, *FilePath))
	{
		Success = false;
		//OutInfoMessage = FString::Printf(TEXT("Read from File Fail, unable to read file? - '%s'"), *FilePath);
		return "";
	}

	Success = true;
	//OutInfoMessage = FString::Printf(TEXT("Read from file Succeeded - '%s'"), *FilePath);
	return RelString;

}
/*
void UMainBlueprintFunctionLibrary::WriteStringToFile(FString FilePath, FString String, bool& Success, FString& OutInfoMessage)
{
	if (!FFileHelper::SaveStringToFile(String, *FilePath))
	{
		Success = false;
		//OutInfoMessage = FString::Printf(TEXT("Wrie String failed- Was unable to write to file is he file path valis?  - '%s'"), FilePath);
		return;
	}

	Success = true;
	//OutInfoMessage = FString::Printf(TEXT("Write STring to file Succeeded '%s'"), FilePath);


}
*/

FString UMainBlueprintFunctionLibrary::ReadStringFromFile(FString FilePath, bool& Success, FString& OutInfoMessage)
{
	//Check if file exists

	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
	{
		Success = false;
		//OutInfoMessage = FString::Printf(TEXT("Read From string file Failed"), *FilePath);
		return "";

	}

	FString RelString = "";

	if (!FFileHelper::LoadFileToString(RelString, *FilePath))
	{
		Success = false;
		//OutInfoMessage = FString::Printf(TEXT("Read from File Fail, unable to read file? - '%s'"), *FilePath);
		return "";
	}

	Success = true;
	//OutInfoMessage = FString::Printf(TEXT("Read from file Succeeded - '%s'"), *FilePath);
	return RelString;

}

void UMainBlueprintFunctionLibrary::WriteStringToFile(FString FilePath, FString String, bool& Success, FString& OutInfoMessage)
{

	if (!FFileHelper::SaveStringToFile(String, *FilePath))
	{
		Success = false;
		//OutInfoMessage = FString::Printf(TEXT("Wrie String failed- Was unable to write to file is he file path valis?  - '%s'"), FilePath);
		return;
	}

	Success = true;
	//OutInfoMessage = FString::Printf(TEXT("Write STring to file Succeeded '%s'"), FilePath);

}
