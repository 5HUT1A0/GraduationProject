// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Grabbable.h"
#include "InteractiveItemsBase.generated.h"


UENUM(BlueprintType)
enum class EInteractiveObjectType:uint8
{
	TypeA UMETA(DisplayName = "酒精灯"),
	TypeB UMETA(DisplayName = "烧杯"),
	TypeC UMETA(DisplayName = "搅拌棒")
};

UCLASS()
class VIRTUALLAB_API AInteractiveItemsBase : public AActor,public IGrabbable
{
	GENERATED_BODY()
	
public:	
	
	AInteractiveItemsBase();

	//可以物体之间交互
	UPROPERTY()
	bool bCanInteractive=false;

	UPROPERTY()
	bool bCanLineTrace = false;

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USceneComponent>JointPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	EInteractiveObjectType SelfType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TArray<EInteractiveObjectType>AllowedTypes;





public:	
	//抓取接口
	virtual void Grab(USceneComponent* HandComponent) override;
   
	//放下接口
	virtual void Drop()override;

	//返回被抓取对象接口
	virtual AActor* GetGrabbedActor()override { return this; }
	
	UFUNCTION()
	bool MatchInteractiveTags(const AInteractiveItemsBase* HandTarget,const AInteractiveItemsBase* OutTarget);

	UFUNCTION()
	bool AttachToPoint(const AInteractiveItemsBase* HandTarget, const AInteractiveItemsBase* OutTarget);
};
