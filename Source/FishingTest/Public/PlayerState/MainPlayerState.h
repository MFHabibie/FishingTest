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
	/// <summary>
	/// Return player state
	/// </summary>
	/// <returns>Reference to State enum</returns>
	FORCEINLINE State GetState() const { return State; }

	/// <summary>
	/// Update state of player state
	/// </summary>
	/// <param name="newState">: New updated state</param>
	FORCEINLINE void SetState(State newState) { State = newState; }

	/// <summary>
	/// Return player state when fishing
	/// </summary>
	/// <returns>Reference to FishingState enum</returns>
	FORCEINLINE FishingState GetFishingState() const { return FishingState; }

	/// <summary>
	/// Update fishing state
	/// </summary>
	/// <param name="newState">: New updated fishing state</param>
	FORCEINLINE void SetFishingState(FishingState newState) { FishingState = newState; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerState", meta = (AllowPrivateAccess = "true"))
	State State;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerState", meta = (AllowPrivateAccess = "true"))
	FishingState FishingState;
};
