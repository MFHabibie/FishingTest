// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/MainPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);

	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {

		// Trigger one time
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMainPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMainPlayerController::StopJumping);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AMainPlayerController::StartInteract);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &AMainPlayerController::StopInteract);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Move);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AMainPlayerController::Interact);

	}
}

void AMainPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	FVector Vector(MovementVector.X, MovementVector.Y, 0);

	if (!OnMove.IsBound())
	{
		return;
	}

	OnMove.Broadcast(Vector);
}

void AMainPlayerController::Jump()
{
	if (!OnJump.IsBound())
	{
		return;
	}

	OnJump.Broadcast();
}

void AMainPlayerController::StopJumping()
{
	if (!OnJumpEnd.IsBound())
	{
		return;
	}

	OnJumpEnd.Broadcast();
}

void AMainPlayerController::StartInteract()
{
	if (!OnInteractStart.IsBound())
	{
		return;
	}

	OnInteractStart.Broadcast();
}

void AMainPlayerController::Interact()
{
	if (!OnInteract.IsBound())
	{
		return;
	}

	OnInteract.Broadcast();
}

void AMainPlayerController::StopInteract()
{
	if (!OnInteractEnd.IsBound())
	{
		return;
	}

	OnInteractEnd.Broadcast();
}
