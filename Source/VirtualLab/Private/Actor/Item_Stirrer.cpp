// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item_Stirrer.h"

AItem_Stirrer::AItem_Stirrer()
{
	AfterAttchRotation = FRotator(0.f, 90.f, 0.f);
}

bool AItem_Stirrer::AttachToPoint(const AInteractiveItemsBase* HandTarget, const AInteractiveItemsBase* OutTarget)
{
	if (OutTarget && HandTarget->bCanInteractive)
	{
		HandTarget->GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

		//不焊接物理
		FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, false);
		Mesh->SetAbsolute(false, false, true);
		AttachToComponent(OutTarget->GetJoinPoint(), Rules);

		Mesh->SetWorldRotation(AfterAttchRotation);

		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		bCanInteractive = false;
		return true;
	}
	return false;
}

void AItem_Stirrer::SetActorTickLocation(AInteractiveItemsBase* HandTarget, FVector ActorInitLocation, FVector2D OffSet)
{
	FVector NewLocation = ActorInitLocation;
	NewLocation.X += OffSet.X * MapScale;
	NewLocation.Y += OffSet.Y * MapScale;
	HandTarget->SetActorLocation(NewLocation);
}
