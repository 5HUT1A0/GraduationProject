// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BeakerWaterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VIRTUALLAB_API UBeakerWaterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UBeakerWaterComponent();

	//当前水量
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float CurrentVolume;

	//最大容量
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxVolume;

	//最大流速
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxFlowRate;
protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	float CalculateFlowRate(float TiltAngle) const;

	float ConsumeWater(float DeltaTime, float FlowRate);

	void AddWater(float Volume);
		
};
