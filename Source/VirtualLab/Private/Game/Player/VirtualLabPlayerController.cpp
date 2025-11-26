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
		if(InputMapping.IsValid()||InputMapping.LoadSynchronous())
		{
			SubSystem->ClearAllMappings();
			SubSystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
		}
	}


}
