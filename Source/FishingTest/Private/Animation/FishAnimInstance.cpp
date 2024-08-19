// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/FishAnimInstance.h"

//local script
#include "Actor/Fish.h"

void UFishAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* owner = GetOwningActor())
	{
		OwnerActor = Cast<AFish>(owner);
	}
}

void UFishAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (OwnerActor)
	{
		IsMoving = OwnerActor->IsFishMoving();
	}
}
