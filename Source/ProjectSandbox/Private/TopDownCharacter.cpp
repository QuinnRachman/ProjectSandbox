// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownCharacter.h"

#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ATopDownCharacter::ATopDownCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	SpringArmComponent->AddWorldTransform(FTransform(FVector(0.f, 0.f, 44.f)));
	SpringArmComponent->AddLocalRotation(FRotator(-60.f, 0.f, 0.f));
	SpringArmComponent->TargetArmLength = 900.f;
	
	GetMesh()->SetWorldRotation(FRotator(0.f, -90.f, 0.f));
	GetMesh()->AddLocalTransform(FTransform(FVector(0.f, 0.f, -88.f)));

	IsRunning = true;
	CurrentSpeed = 0.f;
	RunningSpeed = 5.f;
	WalkingSpeed = RunningSpeed / 2.f;
}

// Called when the game starts or when spawned
void ATopDownCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetLocalViewingPlayerController()->SetShowMouseCursor(true);
}

// Called every frame
void ATopDownCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATopDownCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATopDownCharacter::MovePlayerByClick()
{
	if (IsRunning)
	{
		CurrentSpeed = RunningSpeed;
	}
	else
	{
		CurrentSpeed = WalkingSpeed;
	}
	
	FHitResult Result;

	if (GetLocalViewingPlayerController()->GetHitResultUnderCursor(ECC_Camera, false, Result))
	{
		FVector Direction = GetActorLocation() - Result.Location;
		Direction.Z = 0.0;

		if (Direction.Equals(FVector(40.f, 40.f, 0), 40.0))
		{
			return;
		}

		const FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();

		GetArrowComponent()->SetWorldRotation(Rotation);
		GetMesh()->SetWorldRotation(Rotation);
		GetMesh()->AddWorldRotation(FRotator(0.f, 90, 0.f));

		// For some reason GetForwardVector gives the opposite values? TODO: Look into it more
		const FVector Vector = GetArrowComponent()->GetForwardVector();
		const double ForwardX = Vector.X * CurrentSpeed * -1;
		const double ForwardY = Vector.Y * CurrentSpeed * -1;
		
		AddActorLocalTransform(FTransform(FVector(ForwardX, ForwardY, 0.0)));
	}
}
