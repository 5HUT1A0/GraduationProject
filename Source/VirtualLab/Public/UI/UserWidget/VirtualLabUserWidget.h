// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include"Game/Player/VirtualLabPlayerController.h"
#include "VirtualLabUserWidget.generated.h"

/**
 * 
 */

UCLASS()
class VIRTUALLAB_API UVirtualLabUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void SetWidgetController(UObject*InWidgetController);

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UObject>WidgetController;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	FText UMGName;


	UFUNCTION()
	void HandleInteractStateChanged(EInteractState NewState);

	//必须蓝图实现的函数
	UFUNCTION(BlueprintImplementableEvent)
	void ShowNoneUI();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowGrabTipsUI();

	UFUNCTION(BlueprintImplementableEvent)
	void ShowBeakerControlUI();

};
