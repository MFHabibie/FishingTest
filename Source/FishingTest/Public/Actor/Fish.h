// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Fish.generated.h"

class USceneComponent;
class USkeletalMeshComponent;

UCLASS()
class FISHINGTEST_API AFish : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Fish, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Fish, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> Mesh;

public:	
	// Sets default values for this actor's properties
	AFish();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

public:
	/// <summary>
	/// Call if fish start catching the bait
	/// </summary>
	/// <param name="baitRef">: Reference of the bait</param>
	void StartCatching(UStaticMeshComponent* baitRef);

	/// <summary>
	/// Make fish stop catching the bait
	/// </summary>
	/// <param name="isCatched">: True if catched, False if didn't catch the bait</param>
	void StopCatching(bool isCatched);

	/// <summary>
	/// Get fish moving status
	/// </summary>
	/// <returns>True if fish moving, False if not</returns>
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsFishMoving() const { return IsMoving; }

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Fish, meta = (AllowPrivateAccess = "true"))
	float Speed = 0.5f;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> BaitTarget;

	FTimerHandle EscapeTimer;
	FTimerHandle DissapearTimer;
	bool IsCatching = false;
	bool IsMoving = false;
};
