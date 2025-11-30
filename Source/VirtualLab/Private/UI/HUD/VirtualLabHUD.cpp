// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/VirtualLabHUD.h"
#include"UI/WidgetController/OverlapWidgetController.h"
#include"UI/UserWidget/VirtualLabUserWidget.h"
#include"Game/Player/VirtualLabPlayerController.h"


UOverlapWidgetController* AVirtualLabHUD::GetOverlapWidgetController()
{
	if (OverlapWidgetController==nullptr)
	{
		AVirtualLabPlayerController* PC = Cast<AVirtualLabPlayerController>(GetOwningPlayerController());
		OverlapWidgetController = NewObject<UOverlapWidgetController>(PC, OverlapWidgetControllerClass);
		OverlapWidgetController->BindCallBackToDependencies();

		return OverlapWidgetController;
	}
	return OverlapWidgetController;
}


void AVirtualLabHUD::BeginPlay()
{
	Super::BeginPlay();


	InitUserWidget();
}

void AVirtualLabHUD::InitUserWidget()
{
	//检查初始化
	checkf(OverlapWidgetControllerClass, TEXT("覆盖组件类初始化失败，请填入BP_OverlapWidgetController"));
	checkf(OverlapWidgetClass, TEXT("覆盖组件控制器类初始化失败，请填入BP_OverlapWidget"));

	//创建实例
	OverlapWidget = CreateWidget<UVirtualLabUserWidget>(GetWorld(), OverlapWidgetClass);

	//绑定玩家控制器
	UOverlapWidgetController* OWController = GetOverlapWidgetController();
	OverlapWidget->SetWidgetController(OWController);
	//UE_LOG(LogTemp, Warning, TEXT("Bind To Controller Ptr in HUD: %p"), OWController);
	OverlapWidgetController->BroadcastInitialValues();
	
	//添加到视口
	OverlapWidget->AddToViewport();

	
}


