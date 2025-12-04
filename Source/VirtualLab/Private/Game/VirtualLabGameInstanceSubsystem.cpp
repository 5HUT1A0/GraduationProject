// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VirtualLabGameInstanceSubsystem.h"
#include"Data/IneractiveItemsMatchData.h"
#include"Game/VirtualLabGameInstance.h"

void UVirtualLabGameInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	LoadRule();
}

void UVirtualLabGameInstanceSubsystem::LoadRule()
{
	RuleMap.Empty();

	UVirtualLabGameInstance* GI = Cast <UVirtualLabGameInstance>(GetGameInstance());

	if (!GI->RuleTable)
	{
		return;
	}

	TArray<FIneractiveItemsMatchData*>Rows;
	GI->RuleTable->GetAllRows(TEXT("UI RULES"), Rows);

	for (auto* Row : Rows)
	{
		RuleMap.FindOrAdd(Row->HandType).Add(Row->TargetType, Row->NameOfUI);
	}
	

}

FText UVirtualLabGameInstanceSubsystem::QueryUI(EInteractiveObjectType Hand, EInteractiveObjectType Target) const
{
	
	if (const TMap<EInteractiveObjectType, FText>* Inner = RuleMap.Find(Hand))
	{
		if (const FText* UI = Inner->Find(Target))
		{
			UE_LOG(LogTemp, Display, TEXT("%s"), *UI->ToString());
			return *UI;
		}
	}
	return FText::FromString(TEXT("None"));
}


