// Fill out your copyright notice in the Description page of Project Settings.

/*
	TEST class used to create HUD example was implemented only with C++

	!NOTE: to use this class you need to uncomment:
		// PrivateDependencyModuleNames.AddRange(new string[] { "UMG", "Slate", "SlateCore" });
	in UEMasterFinalProject.Build.cs
*/

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
};
