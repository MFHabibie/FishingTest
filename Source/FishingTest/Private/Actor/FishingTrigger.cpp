// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FishingTrigger.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "NiagaraComponent.h"


// Sets default values
AFishingTrigger::AFishingTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TriggerArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Area"));
	RootComponent = TriggerArea;

	TriggerDisplay = CreateDefaultSubobject<UWidgetComponent>(TEXT("Display"));
	TriggerDisplay->SetupAttachment(TriggerArea);

	AreaEffect = CreateDefaultSubobject<UNiagaraComponent>("Effect");
	AreaEffect->SetupAttachment(TriggerArea);
}

// Called when the game starts or when spawned
void AFishingTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerDisplay->SetVisibility(false);

	TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &AFishingTrigger::OnTriggerEnter);
	TriggerArea->OnComponentEndOverlap.AddDynamic(this, &AFishingTrigger::OnTriggerExit);
}

void AFishingTrigger::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()->ImplementsInterface(UFindingSpotInterface::StaticClass()))
	{
		IFindingSpotInterface::Execute_OnFindingSpot(OtherActor, true);
		IFindingSpotInterface::Execute_OnFindingSpot(this, true);
	}
}

void AFishingTrigger::OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->GetClass()->ImplementsInterface(UFindingSpotInterface::StaticClass()))
	{
		IFindingSpotInterface::Execute_OnFindingSpot(OtherActor, false);
		IFindingSpotInterface::Execute_OnFindingSpot(this, false);
	}
}

void AFishingTrigger::OnFindingSpot_Implementation(bool isSpoted)
{
	TriggerDisplay->SetVisibility(isSpoted);
}

