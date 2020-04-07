// Fill out your copyright notice in the Description page of Project Settings.

#include "StateInfoWidget.h"
#include "Runtime/UMG/Public/UMG.h"

UStateInfoWidget::UStateInfoWidget(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{

}

void UStateInfoWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UStateInfoWidget::UpdateHealthInfo(int32 value)
{// check for nullptr and meaningful value
    if (HealthStatusText && value > 1)
    {// show if hidden
        if (HealthStatusText->Visibility == ESlateVisibility::Hidden)
        {
            HealthStatusText->SetVisibility(ESlateVisibility::Visible);
        }
        HealthStatusText->SetText(FText::FromString(FString::FromInt(value)));
    }
}
