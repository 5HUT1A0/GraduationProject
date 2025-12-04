// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactive.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractive : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AInteractiveItemsBase;

class VIRTUALLAB_API IInteractive
{
	GENERATED_BODY()
public:
	virtual bool MatchInteractiveTags(const AInteractiveItemsBase* HandTarget, const AInteractiveItemsBase* OutTarget);

	virtual bool AttachToPoint(const AInteractiveItemsBase* HandTarget, const AInteractiveItemsBase* OutTarget);


};
