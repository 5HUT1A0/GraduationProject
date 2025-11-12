// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorBase/EffectiveAreaBase.h"

// Sets default values
AEffectiveAreaBase::AEffectiveAreaBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AreaMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AreaMesh"));
	RootComponent = AreaMesh;

}

// Called when the game starts or when spawned
void AEffectiveAreaBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEffectiveAreaBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



