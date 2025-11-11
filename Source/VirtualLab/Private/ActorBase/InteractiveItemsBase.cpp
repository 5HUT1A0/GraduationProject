// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorBase/InteractiveItemsBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
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

// Called when the game starts or when spawned
void AInteractiveItemsBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AInteractiveItemsBase::Grab(USceneComponent* HandComponent)
{
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FAttachmentTransformRules AttachRules = FAttachmentTransformRules::SnapToTargetNotIncludingScale;
	AttachToComponent(HandComponent, AttachRules);
}



