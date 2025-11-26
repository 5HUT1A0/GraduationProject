// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
//#include"VirtualLabDelegates/VirtualLabDelegates.h"
#include "VirtualLabWidgetControllerBase.generated.h"

/**
 * 
 */


UCLASS()
class VIRTUALLAB_API UVirtualLabWidgetControllerBase : public UObject
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues();
	virtual void BindCallBackToDependencies();
	
};
