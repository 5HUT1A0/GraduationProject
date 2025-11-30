// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlapWidgetController.h"
#include"Game/Player/PlayerCharacter.h"
#include"Game/Player/VirtualLabPlayerController.h"

void UOverlapWidgetController::SetInteractState(EInteractState NewState)
{
	if (CurrentInteractState == NewState)return;
	CurrentInteractState = NewState;
	OnInteractStateChanged.Broadcast(NewState);
	UE_LOG(LogTemp, Display, TEXT("%s "), *StaticEnum<EInteractState>()->GetNameByValue((int64)NewState).ToString());
}


	

void UOverlapWidgetController::BroadcastInitialValues()
{
	OnInteractStateChanged.Broadcast(CurrentInteractState);
}

void UOverlapWidgetController::BindCallBackToDependencies()
{
	AVirtualLabPlayerController * PC = Cast<AVirtualLabPlayerController>(GetOuter());
	if (APlayerCharacter *PlayerCharacter = Cast <APlayerCharacter>(PC->GetPawn()))
	{
		PlayerCharacter->OnGrab.AddDynamic(this, &UOverlapWidgetController::OnCharacterGrab);
		PlayerCharacter->OnRelease.AddDynamic(this, &UOverlapWidgetController::OnCharacterRelease);
	}
}

void UOverlapWidgetController::OnCharacterGrab()
{
	SetInteractState(EInteractState::Grab);
	//UE_LOG(LogTemp, Display, TEXT("Grab "));
}

void UOverlapWidgetController::OnCharacterRelease()
{
	SetInteractState(EInteractState::None);
	//UE_LOG(LogTemp, Display, TEXT("None "));
}
