// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interface/FishingPowerInterface.h"
#include "Blueprint/UserWidget.h"
#include "BarMeterWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class FISHINGTEST_API UBarMeterWidget : public UUserWidget, public IFishingPowerInterface
{
	GENERATED_BODY()
	
public:
	void NativeConstruct();

protected:
	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Meter;

	UPROPERTY(meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UImage> Needle;

public:
	virtual void OnCasting_Implementation(bool isStarting) override;
	virtual void OnFishingCasting_Implementation(float power) override;

private:
	float ValueGap = 135.f;
};
