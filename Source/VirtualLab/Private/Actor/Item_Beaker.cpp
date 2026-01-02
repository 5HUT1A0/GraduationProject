// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Item_Beaker.h"
#include"Game/Player/VirtualLabPlayerController.h"
#include"Game/Player/PlayerCharacter.h"
#include"ActorComponent/BeakerWaterComponent.h"
#include"NiagaraComponent.h"

AItem_Beaker::AItem_Beaker()
{
	PrimaryActorTick.bCanEverTick = true;

	PointForBeaker = CreateDefaultSubobject<USceneComponent>(TEXT("PointForBeaker"));
	PointForBeaker->SetupAttachment(RootComponent);

	WaterComponent = CreateDefaultSubobject<UBeakerWaterComponent>(TEXT("WaterComponent"));
	//WaterComponent->SetupAttachment(RootComponent);

	PourWaterFX = CreateDefaultSubobject<UNiagaraComponent>(TEXT("WaterNiagaraComponent"));
	PourWaterFX->SetupAttachment(RootComponent);

}



void AItem_Beaker::AttachToPoint( AInteractiveItemsBase* HandTarget,  AInteractiveItemsBase* OutTarget)
{
	if (OutTarget && HandTarget->bCanInteractive)
	{
		//更新TargetBeaker
		TargetBeaker = Cast<AItem_Beaker>(HandTarget);

		//取消与手部结合
		HandTarget->GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);

		//切换输入上下文
		PC->SwitchInputMapping(EMappingType::PourMappint);

		//结合后的位置、旋转设置
		FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, false);
		Mesh->SetAbsolute(false, false, true);

		if (Cast<AItem_Beaker>(OutTarget))
		{
			 const AItem_Beaker* OutBeaker= Cast<AItem_Beaker>(OutTarget);
			AttachToComponent(OutBeaker->PointForBeaker, Rules);
		}
		else
		{
			AttachToComponent(OutTarget->GetJoinPoint(), Rules);
		}
		Mesh->SetWorldRotation(AfterAttchRotation);
		Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		//倾斜实现所需设置
		PC->SetIgnoreLookInput(true);

		//广播更新UI
		Player = Cast<APlayerCharacter>(GetOwner());
		Player->OnBeakerControl.Broadcast();
		Player->OnInteractiveChanged.Broadcast(NSLOCTEXT("Interactive", "Abort", "中止"));

		
		//HasAttachPoint();  //测试

		//相关布尔值设置
		HandTarget->bCanInteractive = false;
		
		HandTarget->bCanShowUI = false;
		OutTarget->bCanShowUI = true;
		
		bCanQuitInteractive = true;

		HandTarget->bContinue = true;
		//OutTarget->bContinue = true;

		
	}
	
}

void AItem_Beaker::BeginPlay()
{
	Super::BeginPlay();

	PC = Cast<AVirtualLabPlayerController>(GetWorld()->GetFirstPlayerController());
}

void AItem_Beaker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TargetBeaker)return;
	
	float TiltAngle = GetTiltAngle();

	float FlowRate = WaterComponent->CalculateFlowRate(TiltAngle);

	//停止粒子效果实现
	if (FlowRate <= 0.f)
	{
		PourWaterFX->Deactivate();
		return;
	}

	PourWaterFX->Activate();

	float UseVolume = WaterComponent->ConsumeWater(DeltaTime, FlowRate);

	TargetBeaker->WaterComponent->AddWater(UseVolume);

	PourWaterFX->SetFloatParameter(TEXT("SpawnRate"), FlowRate);
	
}

//附着后才激活
void AItem_Beaker::HasAttachPoint(AInteractiveItemsBase* CheckTarget)
{
	//附着物
		if (bBeingAttached(CheckTarget))
		{
			//广播更新UI
			Player = Cast<APlayerCharacter>(GetOwner());
			Player->OnInteractiveChanged.Broadcast(NSLOCTEXT("Interactive", "Abort", "继续操作"));
		}
	//被附着物
		else
		{
			if (!JointPoint->GetAttachChildren().IsEmpty() && !PointForBeaker->GetAttachChildren().IsEmpty())
			{
				CheckTarget->bCanInteractive = false;
				CheckTarget->bCanShowUI = false;
				//CheckTarget->bNeedCheckPoint = false;
			}
			CheckTarget->bCanInteractive = true;
		}
	
}

void AItem_Beaker::PourDowm(const FInputActionValue& Value)
{
	float Strength = Value.Get<float>(); // W = 1, 松开 = 0

	if (FMath::IsNearlyZero(Strength))
		return;

	FRotator DeltaRot( Strength * 20 * GetWorld()->GetDeltaSeconds(),0.f,0.f);
	AddActorLocalRotation(DeltaRot);

}

void AItem_Beaker::Translation(const FInputActionValue& Value)
{
	float Strength = Value.Get<float>();

	FVector Translation( Strength * 20 * GetWorld()->GetDeltaSeconds(), 0.0, 0.0);
	FTransform DeltaTran;
	DeltaTran.SetTranslation(Translation);

	AddActorWorldTransform(DeltaTran);
}

float AItem_Beaker::GetTiltAngle() const
{
	const FRotator Rot = GetActorRotation();
	return FMath::Abs(FRotator::NormalizeAxis(Rot.Pitch));
}




