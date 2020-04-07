// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StateInfoWidget.generated.h"

/**
 *
 */
UCLASS()
class UEMASTERFINALPROJECT_API UStateInfoWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UStateInfoWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* HealthStatusText;
	UFUNCTION()
		void UpdateHealthInfo(int32 value);
};