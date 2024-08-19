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
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Fishing Interface")
	void OnCasting(bool isStarting);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Fishing Interface")
	void OnFishingCasting(float power);
};
