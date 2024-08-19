// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FishingPowerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFishingPowerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FISHINGTEST_API IFishingPowerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/// <summary>
	/// Interface using as condition when start or stop fishing
	/// </summary>
	/// <param name="isStarting">: Condition start when True, stop when False</param>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Fishing Interface")
	void OnCasting(bool isStarting);

	/// <summary>
	/// Interface using as holding input when charging throw fishing pole
	/// </summary>
	/// <param name="power">: Force in percent value</param>
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Fishing Interface")
	void OnFishingCasting(float power);
};
