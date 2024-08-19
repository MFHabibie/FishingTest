// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnumData.h"
#include "GameFramework/PlayerState.h"
#include "MainPlayerState.generated.h"


/**
 * 
 */
UCLASS()
class FISHINGTEST_API AMainPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	FORCEINLINE State GetState() const { return State; }
	FORCEINLINE void SetState(State newState) { State = newState; }
	FORCEINLINE FishingState GetFishingState() const { return FishingState; }
	FORCEINLINE void SetFishingState(FishingState newState) { FishingState = newState; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerState", meta = (AllowPrivateAccess = "true"))
	State State;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerState", meta = (AllowPrivateAccess = "true"))
	FishingState FishingState;
};
