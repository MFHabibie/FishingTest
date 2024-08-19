// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FishingArea.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraFunctionLibrary.h"

//local script
#include "Actor/FishingPole.h"
#include "Actor/Fish.h"

// Sets default values
AFishingArea::AFishingArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	FishArea = CreateDefaultSubobject<UBoxComponent>(TEXT("Area"));
}

// Called when the game starts or when spawned
void AFishingArea::BeginPlay()
{
	Super::BeginPlay();
	
	FishArea->OnComponentBeginOverlap.AddDynamic(this, &AFishingArea::OnTriggerEnter);
	FishArea->OnComponentEndOverlap.AddDynamic(this, &AFishingArea::OnTriggerExit);
}

void AFishingArea::OnTriggerEnter(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("Bait")))
	{
		OnBaitAppear(true);
		BaitRef = Cast<UStaticMeshComponent>(OtherComp);
		if (RippleFX)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), RippleFX, BaitRef->GetComponentLocation(), FRotator::ZeroRotator);
		}
	}
}

void AFishingArea::OnTriggerExit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (Cast<AFishingPole>(OtherActor))
	{
		OnBaitAppear(false);
	}
}

void AFishingArea::OnBaitAppear(bool isAppearing)
{
	if (isAppearing)
	{
		FVector randomLocation = FVector(FMath::RandRange(-1.f, 1.f), FMath::RandRange(-1.f, 1.f), 0.f) * 200.f;
		GetWorld()->GetTimerManager().SetTimer(GenerateFishTimer, FTimerDelegate::CreateLambda([=, this] {
			FVector fishLocation = FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z) + randomLocation;
			FTransform newTransform;
			newTransform.SetLocation(fishLocation);
			ComingFish = GetWorld()->SpawnActor<AFish>(FishRef, newTransform, FActorSpawnParameters::FActorSpawnParameters());
			ComingFish->StartCatching(BaitRef);
			GetWorld()->GetTimerManager().ClearTimer(GenerateFishTimer);
		}), 3.f, false);
	}
	else
	{
		if (ComingFish && ComingFish->IsFishMoving())
		{
			ComingFish->StopCatching(false);
			GetWorld()->GetTimerManager().SetTimer(GenerateFishTimer, FTimerDelegate::CreateLambda([=, this] {
				BaitRef = nullptr;
				ComingFish->Destroy();
			}), 3.f, false);
		}
		GetWorld()->GetTimerManager().ClearTimer(GenerateFishTimer);
	}
}

