// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDPlayerController.h"
#include "SlateBasics.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Engine.h"
#include "Runtime/Core/Public/Templates/SharedPointer.h"

void AHUDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	TSharedRef<SVerticalBox> WidgetVLayout = SNew(SVerticalBox) +
		SVerticalBox::Slot().HAlign(HAlign_Center).VAlign(VAlign_Center)
			[SNew(SButton).Content()
				[SNew(STextBlock).Text(FText::FromString(TEXT("Test button")))]
			];
	if (GEngine)
	{// add created widget to the viewport
		GEngine->GameViewport->AddViewportWidgetForPlayer(GetLocalPlayer(), WidgetVLayout, 0);
	}
}