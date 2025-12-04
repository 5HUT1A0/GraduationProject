// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "VirtualLabGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALLAB_API UVirtualLabGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UDataTable>RuleTable;
};
