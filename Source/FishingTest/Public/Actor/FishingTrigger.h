// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/FindingSpotInterface.h"
#include "GameFramework/Actor.h"
#include "FishingTrigger.generated.h"


class UBoxComponent;
class UWidgetComponent;
class UNiagaraComponent;

UCLASS()
class FISHINGTEST_API AFishingTrigger : public AActor, public IFindingSpotInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFishingTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> TriggerArea;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UWidgetComponent> TriggerDisplay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UNiagaraComponent> AreaEffect;

public:
	UFUNCTION()
	void OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual void OnFindingSpot_Implementation(bool isSpoted) override;
};
