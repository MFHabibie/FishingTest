// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BarMeterWidget.h"
#include "Components/Image.h"

void UBarMeterWidget::NativeConstruct()
{
	UUserWidget::NativeConstruct();

}

void UBarMeterWidget::OnCasting_Implementation(bool isStarting)
{
	if (isStarting)
	{
		Needle->SetRenderTranslation(FVector2D(0.f, 0.f));
	}
}

void UBarMeterWidget::OnFishingCasting_Implementation(float power)
{
	float barX = ValueGap * power;
	Needle->SetRenderTranslation(FVector2D(barX, 0.f));
}
