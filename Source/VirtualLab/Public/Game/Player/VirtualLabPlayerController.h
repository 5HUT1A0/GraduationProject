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



UCLASS()
class VIRTUALLAB_API AVirtualLabPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	//屏幕X、Y尺寸
	int32 ScreenX, ScreenY;

	//屏幕中心点位置向量
	FVector2D CenterScreen;


protected:
	/*输入映射上下文*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	
private:

};
