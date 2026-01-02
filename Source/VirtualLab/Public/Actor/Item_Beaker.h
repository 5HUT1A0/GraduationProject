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
class UBeakerWaterComponent;
class UNiagaraComponent;

UCLASS()
class VIRTUALLAB_API AItem_Beaker : public AInteractiveItemsBase
{
	GENERATED_BODY()

public:
	AItem_Beaker();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USceneComponent>PointForBeaker;

	virtual void AttachToPoint( AInteractiveItemsBase* HandTarget,  AInteractiveItemsBase* OutTarget) override;

	virtual void BeginPlay()override;

	virtual void Tick(float DeltaTime) override;

	virtual void HasAttachPoint(AInteractiveItemsBase* CheckTarget) override;

	virtual void PourDowm(const FInputActionValue& Value) override;

	virtual void Translation(const FInputActionValue& Value)override;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Transfor")
	FRotator AfterAttchRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AVirtualLabPlayerController>PC;

	UPROPERTY()
	TObjectPtr<APlayerCharacter>Player;

	FRotator NewRotator;

	UPROPERTY()
	TObjectPtr<AItem_Beaker> TargetBeaker;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBeakerWaterComponent>WaterComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent>PourWaterFX;

	float GetTiltAngle()const;


};
