// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Engine/DataTable.h"
#include"ActorBase/InteractiveItemsBase.h"
#include"IneractiveItemsMatchData.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct FIneractiveItemsMatchData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EInteractiveObjectType HandType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EInteractiveObjectType TargetType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText NameOfUI;
};
