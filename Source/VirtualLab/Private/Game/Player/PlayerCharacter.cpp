// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Player/PlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Interface/Grabbable.h"
#include "ActorBase/InteractiveItemsBase.h"


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

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorTickEnabled(false);
}


void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LineTrace(Hit);
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
			EnhanceInputComponent->BindAction(IA_RightHand, ETriggerEvent::Triggered, this, &APlayerCharacter::Use);
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

		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("X=%.2f,Y=%.2f"), InputValue.X, InputValue.Y));  Debug
    }
}

void APlayerCharacter::Use(const FInputActionValue& Value)
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

//抓取
void APlayerCharacter::PickUp(const FHitResult& HitResult)
{
	if(IGrabbable*GrabTarget=Cast<IGrabbable>(HitResult.GetActor()))
	{
		OnHandTarget = GrabTarget;
		GrabTarget->Grab(RightHand);
		OnGrab.Broadcast();
		bIsPickUp = false;
	}
}

//放下
void APlayerCharacter::PutDown(const FHitResult& HitResult)
{
	
		OnHandTarget->Drop();
		OnHandTarget->GetGrabbedActor()->SetActorLocation(HitResult.ImpactPoint);
		OnRelease.Broadcast();
		bIsPickUp = true;
	
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
		DrawDebugPoint(GetWorld(), OutHit.ImpactPoint, 10.0f, FColor::Yellow, false, 2.f);
		//if(OutHit.GetActor())
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("Hit Actor Name:%s"), *OutHit.GetActor()->GetName()));
		//}
		//UE_LOG(LogTemp, Display, TEXT("HitLocation:%s "), *Hit.ImpactPoint.ToString());

		return bHit;

}



