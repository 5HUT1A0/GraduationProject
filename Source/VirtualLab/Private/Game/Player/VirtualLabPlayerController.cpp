// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Player/VirtualLabPlayerController.h"
#include "InputMappingContext.h"
#include"EnhancedInputSubsystems.h"
#include"UI/HUD/VirtualLabHUD.h"
#include"GameFramework/Pawn.h"

void AVirtualLabPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if(InputMapping)
		{
			SubSystem->ClearAllMappings();
			SubSystem->AddMappingContext(InputMapping, 0);
			CurrentMapping = InputMapping;
		}
	}
	//bShowMouseCursor = true;


	GetViewportSize(ScreenX, ScreenY);
	CenterScreen = FVector2D(ScreenX * 0.5f, ScreenY * 0.5f);
}

void AVirtualLabPlayerController::SwitchInputMapping(EMappingType MappingType)
{
	UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (CurrentMapping)
	{
		SubSystem->RemoveMappingContext(CurrentMapping);
	}
	switch (MappingType)
	{
	case EMappingType::InputMapping:
		SubSystem->AddMappingContext(InputMapping, 0);
		CurrentMapping = InputMapping;
		break;
	case EMappingType::PourMappint:
		SubSystem->AddMappingContext(PourMapping, 1);
		CurrentMapping = PourMapping;
		UE_LOG(LogTemp, Display, TEXT("Switch PourMapping"));
		break;
	default:
		break;
	}
}
