// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FishingPole.generated.h"


class UStaticMeshComponent;
class UCableComponent;
class AFish;

UCLASS()
class FISHINGTEST_API AFishingPole : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Fishing, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Pole;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Fishing, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCableComponent> String;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Fishing, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> BaitPlacement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Fishing, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Bait;
	
public:	
	// Sets default values for this actor's properties
	AFishingPole();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBaitHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	/// <summary>
	/// Attachment on bait to lock or unlock movement
	/// </summary>
	/// <param name="isAttaching">: True if lock bait, False if unlock</param>
	void BaitAttaching(bool isAttaching);

	/// <summary>
	/// Add force to bait
	/// </summary>
	/// <param name="force">: Force value in percent</param>
	void ThrowingBait(float force);

	/// <summary>
	/// Trigger when player pulling the fishing pole
	/// </summary>
	void Pulled();

	/// <summary>
	/// Condition if fishing pole is on pulling state
	/// </summary>
	/// <returns>True if fishing pole is pulling bait, False if not</returns>
	FORCEINLINE bool IsInUse() const { return IsPulled; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Fish, meta = (AllowPrivateAccess = "true"))
	float PullingSpeed = 0.5f;

	UPROPERTY()
	TObjectPtr<AFish> CatchedFish;

	FTimerHandle PullingTimer;
	FRotator BaitStartRotation;
	float RunningTime;
	bool IsPulled = false;
};
