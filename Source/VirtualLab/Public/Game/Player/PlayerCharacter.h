// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include"Camera/CameraComponent.h"
#include"EnhancedInputComponent.h"
#include"InputActionValue.h"
#include "PlayerCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGrabEvent);


class AInteractiveItemsBase;
class IGrabbable;

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


	IGrabbable* OnHandTarget;

	UPROPERTY()
	bool bIsPickUp=true;


private:
	//回调函数
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Use(const FInputActionValue& Value);

	//功能函数
	UFUNCTION()
	void PickUp(const FHitResult& HitResult);

	UFUNCTION()
	void PutDown(const FHitResult& HitResult);

	UFUNCTION()
	bool LineTrace(FHitResult&Hit);

	UPROPERTY()
	FHitResult Hit;
};
