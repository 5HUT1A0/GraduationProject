// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/InteractiveItemsBase.h"
#include "Item_Stirrer.generated.h"

/**
 * 
 */
UCLASS()
class VIRTUALLAB_API AItem_Stirrer : public AInteractiveItemsBase
{
	GENERATED_BODY()
	AItem_Stirrer();

public:
	virtual bool AttachToPoint(const AInteractiveItemsBase* HandTarget, const AInteractiveItemsBase* OutTarget) override;

	virtual  void SetActorTickLocation(AInteractiveItemsBase* HandTarget, FVector ActorInitLocation, FVector2D OffSet)override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfor")
	FRotator AfterAttchRotation;

private:
	//”≥…‰Àı∑≈
	float MapScale = 0.01;
};
