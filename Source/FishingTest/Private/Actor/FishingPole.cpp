// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/FishingPole.h"
#include "Components/StaticMeshComponent.h"
#include "CableComponent.h"
#include "Kismet/KismetMathLibrary.h"

//local script
#include "Actor/Fish.h"

// Sets default values
AFishingPole::AFishingPole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Pole = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pole"));
	RootComponent = Pole;

	String = CreateDefaultSubobject<UCableComponent>(TEXT("String"));
	String->SetupAttachment(Pole);

	BaitPlacement = CreateDefaultSubobject<USceneComponent>(TEXT("BaitLocation"));
	BaitPlacement->SetupAttachment(Pole);

	Bait = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bait"));
	Bait->SetupAttachment(BaitPlacement);
}

// Called when the game starts or when spawned
void AFishingPole::BeginPlay()
{
	Super::BeginPlay();
	
	Bait->OnComponentHit.AddDynamic(this, &AFishingPole::OnBaitHit);
	BaitStartRotation = Bait->GetComponentRotation();
	BaitAttaching(true);
}

void AFishingPole::OnBaitHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (AFish* fish = Cast<AFish>(OtherActor))
	{
		CatchedFish = fish;
		CatchedFish->StopCatching(true);
		CatchedFish->AttachToComponent(Bait, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void AFishingPole::BaitAttaching(bool isAttaching)
{
	Bait->SetSimulatePhysics(!isAttaching);
	if (isAttaching)
	{
		Bait->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Bait->SetWorldRotation(BaitStartRotation);
	}
}

void AFishingPole::ThrowingBait(float force)
{
	Bait->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Bait->AddImpulse((GetOwner()->GetActorForwardVector() * force * 0.5f) + (GetOwner()->GetActorUpVector() * force * 5.f));
}

void AFishingPole::Pulled()
{
	IsPulled = true; 
	Bait->AddImpulse(GetOwner()->GetActorUpVector() * 30.f);
	GetWorld()->GetTimerManager().SetTimer(PullingTimer, FTimerDelegate::CreateLambda([=, this] {
		RunningTime += 0.01f;
		if (RunningTime > 0.4f)
		{
			if (Bait->IsSimulatingPhysics())
			{
				BaitAttaching(true);
			}
			FVector updatedLocation = UKismetMathLibrary::VInterpTo(Bait->GetComponentLocation(), BaitPlacement->GetComponentLocation(), GetWorld()->DeltaTimeSeconds, PullingSpeed);
			Bait->SetWorldLocation(updatedLocation);
			if (UKismetMathLibrary::Vector_Distance(Bait->GetComponentLocation(), BaitPlacement->GetComponentLocation()) < 5.f)
			{
				if (CatchedFish)
				{
					CatchedFish->Destroy();
				}
				IsPulled = false;
				GetWorld()->GetTimerManager().ClearTimer(PullingTimer);
			}
		}
	}), 0.01f, true);
}

