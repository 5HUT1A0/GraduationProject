
# 虚拟实验室
## 项目介绍
这个项目是作为我的毕业设计和当作一次UE C++练手。游戏内容如项目名称而言，就是一个模拟实验操作的游戏，我打算在游戏中设计一个实验，让玩家进行实验步骤操作。如果后续还有时间就再加多一个实验关卡。

## 代码功能实现
### **VirtualLabPlayerController**
我在这里实现的功能有：

 **1. 绑定增强输入**
 
 - VirtualLabPlayerController.h
 ```
 protected:
	/*输入映射上下文*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;
```
头文件中设置传入上下文引用

- VirtualLabPlayerController,cpp
```
if (UEnhancedInputLocalPlayerSubsystem* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
{
	if(InputMapping.IsValid()||InputMapping.LoadSynchronous())
	{
		SubSystem->ClearAllMappings();
		SubSystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
	}
}
```
先获取本地玩家的增强输入子系统，再根据传入的上下文引用来绑定到玩家增强输入子系统中。

### PlayerCharacter
在这里我实现的功能有：

**1. 初始化第一人称相机**

- PlayerCharacter.h
```
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraCom;
```

- PlayerCharacter.cpp
```
APlayerCharacter::APlayerCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	//创建摄像机并且进行设置
	USceneComponent* RootComp = GetRootComponent();
	if (RootComp)
	{
		CameraCom = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		CameraCom->SetupAttachment(RootComp);
		CameraCom->bUsePawnControlRotation = true;
	}

}
```
在Character构造时候创建摄像机。``CreateDefaultSubobject<UCameraComponent(TEXT("Camera"));``
（先初始化）

后将这个摄像机挂载到根组件下
`` CameraCom->SetupAttachment(RootComp);``
(想要创建一个新的Component的话就需要先获取需要挂载的根组件)

同时设置Pawn随控制器旋转。

**2.  绑定InputAction到增强输入中**

- PlayerCharacter.h

```
protected:
	/*输入动作*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Move;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_Look;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_PickUp;

private:
	/*回调函数*/
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Use(const FInputActionValue& Value);
```

- PlayerCharacter.cpp

```
//动作绑定
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhanceInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(IA_Move)
		{
			EnhanceInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		}
		if (IA_Look)
		{
			EnhanceInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		}
		if (IA_PickUp)
		{
			EnhanceInputComponent->BindAction(IA_PickUp, ETriggerEvent::Triggered, this, &APlayerCharacter::Use);
		}
	}


}


//动作回调函数
void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	if (Controller)
	{
		const FRotator ControlRot = Controller->GetControlRotation();
		const FRotator YawRotator(0.f, ControlRot.Yaw, 0.f);

		const FVector Forward = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y);

		AddMovementInput(Forward, InputValue.Y);
		AddMovementInput(Right, InputValue.X);

	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(InputValue.X);
		AddControllerPitchInput(-InputValue.Y);

    }
}

void APlayerCharacter::Use(const FInputActionValue& Value)
{
	FHitResult HitResult;
	FVector StartLocation = CameraCom->GetComponentLocation();
	FVector EndLocation = StartLocation +CameraCom->GetForwardVector() * 500.f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	
	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, StartLocation, EndLocation, ECC_WorldStatic, Params);
	//Debug射线
	FColor DebugColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), StartLocation, EndLocation,DebugColor, false, 2.f, 0, 1.f);

	if (bHit)
	{
		//Debug
		DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 10.0f, FColor::Yellow, false, 2.f);
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("Hit Actor Name:%s"), *HitResult.GetActor()->GetName()));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Hit Nothing") ));
	}
}

```

一般呢设置增强输入就是这个流程：

<!--stackedit_data:
eyJoaXN0b3J5IjpbLTE1NjU5OTk1MTUsMjg5NDExNTZdfQ==
-->