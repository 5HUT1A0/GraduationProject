// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Player/PlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Interface/Grabbable.h"
#include"Interface/Interactive.h"
#include "ActorBase/InteractiveItemsBase.h"
#include"Game/VirtualLabGameInstanceSubsystem.h"
#include"Game/Player/VirtualLabPlayerController.h"
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

	PC = Cast <AVirtualLabPlayerController>(GetController());

}


//Tick
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//射线检测  显示对应UI
	LineTrace(Hit);
	auto* NewTarget = Cast<AInteractiveItemsBase>(Hit.GetActor());

	if (NewTarget != InteractiveTarget&&!bUsing)
	{
		
		InteractiveTarget = NewTarget;


		//查表匹配UI
		if (InteractiveTarget && InteractiveTarget->bCanShowUI)
		{
			//UE_LOG(LogTemp, DIsValidplay, TEXT("%s"), InteractiveTarget->bCanLineTrace?TEXT("True"):TEXT("fasle"));
			HandTarget->bCanInteractive = InteractiveTarget->MatchInteractiveTags(HandTarget, InteractiveTarget);
			auto* Sub = GetGameInstance()->GetSubsystem<UVirtualLabGameInstanceSubsystem>();
			FText OutUIName = Sub->QueryUI(HandTarget->SelfType, InteractiveTarget->SelfType);
			OnInteractiveChanged.Broadcast(OutUIName);
		}
		else if(InteractiveTarget==nullptr)
		{
			OnInteractiveChanged.Broadcast(FText::FromString("None"));
		}

		//查结合点
		if (InteractiveTarget && InteractiveTarget->bContinue)
		{
			IInteractive* CheckPointTarget = Cast<IInteractive>(InteractiveTarget);
			CheckPointTarget->HasAttachPoint(InteractiveTarget);
		}

		//结合物判断
		if (!RightHand->GetAttachChildren().IsEmpty())
		{
			return;
		}
		if (IInteractive* InspectionItem = Cast<IInteractive>(InteractiveTarget))
		{
			bCanPickUp = InspectionItem->bBeingAttached(InspectionItem);
		}
		else
		{
			bCanPickUp = true;
		}

		UE_LOG(LogTemp, Display, TEXT("PickUp State: %s"),
			bCanPickUp ? TEXT("true") : TEXT("false"));


	}

	//调用搅拌
	if(UsingTarget && UsingTarget->bCanStirring)
	{
		Stir();
	}
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
		if (IA_QuitInteractive)
		{
			EnhanceInputComponent->BindAction(IA_QuitInteractive, ETriggerEvent::Triggered, this, &APlayerCharacter::QuitInteractive);
		}
		if (IA_Pour)
		{
			EnhanceInputComponent->BindAction(IA_Pour, ETriggerEvent::Triggered, this, &APlayerCharacter::Pour);
		}
		if (IA_Trans)
		{
			EnhanceInputComponent->BindAction(IA_Trans, ETriggerEvent::Triggered, this, &APlayerCharacter::ActorTransform);
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

		if (HitActor->GetRootComponent()->GetCollisionObjectType() == ECC_GameTraceChannel1 && bCanPickUp)
		{
			PickUp(Hit);
			SetActorTickEnabled(true);
		}
		else if (HitActor->GetRootComponent()->GetCollisionObjectType() == ECC_GameTraceChannel2 && !bCanPickUp&&!bUsing)
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

		HandTarget->SetOwner(this);
		bCanPickUp = false;

	}
}

//调用放下
void APlayerCharacter::PutDown(const FHitResult& HitResult)
{
	
		OnHandTarget->Drop();
		OnHandTarget->GetGrabbedActor()->SetActorLocation(HitResult.ImpactPoint);
		OnRelease.Broadcast();
		HandTarget = nullptr;
		bCanPickUp = true;
	
}

//调用与桌子物体交互逻辑
void APlayerCharacter::Interaction()
{
	bUsing = true;
	//UE_LOG(LogTemp, Display, TEXT("PressedF"));
	IInteractive* IHandTarget = Cast<IInteractive>(HandTarget);
	RightHandInitLocation = RightHand->GetComponentLocation();
	if (IHandTarget&&InteractiveTarget&& InteractiveTarget->bContinue != true)
	{
		OnRelease.Broadcast();
		IHandTarget->AttachToPoint(HandTarget, InteractiveTarget);
		//LastInteractiveTarget = InteractiveTarget;
		
		//OnBeakerControl.Broadcast();
		UsingTarget = HandTarget;
		bCanPickUp = true;
	}

	//继续操作
	if(InteractiveTarget&& InteractiveTarget->bContinue == true)
	
	{
		switch (InteractiveTarget->SelfType)
		{
		case EInteractiveObjectType::Stirrer:
			//搅拌实现所需设置
			PC->SetMouseLocation(PC->CenterScreen.X, PC->CenterScreen.Y);
			PC->SetIgnoreLookInput(true);
			UsingTarget = InteractiveTarget;
			UsingTarget->ActorInitLocation = UsingTarget->GetRootComponent()->GetAttachParent()->GetComponentLocation();
			UsingTarget->bCanQuitInteractive = true;
			OnInteractiveChanged.Broadcast(NSLOCTEXT("Interactive", "Abort", "中止"));
			UsingTarget->bCanStirring = true;
			break;

		case EInteractiveObjectType::Beaker:
			PC->SwitchInputMapping(EMappingType::PourMappint);
			OnBeakerControl.Broadcast();
			OnInteractiveChanged.Broadcast(NSLOCTEXT("Interactive", "Abort", "中止"));
			UsingTarget = InteractiveTarget;
			UsingTarget->bCanQuitInteractive = true;
		}
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
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, DebugColor, false, 2.f, 0, 1.f);
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
		FVector2D OffSet = MousePos - PC->CenterScreen;	
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
		UsingTarget->SetActorTickLocation(UsingTarget, OffSet);
		
		//这个其实不应该放在这这里
		bCanPickUp = false;

		//Debug
		GEngine->AddOnScreenDebugMessage(2, 2.f, FColor::Green, FString::Printf(TEXT("Offset:%s"), *OffSet.ToString()));
		LastMousePos = MousePos;
	}
	
}

void APlayerCharacter::QuitInteractive()
{
	bUsing = false;
	if(UsingTarget->bCanQuitInteractive)
	{
		OnRelease.Broadcast();
		OnInteractiveChanged.Broadcast(FText::FromString("None"));
		UsingTarget->bCanStirring = false;
		PC->SetIgnoreLookInput(false);
		PC->SwitchInputMapping(EMappingType::InputMapping);
		UsingTarget->bCanQuitInteractive = false;
		bCanPickUp = true;
		UE_LOG(LogTemp, Display, TEXT("Stop Using"));
	}

}

void APlayerCharacter::Pour(const FInputActionValue& Value)
{
	UsingTarget->PourDowm(Value);
	UsingTarget->bCanQuitInteractive = true;
	
}

void APlayerCharacter::ActorTransform(const FInputActionValue& Value)
{
	UsingTarget->Translation(Value);
	UsingTarget->bCanQuitInteractive = true;
}







