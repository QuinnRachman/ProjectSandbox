// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "TopDownCharacter.generated.h"

UCLASS()
class ATopDownCharacter : public ACharacter
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* CameraComponent;

	UPROPERTY()
	float WalkingSpeed;

	UPROPERTY()
	float RunningSpeed;

	UPROPERTY()
	float CurrentSpeed;

	UPROPERTY()
	bool IsRunning;

	UFUNCTION(BlueprintCallable)
	void MovePlayerByClick();

public:
	// Sets default values for this character's properties
	ATopDownCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	float GetCurrentSpeed()
	{
		return CurrentSpeed;
	}

	UFUNCTION(BlueprintCallable)
	float GetWalkingSpeed()
	{
		return WalkingSpeed;
	}

	UFUNCTION(BlueprintCallable)
	float GetRunningSpeed()
	{
		return RunningSpeed;
	}

	UFUNCTION(BlueprintCallable)
	void ToggleRunning()
	{
		IsRunning = !IsRunning;
	}

	UFUNCTION(BlueprintCallable)
	void StopMovement()
	{
		CurrentSpeed = 0;
	}
};
