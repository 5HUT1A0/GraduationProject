


#include "ActorComponent/BeakerWaterComponent.h"

UBeakerWaterComponent::UBeakerWaterComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void UBeakerWaterComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UBeakerWaterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

float UBeakerWaterComponent::CalculateFlowRate(float TiltAngle) const
{
	float Alpha = FMath::GetMappedRangeValueClamped(FVector2D( 30.f,80.f ), FVector2D( 0.f,34.f ), TiltAngle);

	Alpha *=  Alpha;
	return MaxFlowRate* Alpha;
}

float UBeakerWaterComponent::ConsumeWater(float DeltaTime, float FlowRate)
{
	if(CurrentVolume<=0.f)
	return 0.0f;

	float DeltaVolume = FlowRate * DeltaTime;
	float ActualUsed = FMath::Min(CurrentVolume, DeltaVolume);

	CurrentVolume = ActualUsed;
	return ActualUsed;
}

void UBeakerWaterComponent::AddWater(float Volume)
{
	CurrentVolume = FMath::Clamp(CurrentVolume += Volume, 0.f, MaxVolume);
}

