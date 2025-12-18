// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item_Beaker.h"
#include"Game/Player/VirtualLabPlayerController.h"
#include"Game/Player/PlayerCharacter.h"

AItem_Beaker::AItem_Beaker()
{
	PointForBeaker = CreateDefaultSubobject<USceneComponent>(TEXT("PointForBeaker"));
	PointForBeaker->SetupAttachment(RootComponent);

}



bool AItem_Beaker::AttachToPoint( AInteractiveItemsBase* HandTarget,  AInteractiveItemsBase* OutTarget)
{
	if (OutTarget && HandTarget->bCanInteractive)
	{
		//取消与手部结合
		HandTarget->GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

		//结合后的位置、旋转设置
		FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, false);
		Mesh->SetAbsolute(false, false, true);

		if (Cast<AItem_Beaker>(OutTarget))
		{
			 const AItem_Beaker* OutBeaker= Cast<AItem_Beaker>(OutTarget);
			AttachToComponent(OutBeaker->PointForBeaker, Rules);
		}
		else
		{
			AttachToComponent(OutTarget->GetJoinPoint(), Rules);
		}
		Mesh->SetWorldRotation(AfterAttchRotation);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		//倾斜实现所需设置
		PC->SetIgnoreLookInput(true);

		//广播更新UI
		Player = Cast<APlayerCharacter>(GetOwner());
		Player->OnInteractiveChanged.Broadcast(NSLOCTEXT("Interactive", "Abort", "中止"));

		
		//HasAttachPoint();  //测试

		//相关布尔值设置
		HandTarget->bCanInteractive = false;
		
		HandTarget->bCanShowUI = false;
		OutTarget->bCanShowUI = true;
		
		bCanQuitInteractive = true;

		HandTarget->bNeedCheckPoint = true;
		OutTarget->bNeedCheckPoint = true;

		return true;
	}
	return false;
}

void AItem_Beaker::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<AVirtualLabPlayerController>(GetWorld()->GetFirstPlayerController());
}

//附着后才激活
void AItem_Beaker::HasAttachPoint(AInteractiveItemsBase* CheckTarget)
{
	//附着物
		if (bBeingAttached(CheckTarget))
		{
			UE_LOG(LogTemp, Display, TEXT("继续操作"));
		}
	//被附着物
		else
		{
			if (!JointPoint->GetAttachChildren().IsEmpty() && !PointForBeaker->GetAttachChildren().IsEmpty())
			{
				CheckTarget->bCanInteractive = false;
				CheckTarget->bCanShowUI = false;
				//CheckTarget->bNeedCheckPoint = false;
			}
			CheckTarget->bCanInteractive = true;
		}
	
}

bool AItem_Beaker::bBeingAttached(IInteractive* InspectionItem)
{

	if (Cast<AInteractiveItemsBase>(InspectionItem)->GetRootComponent()->GetAttachParent())
	{
		return true;
	}
	return false;
}



