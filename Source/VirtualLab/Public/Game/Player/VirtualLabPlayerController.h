// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VirtualLabPlayerController.generated.h"

/**
 * 
 */

class UInputMappingContext;
class UInputAction;

UENUM(BlueprintType)
enum class EMappingType :uint8
{
  InputMapping,
  PourMappint
};


UCLASS()
class VIRTUALLAB_API AVirtualLabPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	void SwitchInputMapping(EMappingType MappingType);

	//屏幕X、Y尺寸
	int32 ScreenX, ScreenY;

	//屏幕中心点位置向量
	FVector2D CenterScreen;


protected:
	/*输入映射上下文*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> PourMapping;

	UPROPERTY()
	TObjectPtr<UInputMappingContext>CurrentMapping;
	
private:

};
