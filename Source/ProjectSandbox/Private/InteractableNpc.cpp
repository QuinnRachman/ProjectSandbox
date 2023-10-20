// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableNpc.h"

// Sets default values
AInteractableNpc::AInteractableNpc()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AInteractableNpc::BeginPlay()
{
	Super::BeginPlay();

	ConversationHistory.Add(AMainGameModeBase::CreateMessageAs(AMainGameModeBase::System, ReturnFullPrompt()));
}

// Called every frame
void AInteractableNpc::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AInteractableNpc::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

FString AInteractableNpc::ReturnFullPrompt()
{
	return FString::Printf(
		TEXT(R"(
			%s. Your name is %s. Your gender is %s. Your job is %s. You're in the %s. Keep responses within 1 to 3 sentences.
			You only have knowledge based on what is written under "Context".
			Context: %s
		)"),
		*PersonalityPrompt,
		*Name,
		*Gender,
		*Job,
		*Region,
		*Context
	);
}

