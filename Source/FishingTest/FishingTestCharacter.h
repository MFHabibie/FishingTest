// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Interface/FindingSpotInterface.h"
#include "Interface/FishingPowerInterface.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "FishingTestCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
class UCurveFloat;
class AFishingPole;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class AFishingTestCharacter : public ACharacter, public IFindingSpotInterface, public IFishingPowerInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Fishing", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> FishingMeterDisplay;

public:
	AFishingTestCharacter();
	
protected:
	// To add mapping context
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime);

	UFUNCTION()
	void Move(FVector Value);

	UFUNCTION()
	void StartInteract();

	UFUNCTION()
	void Interact();

	UFUNCTION()
	void StopInteract();

	UFUNCTION()
	void EquipFishingPole();

	void Pulling();

	UFUNCTION()
	void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fishing", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFishingPole> FishingPoleRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fishing", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCurveFloat> CastingCurve;

public:
	virtual void OnFindingSpot_Implementation(bool isSpoted) override;
	virtual void OnCasting_Implementation(bool isStarting) override;
	virtual void OnFishingCasting_Implementation(float power) override; 

	FORCEINLINE TObjectPtr<AFishingPole> GetFishingPole() const { return FishingPole; }

private:
	float HoldThrowing();

	UPROPERTY()
	TObjectPtr<AFishingPole> FishingPole;

	UPROPERTY()
	TObjectPtr<AActor> InteractArea;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fishing", meta = (AllowPrivateAccess = "true"))
	float ThrowingForce = 300.f;

	FTimerHandle DelayStopCasting;
	bool IsOnFishingSpot = false;
	float RunningTime = 0.0f;
	float MaxRunningTime = 0.0f;
	float ThrowingValue = 0.f;
	float ForceCalculation;
};

