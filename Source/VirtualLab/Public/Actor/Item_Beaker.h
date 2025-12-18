// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase/InteractiveItemsBase.h"
#include "Item_Beaker.generated.h"

/**
 * 
 */
class AVirtualLabPlayerController;
class APlayerCharacter;

UCLASS()
class VIRTUALLAB_API AItem_Beaker : public AInteractiveItemsBase
{
	GENERATED_BODY()

public:
	AItem_Beaker();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USceneComponent>PointForBeaker;

	virtual bool AttachToPoint( AInteractiveItemsBase* HandTarget,  AInteractiveItemsBase* OutTarget) override;

	virtual void BeginPlay()override;

	virtual void HasAttachPoint(AInteractiveItemsBase* CheckTarget) override;

	//附着物为True，被附着为false
	virtual bool bBeingAttached(IInteractive* InspectionItem)override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfor")
	FRotator AfterAttchRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AVirtualLabPlayerController>PC;

	UPROPERTY()
	TObjectPtr<APlayerCharacter>Player;

};
