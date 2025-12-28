// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/Interactive.h"

// Add default functionality here for any IInteractive functions that are not pure virtual.
bool IInteractive::MatchInteractiveTags(const AInteractiveItemsBase* A, const AInteractiveItemsBase* B)
{
	// 默认实现（随便 return）
	return false;
}

void IInteractive::AttachToPoint( AInteractiveItemsBase* A,  AInteractiveItemsBase* B)
{
	// 默认实现
}

void IInteractive::SetActorTickLocation(AInteractiveItemsBase* HandTarget, FVector2D OffSet)
{
	
}

void IInteractive::HasAttachPoint(AInteractiveItemsBase* CheckTarget)
{

}

bool IInteractive::bBeingAttached(IInteractive* InspectionItem)
{
	return true;
}

void IInteractive::PourDowm(const FInputActionValue& Value)
{
}

void IInteractive::Translation(const FInputActionValue& Value)
{
}
