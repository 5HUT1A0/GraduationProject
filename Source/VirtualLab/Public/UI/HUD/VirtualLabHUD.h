// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "VirtualLabHUD.generated.h"

/**
 * 
 */
class UUserWidget;
class UOverlapWidgetController;
class UVirtualLabUserWidget;

UCLASS()
class VIRTUALLAB_API AVirtualLabHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay()override;

	void InitUserWidget();

	UOverlapWidgetController* GetOverlapWidgetController();


protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UVirtualLabUserWidget>OverlapWidgetClass;


	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlapWidgetController>OverlapWidgetControllerClass;


	UPROPERTY()
	TObjectPtr<UOverlapWidgetController>OverlapWidgetController;


	UPROPERTY()
	TObjectPtr<UVirtualLabUserWidget>OverlapWidget;
};
