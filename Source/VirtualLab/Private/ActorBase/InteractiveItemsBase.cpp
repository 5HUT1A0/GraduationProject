// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorBase/InteractiveItemsBase.h"
#include "Components/CapsuleComponent.h"


AInteractiveItemsBase::AInteractiveItemsBase()
{
	PrimaryActorTick.bCanEverTick = false;

	//创建静态网格体组件,并设置为根组件
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = Mesh;

	JointPoint = CreateDefaultSubobject<USceneComponent>(TEXT("JointPoint"));
	JointPoint->SetupAttachment(RootComponent);

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
	else
	{
		for (auto AllowedType : AllowedTypes)
		{
			if (HandTarget->SelfType == AllowedType)
			{
				//UE_LOG(LogTemp,Display,TEXT("Match!"))
				return true;
			}
		}
	}
	return false;
}

//结合逻辑
bool AInteractiveItemsBase::AttachToPoint(const AInteractiveItemsBase* HandTarget, const AInteractiveItemsBase* OutTarget)
{
	if(OutTarget&&HandTarget->bCanInteractive)
	{
		HandTarget->Mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

		//不焊接物理
		FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative,false);
		AttachToComponent(OutTarget->JointPoint, Rules);
		
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
		bCanInteractive = false;
		return true;
	}
	return false;
}

//抓取逻辑
void AInteractiveItemsBase::Grab(USceneComponent* HandComponent)
{
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	AttachToComponent(HandComponent, AttachRules);
	bCanLineTrace = false;
	
}

//放下逻辑
void AInteractiveItemsBase::Drop()
{
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	bCanLineTrace = true;
	bCanInteractive = true;
}



/*这些接口功能可以到时候到这个类的子类里各自重写，现在写的只是默认状态下的功能，之后拓展详细功能*/