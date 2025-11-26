// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"Engine/DataTable.h"
#include"TipsData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FTipsData : public FTableRowBase
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    FText TipsName;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D>Icon;
};