// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class UEMASTERFINALPROJECT_API AMainHUD : public AHUD
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void DrawHUD() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
    TSubclassOf<class UStateInfoWidget> InfoWidgetClass;

    UStateInfoWidget* InfoWidget;
};
