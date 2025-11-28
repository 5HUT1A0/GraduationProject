// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorBase/InteractiveItemsBase.h"
#include "Components/CapsuleComponent.h"


AInteractiveItemsBase::AInteractiveItemsBase()
{
	PrimaryActorTick.bCanEverTick = false;

	//创建静态网格体组件,并设置为根组件
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = Mesh;

	//设置网格体碰撞
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh->SetCollisionObjectType(ECC_GameTraceChannel1);
	Mesh->SetCollisionResponseToAllChannels(ECR_Block);
}

void AInteractiveItemsBase::BeginPlay()
{
	Super::BeginPlay();
	

}



//匹配标签
bool AInteractiveItemsBase::MatchInteractiveTags(const AInteractiveItemsBase* HandTarget,const AInteractiveItemsBase* OutTarget)
{
	if (!HandTarget || !OutTarget)
	{
		if(!HandTarget)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString::Printf(TEXT("NUllHandTarget")));
		}
		if (!OutTarget)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString::Printf(TEXT("NullOutTarget")));
		}
		return false;
	}

	if (OutTarget->AllowedTypes.Num() == 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Black, FString::Printf(TEXT("TempArray")));
		return false;
	}
	for (int32 i = 0; i < OutTarget->AllowedTypes.Num(); i++)
	{
		if (HandTarget->SelfType == OutTarget->AllowedTypes[i])
		{
			return true;
		}	
	}
	return false;
}

//抓取逻辑
void AInteractiveItemsBase::Grab(USceneComponent* HandComponent)
{
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	AttachToComponent(HandComponent, AttachRules);
}

//放下逻辑
void AInteractiveItemsBase::Drop()
{
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}



