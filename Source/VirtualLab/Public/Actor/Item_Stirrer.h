// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/InteractiveItemsBase.h"
#include "Item_Stirrer.generated.h"

/**
 * 
 */
class AVirtualLabPlayerController;
class APlayerCharacter;

UCLASS()
class VIRTUALLAB_API AItem_Stirrer : public AInteractiveItemsBase
{
	GENERATED_BODY()
	AItem_Stirrer();

public:
	virtual void AttachToPoint( AInteractiveItemsBase* HandTarget,  AInteractiveItemsBase* OutTarget) override;

	virtual  void SetActorTickLocation(AInteractiveItemsBase* HandTarget, FVector2D OffSet)override;

	virtual void BeginPlay()override;

	virtual void HasAttachPoint(AInteractiveItemsBase* CheckTarget) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfor")
	FRotator AfterAttchRotation;

private:
	//”≥…‰Àı∑≈
	float MapScale = 0.01;




	UPROPERTY()
	TObjectPtr<AVirtualLabPlayerController>PC;
	UPROPERTY()
	TObjectPtr<APlayerCharacter>Player;
};
