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
struct FInputActionValue;

class VIRTUALLAB_API IInteractive
{
	GENERATED_BODY()
public:
	virtual bool MatchInteractiveTags(const AInteractiveItemsBase* HandTarget, const AInteractiveItemsBase* OutTarget);

	virtual void AttachToPoint( AInteractiveItemsBase* HandTarget,  AInteractiveItemsBase* OutTarget);

	virtual void SetActorTickLocation(AInteractiveItemsBase* HandTarget, FVector2D OffSet);
	
	virtual void HasAttachPoint(AInteractiveItemsBase*CheckTarget);

	virtual bool bBeingAttached(IInteractive* InspectionItem);

	virtual void PourDowm(const FInputActionValue& Value);

	virtual void Translation(const FInputActionValue& Value);
};
