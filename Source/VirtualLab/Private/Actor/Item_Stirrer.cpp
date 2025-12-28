// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item_Stirrer.h"
#include"Game/Player/VirtualLabPlayerController.h"
#include"Game/Player/PlayerCharacter.h"

AItem_Stirrer::AItem_Stirrer()
{
	AfterAttchRotation = FRotator(0.f, 90.f, 0.f);
}

void AItem_Stirrer::AttachToPoint( AInteractiveItemsBase* HandTarget,  AInteractiveItemsBase* OutTarget)
{
	if (OutTarget && HandTarget->bCanInteractive)
	{
		HandTarget->GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

		//结合后的位置、旋转设置
		FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, false);
		Mesh->SetAbsolute(false, false, true);
		AttachToComponent(OutTarget->GetJoinPoint(), Rules);
		Mesh->SetWorldRotation(AfterAttchRotation);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		//搅拌实现所需设置
		PC->SetMouseLocation(PC->CenterScreen.X, PC->CenterScreen.Y);
		PC->SetIgnoreLookInput(true);
		ActorInitLocation = HandTarget->GetActorLocation();

		//广播更新UI
		Player = Cast<APlayerCharacter>(GetOwner());
		Player->OnInteractiveChanged.Broadcast(NSLOCTEXT("Interactive", "Abort", "中止"));


		//相关布尔值设置
		HandTarget->bContinue = true;
		bCanStirring = true;
		bCanInteractive = false;
		bCanQuitInteractive = true;
		
	}
	
}

void AItem_Stirrer::SetActorTickLocation(AInteractiveItemsBase* HandTarget, FVector2D OffSet)
{
	FVector NewLocation = ActorInitLocation;
	NewLocation.X += OffSet.X * MapScale;
	NewLocation.Y += OffSet.Y * MapScale;
	HandTarget->SetActorLocation(NewLocation);
}

void AItem_Stirrer::BeginPlay()
{
    Super::BeginPlay();

	PC = Cast <AVirtualLabPlayerController>(GetWorld()->GetFirstPlayerController());
}

void AItem_Stirrer::HasAttachPoint(AInteractiveItemsBase* CheckTarget)
{
	if (bBeingAttached(CheckTarget))
	{
		//广播更新UI
		Player = Cast<APlayerCharacter>(GetOwner());
		Player->OnInteractiveChanged.Broadcast(NSLOCTEXT("Interactive", "Abort", "继续操作"));
	}
}

