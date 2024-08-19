// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Fish.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AFish::AFish()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Mesh->SetupAttachment(Root);
}

// Called when the game starts or when spawned
void AFish::BeginPlay()
{
	Super::BeginPlay();

}

void AFish::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsCatching && BaitTarget)
	{
		FVector newLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), BaitTarget->GetComponentLocation(), DeltaTime, Speed);
		FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), BaitTarget->GetComponentLocation());
		SetActorLocation(newLocation);
		SetActorRotation(FRotator(newRotation.Pitch, newRotation.Yaw, newRotation.Roll));
	}
}

void AFish::StartCatching(UStaticMeshComponent* baitRef)
{
	IsCatching = true;
	IsMoving = true;
	BaitTarget = baitRef;
}

void AFish::StopCatching(bool isCatched)
{
	IsCatching = false;
	BaitTarget = nullptr;
	if (isCatched)
	{
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		IsMoving = false;
	}
	else
	{
		FVector randomLocation = FVector(FMath::RandRange(- 1.f, 1.f), FMath::RandRange(-1.f, 1.f), 0.f) * 300.f;
		GetWorld()->GetTimerManager().SetTimer(EscapeTimer, FTimerDelegate::CreateLambda([randomLocation = randomLocation, this] {
			FVector newLocation = UKismetMathLibrary::VInterpTo(GetActorLocation(), FVector(randomLocation.X, randomLocation.Y, GetActorLocation().Z), GetWorld()->DeltaTimeSeconds, Speed * 2.f);
			FRotator newRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), FVector(randomLocation.X, randomLocation.Y, GetActorLocation().Z));
			SetActorLocation(newLocation);
			SetActorRotation(FRotator(newRotation.Pitch, newRotation.Yaw - 180.f, newRotation.Roll));
		}), 0.01f, true);
	}
}

