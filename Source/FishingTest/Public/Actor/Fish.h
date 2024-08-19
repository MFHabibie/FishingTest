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
	void StartCatching(UStaticMeshComponent* baitRef);
	void StopCatching(bool isCatched);

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
