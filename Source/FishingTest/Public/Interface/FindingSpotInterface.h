// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FindingSpotInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFindingSpotInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FISHINGTEST_API IFindingSpotInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Fishing Interface")
	void OnFindingSpot(bool isSpoted);

};
