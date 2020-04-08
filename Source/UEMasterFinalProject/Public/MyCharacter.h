// Fill out your copyright notice in the Description page of Project Settings.
/*

This class represents main character with its abilities

*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UArrowComponent;

UCLASS()
class UEMASTERFINALPROJECT_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Components
	UPROPERTY(EditDefaultsOnly, Category = "BaseCharacter")
	USpringArmComponent* SpringArm = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "BaseCharacter")
	UCameraComponent* Camera = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "BaseCharacter")
	UArrowComponent* ForwardDirection = nullptr;
	// Property for determing whether the firing action has been initiating -- used for animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimationSettings")
	bool bIsFiring = false;
	// Firing effect emitter
	UPROPERTY(EditDefaultsOnly, Category = "BaseCharacter")
	UParticleSystemComponent* FiringEffectEmitter = nullptr;
	// Firing sound -- set in editor inside character BP
	UPROPERTY(EditDefaultsOnly, Category = "BaseCharacter")
	USoundBase* FireSound = nullptr;
	// Sprint implementation properties
	UPROPERTY(EditDefaultsOnly, Category = "BaseCharacter")
	bool bIsSprinting = false;
	UPROPERTY(EditDefaultsOnly, Category = "BaseCharacter")
	float BaseRunSpeed = 300.f;
	UPROPERTY(EditDefaultsOnly, Category = "BaseCharacter")
	float TimeToMaxSpeed = 2.f;
	UPROPERTY(EditDefaultsOnly, Category = "BaseCharacter")
	float MaxSprintMultiplier = 2.f;
	float CurrentSprintMultiplier = 1.f;
	// Input action functions
	void MoveForwardBackward(float Amount);
	void MoveRightLeft(float Amount);
	void RotateCamera(float Amount);
	void ChangeCameraHeight(float Amount);
	void StartSprint();
	void EndSprint();
	// Firing functions divided by two parts to use in anim notify
	UFUNCTION(BlueprintCallable, Category = "AnimationSettings")
	void FireStart();
	UFUNCTION(BlueprintCallable, Category = "AnimationSettings")
	void Fired();
	
};
