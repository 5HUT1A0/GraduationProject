// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/UserWidget/VirtualLabUserWidget.h"
#include"UI/WidgetController/OverlapWidgetController.h"

void UVirtualLabUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	UOverlapWidgetController* OWController = Cast<UOverlapWidgetController>(WidgetController);
	//UE_LOG(LogTemp, Warning, TEXT("Bind To Controller Ptr in Widget: %p"), OWController);
	OWController->OnInteractStateChanged.AddDynamic(this, &UVirtualLabUserWidget::HandleInteractStateChanged);
}

void UVirtualLabUserWidget::HandleInteractStateChanged(EInteractState NewState)
{
	//UE_LOG(LogTemp, Display, TEXT("StartChange "));
	switch (NewState)
	{
	case EInteractState::None:
		ShowNoneUI();
		break;
	case EInteractState::BeakerControl:
		ShowBeakerControlUI();
		break;
	case EInteractState::Grab:
		ShowGrabTipsUI();
		break;
	default:
		break;
	}
}

