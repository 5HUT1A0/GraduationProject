// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/VirtualLabWidgetControllerBase.h"
#include "OverlapWidgetController.generated.h"

/**
 * 
 */


UENUM(BlueprintType)
enum class EInteractState :uint8
{
	None UMETA(DisplayName = "无交互"),
	Hover UMETA(DisplayName = "悬停提示"),
	Grab UMETA(DisplayName = "抓取提示")
};

//多播事件
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractState, EInteractState, NewState);

UCLASS(BlueprintType, Blueprintable)
class VIRTUALLAB_API UOverlapWidgetController : public UVirtualLabWidgetControllerBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "UIState")
	EInteractState CurrentInteractState = EInteractState::None;

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnInteractState OnInteractStateChanged;


public:
	UFUNCTION(BlueprintCallable)
	void SetInteractState(EInteractState NewState);



	virtual void BroadcastInitialValues() override;
	virtual void BindCallBackToDependencies() override;

private:
	UFUNCTION()
	void OnCharacterGrab();

	UFUNCTION()
	void OnCharacterRelease();

};
