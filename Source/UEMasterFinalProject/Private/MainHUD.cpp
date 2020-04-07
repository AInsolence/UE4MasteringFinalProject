// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"
#include "StateInfoWidget.h"

void AMainHUD::BeginPlay()
{
	Super::BeginPlay(); 
	if (InfoWidgetClass)
	{
		InfoWidget = CreateWidget<UStateInfoWidget>(GetWorld(), InfoWidgetClass);
		if (InfoWidget)
		{
			InfoWidget->AddToViewport();
		}
	}
}

void AMainHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainHUD::DrawHUD()
{
	Super::DrawHUD();
}
