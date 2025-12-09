// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include"Camera/CameraComponent.h"
#include"EnhancedInputComponent.h"
#include"InputActionValue.h"
#include "PlayerCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrabEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractiveChanged, FText, bNewState);


class AInteractiveItemsBase;
class IGrabbable;
class AVirtualLabPlayerController;

UCLASS()
class VIRTUALLAB_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	
	APlayerCharacter();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintAssignable)
	FOnGrabEvent OnGrab;

	UPROPERTY(BlueprintAssignable)
	FOnGrabEvent OnRelease;

	UPROPERTY()
	FHitResult Hit;

	UPROPERTY(BlueprintAssignable)
	FOnInteractiveChanged OnInteractiveChanged;



protected:
	
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	//组件
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCameraComponent> CameraCom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<USceneComponent> RightHand;
	//输入动作
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_RightHand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction>IA_Interactive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction>IA_QuitInteractive;

	IGrabbable* OnHandTarget = nullptr;

	FVector ActorInitLocation;

	FVector RightHandInitLocation;

	UPROPERTY()
	bool bIsPickUp=true;


private:
	//回调函数
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void PickAndDown();

	//功能函数
	UFUNCTION()
	void PickUp(const FHitResult& HitResult);

	UFUNCTION()
	void PutDown(const FHitResult& HitResult);

	UFUNCTION()
	void Interaction();

	UFUNCTION()
	bool LineTrace(FHitResult& OutHit);

	UFUNCTION()
	void Stir();

	UFUNCTION()
	void QuitInteractive();

	UPROPERTY()
	TObjectPtr<AVirtualLabPlayerController>PC;
	

	//搅拌最大半径
	float MaxRadius = 200.f;

	//鼠标位置
	FVector2D MousePos;

	//上一帧鼠标位置
	FVector2D LastMousePos;


	//可与手中物品操作的对象
	UPROPERTY()
	TObjectPtr<AInteractiveItemsBase> InteractiveTarget;

	//手中对象
	UPROPERTY()
	TObjectPtr<AInteractiveItemsBase> HandTarget;

	 UPROPERTY()
	 FText UIName;


};
