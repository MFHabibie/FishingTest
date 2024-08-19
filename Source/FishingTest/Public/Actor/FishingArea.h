// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/FishingInterface.h"
#include "GameFramework/Actor.h"
#include "FishingArea.generated.h"


class UBoxComponent;
class AFish;
class UNiagaraSystem;

UCLASS()
class FISHINGTEST_API AFishingArea : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Fish, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> FishArea;
	
public:	
	// Sets default values for this actor's properties
	AFishingArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/// <summary>
	/// Trigger when bait appear on fishing area
	/// </summary>
	/// <param name="isAppearing">: True when bait appear, False when bait dissapear</param>
	void OnBaitAppear(bool isAppearing);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fish", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AFish> FishRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Fish | VFX", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraSystem> RippleFX;

private:
	UPROPERTY()
	TObjectPtr<AFish> ComingFish;

	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> BaitRef;

	FTimerHandle GenerateFishTimer;
	FVector BaitLocation;

};
