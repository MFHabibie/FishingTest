#pragma once

#include "CoreMinimal.h"
#include "EnumData.generated.h"

UENUM(BlueprintType)
enum class State : uint8
{
	Default	UMETA(DisplayName = "Default"),
	Fishing	UMETA(DisplayName = "Fishing")
};

UENUM(BlueprintType)
enum class FishingState : uint8
{
	Idle	UMETA(DisplayName = "Idle"),
	Start	UMETA(DisplayName = "Start"),
	Throw	UMETA(DisplayName = "Throw"),
	Pull	UMETA(DisplayName = "Pull")
};