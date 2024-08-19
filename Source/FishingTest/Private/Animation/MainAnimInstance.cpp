// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

//local script
#include "FishingTest/FishingTestCharacter.h"
#include "PlayerState/MainPlayerState.h"

void UMainAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* owner = GetOwningActor())
	{
		OwnerCharacter = Cast<AFishingTestCharacter>(owner);
		if (OwnerCharacter)
		{
			OwnerCharacterMovement = OwnerCharacter->GetCharacterMovement();
		}
	}
}

void UMainAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (OwnerCharacter)
	{
		Velocity = OwnerCharacterMovement->Velocity;
		GroundSpeed = UKismetMathLibrary::VSizeXYSquared(Velocity);
		ShouldMove = GroundSpeed > 3.f && !UKismetMathLibrary::EqualEqual_VectorVector(OwnerCharacterMovement->GetCurrentAcceleration(), FVector::ZeroVector, 0.f);
		IsFalling = OwnerCharacterMovement->IsFalling();

		if (OwnerCharacter->GetPlayerState<AMainPlayerState>())
		{
			PlayerState = OwnerCharacter->GetPlayerState<AMainPlayerState>()->GetState();
			PlayerFishingState = OwnerCharacter->GetPlayerState<AMainPlayerState>()->GetFishingState();
		}
	}
}