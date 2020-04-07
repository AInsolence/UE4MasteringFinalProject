// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HUDPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class UEMASTERFINALPROJECT_API AHUDPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	UPROPERTY()
	FTimerHandle HUDToggleTimer;
};
