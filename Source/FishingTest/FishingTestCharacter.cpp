// Copyright Epic Games, Inc. All Rights Reserved.

#include "FishingTestCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "Components/WidgetComponent.h"

//local script
#include "PlayerController/MainPlayerController.h"
#include "PlayerState/MainPlayerState.h"
#include "Actor/FishingPole.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AFishingTestCharacter

AFishingTestCharacter::AFishingTestCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Create widget component
	FishingMeterDisplay = CreateDefaultSubobject<UWidgetComponent>("FishingMeterDisplay");
	FishingMeterDisplay->SetupAttachment(RootComponent);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AFishingTestCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Binding input from player controller
	if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController()))
	{
		PlayerController->OnJump.AddDynamic(this, &ACharacter::Jump);
		PlayerController->OnJumpEnd.AddDynamic(this, &ACharacter::StopJumping);
		PlayerController->OnInteractStart.AddDynamic(this, &AFishingTestCharacter::StartInteract);
		PlayerController->OnInteractEnd.AddDynamic(this, &AFishingTestCharacter::StopInteract);

		PlayerController->OnMove.AddDynamic(this, &AFishingTestCharacter::Move);
		PlayerController->OnInteract.AddDynamic(this, &AFishingTestCharacter::Interact);
	}

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AFishingTestCharacter::OnTriggerEnter);
	GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &AFishingTestCharacter::OnTriggerExit);
	FishingMeterDisplay->SetVisibility(false);
	CastingCurve->GetTimeRange(RunningTime, MaxRunningTime);

	EquipFishingPole();
}

void AFishingTestCharacter::Tick(float DeltaTime)
{
	if (GetPlayerState<AMainPlayerState>()->GetFishingState() == FishingState::Start)
	{
		HoldThrowing();
	}
}

void AFishingTestCharacter::OnFindingSpot_Implementation(bool isSpoted)
{
	IsOnFishingSpot = isSpoted;
}

void AFishingTestCharacter::OnCasting_Implementation(bool isStarting)
{
	if (isStarting)
	{
		GetPlayerState<AMainPlayerState>()->SetState(State::Fishing);
		GetPlayerState<AMainPlayerState>()->SetFishingState(FishingState::Start);
		FishingMeterDisplay->SetVisibility(true);
		FishingPole->BaitAttaching(false);
		if (InteractArea)
		{
			IFindingSpotInterface::Execute_OnFindingSpot(InteractArea, false);
		}
	}
	else
	{
		RunningTime = 0.f;
		ThrowingValue = 0.f;
		FishingPole->ThrowingBait(ForceCalculation);
		GetPlayerState<AMainPlayerState>()->SetFishingState(FishingState::Throw);

		GetWorld()->GetTimerManager().SetTimer(DelayStopCasting, FTimerDelegate::CreateLambda([this] {
			FishingMeterDisplay->SetVisibility(false);
			GetPlayerState<AMainPlayerState>()->SetFishingState(FishingState::Idle);
			GetWorld()->GetTimerManager().ClearTimer(DelayStopCasting);
		}), 2.f, false);
	}
}

void AFishingTestCharacter::OnFishingCasting_Implementation(float power)
{
	ForceCalculation = FMath::Clamp(ThrowingForce * power, 100.f, 1000.f);
}

float AFishingTestCharacter::HoldThrowing()
{
	RunningTime += GetWorld()->DeltaTimeSeconds;
	ThrowingValue = CastingCurve->GetFloatValue(RunningTime);
	if (RunningTime > MaxRunningTime)
	{
		RunningTime = 0.f;
	}

	return ThrowingValue;
}

void AFishingTestCharacter::Move(FVector Value)
{
	if (GetPlayerState<AMainPlayerState>()->GetState() != State::Default)
	{
		return;
	}

	// input is a Vector2D
	FVector MovementVector = Value;

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AFishingTestCharacter::StartInteract()
{
	if (IsOnFishingSpot)
	{
		if (GetPlayerState<AMainPlayerState>()->GetState() == State::Default)
		{
			if (!FishingPole->IsInUse())
			{
				IFishingPowerInterface::Execute_OnCasting(this, true);
				if (FishingMeterDisplay->GetWidget()->GetClass()->ImplementsInterface(UFishingPowerInterface::StaticClass()))
				{
					IFishingPowerInterface::Execute_OnCasting(FishingMeterDisplay->GetWidget(), true);
				}
			}
		}
		else if (GetPlayerState<AMainPlayerState>()->GetState() == State::Fishing)
		{
			if (GetPlayerState<AMainPlayerState>()->GetFishingState() == FishingState::Idle)
			{
				Pulling();
			}
		}
	}
}

void AFishingTestCharacter::Interact()
{
	if (IsOnFishingSpot)
	{
		if (GetPlayerState<AMainPlayerState>()->GetFishingState() == FishingState::Start)
		{
			IFishingPowerInterface::Execute_OnFishingCasting(this, ThrowingValue);
			if (FishingMeterDisplay->GetWidget()->GetClass()->ImplementsInterface(UFishingPowerInterface::StaticClass()))
			{
				IFishingPowerInterface::Execute_OnFishingCasting(FishingMeterDisplay->GetWidget(), ThrowingValue);
			}
		}
		
	}
}

void AFishingTestCharacter::StopInteract()
{
	if (IsOnFishingSpot)
	{
		if (GetPlayerState<AMainPlayerState>()->GetState() == State::Fishing)
		{
			if (GetPlayerState<AMainPlayerState>()->GetFishingState() == FishingState::Start)
			{
				IFishingPowerInterface::Execute_OnCasting(this, false);
				if (FishingMeterDisplay->GetWidget()->GetClass()->ImplementsInterface(UFishingPowerInterface::StaticClass()))
				{
					IFishingPowerInterface::Execute_OnCasting(FishingMeterDisplay->GetWidget(), false);
				}
			}
		}
	}
}

void AFishingTestCharacter::EquipFishingPole()
{
	FActorSpawnParameters spawnParameter;
	spawnParameter.Owner = this;
	FishingPole = GetWorld()->SpawnActor<AFishingPole>(FishingPoleRef, spawnParameter);
	FishingPole->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("FishingPoleSocket"));
}

void AFishingTestCharacter::Pulling()
{
	GetPlayerState<AMainPlayerState>()->SetFishingState(FishingState::Pull);
	FishingPole->Pulled();

	GetWorld()->GetTimerManager().SetTimer(DelayStopCasting, FTimerDelegate::CreateLambda([this] {
		GetPlayerState<AMainPlayerState>()->SetState(State::Default);
		if (InteractArea)
		{
			IFindingSpotInterface::Execute_OnFindingSpot(InteractArea, true);
		}
		GetWorld()->GetTimerManager().ClearTimer(DelayStopCasting);
	}), 2.5f, false);
}

void AFishingTestCharacter::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UFindingSpotInterface::StaticClass()))
	{
		InteractArea = OtherActor;
	}
}

void AFishingTestCharacter::OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->ImplementsInterface(UFindingSpotInterface::StaticClass()))
	{
		InteractArea = nullptr;
	}
}
