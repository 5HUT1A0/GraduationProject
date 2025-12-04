// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include"ActorBase/InteractiveItemsBase.h"
#include "VirtualLabGameInstanceSubsystem.generated.h"

/**
 * 
 */



UCLASS()
class VIRTUALLAB_API UVirtualLabGameInstanceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:

	virtual void Initialize(FSubsystemCollectionBase& Collection)override;

	UFUNCTION()
	void LoadRule();

	UFUNCTION()
	FText QueryUI(EInteractiveObjectType Hand, EInteractiveObjectType Target) const;

	TMap<EInteractiveObjectType, TMap<EInteractiveObjectType, FText>>RuleMap;

	
};
