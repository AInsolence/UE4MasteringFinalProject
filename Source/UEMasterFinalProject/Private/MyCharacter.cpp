// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Engine/Classes/Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	///*** Setup custom components ***///
	// Setup spring arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);
	// Set absolute spring rotation. Pawn froward direction is independent
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->bAbsoluteRotation = true;
	// Define distance between character and camera
	SpringArm->TargetArmLength = 600.f;
	// Setup camera
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	// Set camera view angle
	Camera->FieldOfView = 110.f;
	// Setup direction arrow
	ForwardDirection = CreateDefaultSubobject<UArrowComponent>("ForwardDirectionArrow");
	ForwardDirection->SetupAttachment(RootComponent);

	// Setup to avoid Self-Response
	GetMesh()->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);

	///*** Character movement settings ***///
	// Allow the character to walk up stairs
	GetCharacterMovement()->SetWalkableFloorAngle(50.f);
	GetCharacterMovement()->MaxStepHeight = 40.f;

	// Jump tweaking
	JumpMaxHoldTime = 0.7f;
	// Set air control ability
	GetCharacterMovement()->AirControl = 0.5f; // counted as ratio of the standart control
	GetCharacterMovement()->JumpZVelocity = 800.f; // control jump height through this parameter
	GetCharacterMovement()->GravityScale = 1.5f;
	JumpMaxCount = 2; // set double jumping

	// Set base walking speed to use in sprint functionality
	GetCharacterMovement()->MaxWalkSpeed = BaseRunSpeed;

	// Firing emitter settings
	FiringEffectEmitter = CreateDefaultSubobject<UParticleSystemComponent>("FiringEffectEmitter");
	FiringEffectEmitter->bAutoActivate = false;
	FiringEffectEmitter->SetCollisionResponseToChannel(ECC_Destructible, ECR_Ignore);
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Turn spring arm under the ground
	SpringArm->SetRelativeRotation(FQuat::MakeFromEuler(FVector(0, -25, 0)));
	// Activate camera
	Camera->Activate();
	// Ensure charcter is always facing the movements direction
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void AMyCharacter::MoveForwardBackward(float Amount)
{
	// Ensure controller is valid and the input exists
	if (Controller && Amount)
	{// Add movement in the camera facing direction
		AddMovementInput(Camera->GetForwardVector(), Amount);
	}
}

void AMyCharacter::MoveRightLeft(float Amount)
{
	// Ensure controller is valid and the input exists
	if (Controller && Amount)
	{// Add movement in the camera right direction
		AddMovementInput(Camera->GetRightVector(), Amount);
	}
}

void AMyCharacter::RotateCamera(float Amount)
{ 
	// Ensure controller is valid and the input exists
	if (Controller && Amount)
	{// Get current spring arm rotation
		FVector CurrentRotation = SpringArm->GetComponentRotation().Euler();
		// Add the amount of the rotation vector
		CurrentRotation += FVector(0, 0, Amount);
		// Set new rotation in the world
		SpringArm->SetWorldRotation(FQuat::MakeFromEuler(CurrentRotation));
	}
}

void AMyCharacter::ChangeCameraHeight(float Amount)
{
	// Ensure controller is valid and the input exists
	if (Controller && Amount)
	{// Get current spring arm rotation
		FVector CurrentRotation = SpringArm->GetComponentRotation().Euler();
		// Extract height in separate variable
		float Height = CurrentRotation.Y;
		// Change it by the amount value
		Height += Amount;
		// Rebuild the rotation vector with 0 for X and new height for Y
		CurrentRotation = FVector(0, Height, CurrentRotation.Z);
		// Set new world rotation
		SpringArm->SetWorldRotation(FQuat::MakeFromEuler(CurrentRotation));
	}
}

void AMyCharacter::StartSprint()
{
	bIsSprinting = true;
}

void AMyCharacter::EndSprint()
{
	bIsSprinting = false;
}

void AMyCharacter::FireStart()
{
	if (!bIsFiring)
	{
		bIsFiring = true;
	}
}

void AMyCharacter::Fired()
{
	if (!bIsFiring)
	{
		return;
	}
	bIsFiring = false;
	FVector HandLocation = GetMesh()->GetBoneLocation(TEXT("LeftHandMiddle1"));
	// Raycasting properties  
	float RayDistance = 10000.f;
	FVector RayDirection = ForwardDirection->GetForwardVector();
	FVector RayStart = HandLocation;
	FVector RayEnd = RayStart + (RayDirection * RayDistance);
	FHitResult outHitresult;
	auto Color = FColor::MakeRandomColor();
	bool bHasHitSmth = GetWorld()->LineTraceSingleByChannel(outHitresult, 
															RayStart, 
															RayEnd, 
															ECollisionChannel::ECC_Destructible);
	if (FiringEffectEmitter)
	{// Emit firing effect
		FiringEffectEmitter->SetWorldLocation(HandLocation + (RayDirection * 60.f));
		FiringEffectEmitter->Activate(true);
	}

	if (bHasHitSmth)
	{
		DrawDebugLine(GetWorld(), RayStart, outHitresult.ImpactPoint, Color, false, 0.5f, 0, 15.f);
		// Hit the movable actors
		if (outHitresult.GetActor()->IsRootComponentMovable())
		{
			TArray<UStaticMeshComponent*> outComponentsList;
			// Get all static mesh components of the hitted actor
			outHitresult.GetActor()->GetComponents<UStaticMeshComponent>(outComponentsList);

			for (auto& mesh : outComponentsList)
			{
				mesh->AddForce(RayDirection * 100000000.f);
			}
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), RayStart, RayEnd, Color, false, 0.5f, 0, 15.f);
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float RampThisFrame = (DeltaTime / TimeToMaxSpeed) * MaxSprintMultiplier;

	if (bIsSprinting)
	{
		CurrentSprintMultiplier += RampThisFrame;
	}
	else
	{
		CurrentSprintMultiplier -= RampThisFrame;
	}

	CurrentSprintMultiplier = FMath::Clamp(CurrentSprintMultiplier, 1.f, MaxSprintMultiplier);
	GetCharacterMovement()->MaxWalkSpeed = BaseRunSpeed * CurrentSprintMultiplier;
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind action mappings
	InputComponent->BindAction("Fire", 
								EInputEvent::IE_Pressed, 
								this, 
								&AMyCharacter::FireStart);
	InputComponent->BindAction("Jump",
								EInputEvent::IE_Pressed,
								this,
								&AMyCharacter::Jump);
	InputComponent->BindAction("Jump",
								EInputEvent::IE_Released,
								this,
								&AMyCharacter::StopJumping);
	InputComponent->BindAction("Sprint",
								EInputEvent::IE_Pressed,
								this,
								&AMyCharacter::StartSprint);
	InputComponent->BindAction("Sprint",
								EInputEvent::IE_Released,
								this,
								&AMyCharacter::EndSprint);

	// Bind the axis mappings
	InputComponent->BindAxis("MoveForwardBackward",
							  this,
							  &AMyCharacter::MoveForwardBackward);
	InputComponent->BindAxis("MoveRightLeft",
							  this,
							  &AMyCharacter::MoveRightLeft);
	InputComponent->BindAxis("RotateCamera",
							  this,
							  &AMyCharacter::RotateCamera);
	InputComponent->BindAxis("ChangeCameraHeight",
							  this,
							  &AMyCharacter::ChangeCameraHeight);
}
