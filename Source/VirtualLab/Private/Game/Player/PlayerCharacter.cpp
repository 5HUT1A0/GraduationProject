// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Player/PlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Interface/Grabbable.h"
#include"Interface/Interactive.h"
#include "ActorBase/InteractiveItemsBase.h"
#include"Game/VirtualLabGameInstanceSubsystem.h"
#include"Blueprint/UserWidget.h"


//构造
APlayerCharacter::APlayerCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	//创建组件并且进行设置
	USceneComponent* RootComp = GetRootComponent();
	if (RootComp)
	{
		//摄像机组件
		CameraCom = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		CameraCom->SetupAttachment(RootComp);
		CameraCom->bUsePawnControlRotation = true;

		//右手组件
		RightHand = CreateDefaultSubobject<USceneComponent>(TEXT("RightHand"));
		RightHand->SetupAttachment(CameraCom);
	}


}

//BeginPlay
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(false);

	//获取屏幕尺寸
	PC = GetWorld()->GetFirstPlayerController();
	PC->GetViewportSize(ScreenX, ScreenY);
	CenterScreen= FVector2D(ScreenX * 0.5f, ScreenY * 0.5f);

}


//Tick
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//LineTrace(Hit);
	//auto* NewTarget = Cast<AInteractiveItemsBase>(Hit.GetActor());

	//if (NewTarget != InteractiveTarget)
	//{
	//	
	//	InteractiveTarget = NewTarget;

	//	if (InteractiveTarget && InteractiveTarget->bCanLineTrace)
	//	{
	//		HandTarget->bCanInteractive = InteractiveTarget->MatchInteractiveTags(HandTarget, InteractiveTarget);

	//		auto* Sub = GetGameInstance()->GetSubsystem<UVirtualLabGameInstanceSubsystem>();
	//		FText OutUIName = Sub->QueryUI(HandTarget->SelfType, InteractiveTarget->SelfType);
	//		OnInteractiveChanged.Broadcast(OutUIName);
	//	}
	//	else
	//	{
	//		OnInteractiveChanged.Broadcast(FText::FromString("None"));
	//	}
	//}

	//调用搅拌
	Stir();
}


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
		if (IA_RightHand)
		{
			EnhanceInputComponent->BindAction(IA_RightHand, ETriggerEvent::Triggered, this, &APlayerCharacter::PickAndDown);
		}
		if (IA_Interactive)
		{
			EnhanceInputComponent->BindAction(IA_Interactive, ETriggerEvent::Triggered, this, &APlayerCharacter::Interaction);
		}
	}


}


//移动逻辑
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

//视角逻辑
void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D InputValue = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(InputValue.X);
		AddControllerPitchInput(-InputValue.Y);

		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("X=%.2f,Y=%.2f"), InputValue.X, InputValue.Y));  Debug
    }
}

//拿起放下的回调函数
void APlayerCharacter::PickAndDown()
{

	if (LineTrace(Hit))
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Hit but actor == nullptr"));
			return;
		}

		if (HitActor->GetRootComponent()->GetCollisionObjectType() == ECC_GameTraceChannel1 && bIsPickUp)
		{
			PickUp(Hit);
			SetActorTickEnabled(true);
		}
		else if (HitActor->GetRootComponent()->GetCollisionObjectType() == ECC_GameTraceChannel2 && !bIsPickUp)
		{
			PutDown(Hit);
			SetActorTickEnabled(false);
		}
	}

}



//调用抓取
void APlayerCharacter::PickUp(const FHitResult& HitResult)
{
	if(IGrabbable*GrabTarget=Cast<IGrabbable>(HitResult.GetActor()))
	{
		HandTarget = Cast<AInteractiveItemsBase>(HitResult.GetActor());
		OnHandTarget = GrabTarget;
		GrabTarget->Grab(RightHand);
		OnGrab.Broadcast();
		bIsPickUp = false;

		PC->SetMouseLocation(CenterScreen.X, CenterScreen.Y);  
		PC->SetIgnoreLookInput(true);
		//测试
		ActorInitLocation = HandTarget->GetActorLocation();//测试,后续改到其他地方
	}
}

//调用放下
void APlayerCharacter::PutDown(const FHitResult& HitResult)
{
	
		OnHandTarget->Drop();
		OnHandTarget->GetGrabbedActor()->SetActorLocation(HitResult.ImpactPoint);
		OnRelease.Broadcast();
		HandTarget = nullptr;
		bIsPickUp = true;
	
}

//调用与桌子物体交互逻辑
void APlayerCharacter::Interaction()
{
	UE_LOG(LogTemp, Display, TEXT("PressedF"));
	IInteractive* IHandTarget = Cast<IInteractive>(HandTarget);
	if (IHandTarget&& IHandTarget->AttachToPoint(HandTarget, InteractiveTarget))
	{
		SetActorTickEnabled(false);
		OnInteractiveChanged.Broadcast(FText::FromString("None"));
		bIsPickUp = true;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("NoHandTarget "));
	}
}


//射线检测（只负责检测）
bool APlayerCharacter::LineTrace(FHitResult& OutHit)
{
	FVector StartLocation = CameraCom->GetComponentLocation();
	FVector EndLocation = StartLocation + CameraCom->GetForwardVector() * 500.f;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FCollisionObjectQueryParams ObjectParms;
	ObjectParms.AddObjectTypesToQuery(ECC_GameTraceChannel1);
	ObjectParms.AddObjectTypesToQuery(ECC_GameTraceChannel2);

	bool bHit = GetWorld()->LineTraceSingleByObjectType(OutHit, StartLocation, EndLocation, ObjectParms, Params);
	//Debug射线
	FColor DebugColor = bHit ? FColor::Green : FColor::Red;
	//DrawDebugLine(GetWorld(), StartLocation, EndLocation, DebugColor, false, 2.f, 0, 1.f);
		//DebugPoint
	/*	DrawDebugPoint(GetWorld(), OutHit.ImpactPoint, 10.0f, FColor::Yellow, false, 2.f);
		if(OutHit.GetActor())
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("Hit Actor Name:%s"), *OutHit.GetActor()->GetName()));
		}*/
		//UE_LOG(LogTemp, Display, TEXT("HitLocation:%s "), *Hit.ImpactPoint.ToString());

		return bHit;

}

void APlayerCharacter::Stir()
{

	GetWorld()->GetFirstPlayerController()->GetMousePosition(MousePos.X, MousePos.Y);

	if (MousePos != LastMousePos)
	{
		FVector2D OffSet = MousePos - CenterScreen;	
		//Debug
		GEngine->AddOnScreenDebugMessage(1, 
			                             2.f,
			                             FColor::Black,
			                             FString::Printf(TEXT("Distacne:%d"), OffSet.Size()));
		if (OffSet.Size()>= MaxRadius)
		{
			OffSet = OffSet.GetSafeNormal() * MaxRadius;
			//PC->SetMouseLocation(OffSet.X, OffSet.Y);
			
		}
		//调用更新位置函数
		HandTarget->SetActorTickLocation(HandTarget, ActorInitLocation, OffSet);
		//Debug
		GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, FString::Printf(TEXT("Offset:%s"), *OffSet.ToString()));
		LastMousePos = MousePos;
	}
	
}







