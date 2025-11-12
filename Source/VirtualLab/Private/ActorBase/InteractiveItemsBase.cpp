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



