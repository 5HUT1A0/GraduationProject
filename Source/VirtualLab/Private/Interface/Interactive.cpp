// Fill out your copyright notice in the Description page of Project Settings.


#include "Interface/Interactive.h"

// Add default functionality here for any IInteractive functions that are not pure virtual.
bool IInteractive::MatchInteractiveTags(const AInteractiveItemsBase* A, const AInteractiveItemsBase* B)
{
	// 默认实现（随便 return）
	return false;
}

bool IInteractive::AttachToPoint(const AInteractiveItemsBase* A, const AInteractiveItemsBase* B)
{
	// 默认实现
	return false;
}

void IInteractive::SetActorTickLocation(AInteractiveItemsBase* HandTarget, FVector2D OffSet)
{
	
}

