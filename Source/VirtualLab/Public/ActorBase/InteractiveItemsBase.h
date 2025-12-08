// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/Grabbable.h"
#include"Interface/Interactive.h"
#include "InteractiveItemsBase.generated.h"


UENUM(BlueprintType)
enum class EInteractiveObjectType:uint8
{
	AlcoholBurner UMETA(DisplayName = "AlcoholBurner"),
	Beaker UMETA(DisplayName = "Beaker"),
	Stirrer UMETA(DisplayName = "Stirrer")
};

UCLASS()
class VIRTUALLAB_API AInteractiveItemsBase : public AActor,public IGrabbable,public IInteractive
{
	GENERATED_BODY()
	
public:	
	
	AInteractiveItemsBase();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	EInteractiveObjectType SelfType;

	//是否物体之间交互
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bCanInteractive=false;

	//是否启用射线检测
	UPROPERTY()
	bool bCanLineTrace = false;

protected:
	
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	TObjectPtr<USceneComponent>JointPoint;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TArray<EInteractiveObjectType>AllowedTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interaction")
	TObjectPtr<UDataTable>InteractiveRule;




public:	
	//抓取接口
	virtual void Grab(USceneComponent* HandComponent) override;
   
	//放下接口
	virtual void Drop()override;

	//返回被抓取对象接口
	virtual AActor* GetGrabbedActor()override { return this; }
	
	//匹配可交互标签接口
	virtual bool MatchInteractiveTags(const AInteractiveItemsBase* HandTarget,const AInteractiveItemsBase* OutTarget) override;

	//吸附到点接口
	virtual bool AttachToPoint(const AInteractiveItemsBase* HandTarget, const AInteractiveItemsBase* OutTarget) override;

	//获取Mesh
	UFUNCTION()
	UStaticMeshComponent* GetMesh() const { return Mesh; }

	//获取JoinPoint
	UFUNCTION()
	USceneComponent* GetJoinPoint()const { return JointPoint; }

};
