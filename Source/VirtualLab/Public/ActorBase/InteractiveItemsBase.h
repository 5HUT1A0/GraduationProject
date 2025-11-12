// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Grabbable.h"
#include "InteractiveItemsBase.generated.h"

UCLASS()
class VIRTUALLAB_API AInteractiveItemsBase : public AActor,public IGrabbable
{
	GENERATED_BODY()
	
public:	
	
	AInteractiveItemsBase();



protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> Mesh;





public:	
	//抓取接口
	virtual void Grab(USceneComponent* HandComponent) override;
   
	//放下接口
	virtual void Drop()override;

	//返回被抓取对象接口
	virtual AActor* GetGrabbedActor()override { return this; }
};
