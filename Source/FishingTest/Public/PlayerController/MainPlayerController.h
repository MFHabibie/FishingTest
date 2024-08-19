// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMove, FVector, NewVector);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJump);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnJumpEnd);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteract);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractEnd);

/**
 * 
 */
UCLASS()
class FISHINGTEST_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnMove OnMove;

	UPROPERTY(BlueprintAssignable)
	FOnJump OnJump;

	UPROPERTY(BlueprintAssignable)
	FOnJumpEnd OnJumpEnd;

	UPROPERTY(BlueprintAssignable)
	FOnInteractStart OnInteractStart;

	UPROPERTY(BlueprintAssignable)
	FOnInteract OnInteract;

	UPROPERTY(BlueprintAssignable)
	FOnInteractEnd OnInteractEnd;

	void SetupInputComponent() override;

protected:
	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Jump();

	UFUNCTION()
	void StopJumping();

	UFUNCTION()
	void StartInteract();

	UFUNCTION()
	void Interact();

	UFUNCTION()
	void StopInteract();

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;
};
